#ifndef QTTILEFILE_H
#define QTTILEFILE_H

#include <QObject>
#include <QMutex>
#include <QFile>
#include "QtTileField.h"
#include "QtTileBlock.h"
#include "QtTileHeader.h"
#include "QtTileUserData.h"
#include "QtTempBaseFile.h"
#include "QtOctree.h"
#include "QtPoint3D.hpp"
#include "QtProgressTask.h"
#include "QtPointCompressor.h"
#include "lasconv_global.h"

#include "QtLruCache.h"

///
/// \brief 分块文件驱动，提供两个核心功能：1）生成新文件；2）读写已有文件
///
class LASCONV_EXPORT QtTileFile : public QObject
{
  Q_OBJECT
public:
  ///
  /// \brief 统计选项，用于更新指定的统计数据
  ///
  struct StatisticsOptions
  {
  public:
    QString m_strClassField;               // 分类字段名，默认为空，不为空时更新分类统计
    QString m_strReturnField;              // 回波数字段名，默认为空，不为空时更新回波数统计
    QString m_strIntensityField;           // 强度字段名，默认为空，不为空时更新强度统计

    bool m_bUpdateNumOfPoints;             // 更新点总数，默认为false，为true时重新统计点总数
    bool m_bUpdateNumOfPointsByElevation;  // 更新高程统计，默认为false，为true时重新统计高程

    bool    m_bAvgStdElevation;            // 扩展数据，高度平均值&标准差
    QString m_strAvgStdIntensity;          // 扩展数据，强度平均值&标准差
    QString m_strMinMaxGpstime;            // 扩展数据，GPS最大最小值

  public:
    StatisticsOptions() :
      m_bUpdateNumOfPoints(false),
      m_bUpdateNumOfPointsByElevation(false),
      m_bAvgStdElevation(false)
    {}

    bool IsValid() const { return !m_strClassField.isEmpty() ||
                                  !m_strReturnField.isEmpty() ||
                                  !m_strIntensityField.isEmpty() ||
                                   m_bUpdateNumOfPoints ||
                                   m_bUpdateNumOfPointsByElevation ||
                                   m_bAvgStdElevation ||
                                  !m_strAvgStdIntensity.isEmpty() ||
                                  !m_strMinMaxGpstime.isEmpty(); }
  };

public:
  explicit QtTileFile(QObject *parent = nullptr);
  virtual ~QtTileFile();

  QString LastError() const; // 错误字符串
  QString FileName() const;  // 文件路径

  /* IO接口（用于读写已存在的数据） */
  bool IsOpen() const;                                       // 是否已打开数据文件
  bool IsReadOnly() const;                                   // 是否以只读方式打开
  bool Open(const QString &strFile, bool bReadOnly = false); // 打开已存在的文件，程序内同一个文件最多只能有一个非只读打开的实例
  void Close();                                              // 关闭已打开的文件

  bool Compressed() const;  // 数据是否已压缩

  QtPointQuantizer   PointQuantizer() const;   // 获取点坐标转换器（int与double互转）
  QtPointCompressor* PointCompressor() const;  // 获取点数据压缩器（压缩与解压）

  QtTileHeader Header() const;  // 获取文件头信息
  QtTileFields Fields() const;  // 获取文件字段信息
  QtTileBlocks Blocks() const;  // 获取文件块信息

  QtTileUserData UserData() const;   // 获取文件用户自定义数据

  int           PointSize() const;   // 计算点数据大小（字节）
  QtPointFields PointFields() const; // 获取点字段信息（由文件字段信息转换而来）

  quint64 NumOfPoints();   // 快速获取点数量（与头信息一致）
  quint32 NumOfFields();   // 快速获取字段数量（与头信息一致）
  quint32 NumOfBlocks();   // 快速获取块数量（与头信息一致）

  //会将当前block的成果通过lru的方式缓存起来，以方便后面的调用
  const QtPointEx3DVec *ReadBlock(const quint32 block);
  bool ReadBlock(const quint32 block, QtPointEx3DVec &points, QtPointIndexes &indexes); // 读取块数据，输入块索引，返回点数据以及点索引
  bool ReadBlock(const quint32 block, QtPointIndexes &indexes);                         // 读取块数据，输入块索引，返回点索引
  bool WriteBlock(const quint32 block, const QtPointEx3DVec &points);                   // 写入块数据，输入块索引以及块数据，块数据必须与块大小一致

  bool HasPoint(const QtPointIndex index);                                              // 判断指定点索引是否存在
  bool ReadPoints(const QtPointIndexes &indexes, QtPointEx3DVec &points);               // 读取点数据，输入指定点索引（文件中任意存在的点），返回对应点数据
  bool WritePoints(const QtPointIndexes &indexes, const QtPointEx3DVec &points);        // 写入点数据，输入指定点索引（文件中任意存在的点）以及索引对应点数据

  bool UpdateField(const QtPointIndexes &indexes, const QString &field, const QVariant &value);              // 更新点字段数据，输入点索引、字段名、字段值
  bool UpdatePoints(const QtPointIndexes &indexes, std::function<bool(QtPointEx3D &pt, bool &cancel)> when); // 更新点数据，输入点索引和判断回调，在回调修改点数据后需返回true，否则不更新
  bool UpdateHeader(const QtTileHeader &header);                                                             // 更新头信息，需要谨慎修改，错误的信息可能会破坏后续的所有读写操作
  bool UpdateUserData(const QtTileUserData &data);                                                           // 更新用户数据
  bool UpdateStatistics(const StatisticsOptions &options);                                                   // 更新统计，仅修改选项指定的数据，不影响其他现有数据

  QtOctree* CreateOctree();  // 创建绑定的八叉树，销毁八叉树实例时本实例会自动销毁，关闭文件或销毁本实例时会自动销毁八叉树
  void      DestroyOctree(); // 销毁绑定的八叉树，但不影响本文件实例
  void ClearLruCache(void);

  static QtTileFile* FileOfTree(QtOctree *pTree); // 查找八叉树绑定的分块文件指针，只有从分块文件接口创建的八叉树才存在绑定关系
  /* ~IO接口 */

  /* 分块接口（用于生成新数据文件，与IO接口互斥） */
  /* 常规调用方式如下：
   * 1)BeginTiling
   * 2)BeginIndexing
   * 3)DoIndexing --> Loop
   * 4)EndIndexing
   * 5)BeginFilling
   * 6)DoFilling --> Loop
   * 7)EndFilling
   * 8)EndTiling
   *
   * 注意：如果需要中断，可在任意阶段直接调用EndTilling(true)以强制结束分块并关闭底层文件实例
   */
  bool IsTiling() const;                                     // 是否正在进行分块操作
  bool BeginTiling(const QString &strFile,
                   const QtTileHeader &header,
                   const QtTileFields &fields,
                   QtProgressTask::Ptr progress = QtProgressTask::Ptr(nullptr));  // 开始分块（即生成新文件），输入文件路径、文件头、字段信息；其中文件头必须包含范围、缩放、偏移值；

  bool IsIndexing() const;                           // 是否正在构建索引
  bool BeginIndexing();                              // 开始构建索引
  bool DoIndexing(const QtPoint3D &point);           // 输入索引数据
  bool DoIndexing(const QtPoint3DVec &points);       // 输入索引数据，仅需要xyz
  bool DoIndexing(const QtPointEx3DVec &points);     // 输入索引数据，仅需要xyz
  bool DoIndexing(double x, double y, double z);     // 输入索引数据
  bool EndIndexing(qint64 nMergingThreshold = -1);   // 结束构建索引

  bool IsFilling() const;                                                    // 是否正在填充数据
  bool BeginFilling(int nBufferSize = 10000,
                    const StatisticsOptions &options = StatisticsOptions()); // 开始填充数据，输入缓冲区大小（字节）、统计选项
  bool DoFilling(const QtPointEx3D &point);                                  // 填充数据
  bool DoFilling(const QtPointEx3DVec &points);                              // 填充数据
  bool DoFilling(const QByteArray &points, int nNumOfPoints);                // 填充数据
  bool EndFilling();                                                         // 结束填充数据，注意：填充的数据量以及xyz，必须与构建索引阶段的输入一致

  bool EndTiling(bool bForce = false); // 结束分块，输入是否强制结束；注意：如果在分块过程中需要中断，必须设置强制参数，否则不会正确重置状态
  /* ~分块接口 */

  //写块信息，慎用，导致文件损坏。
  bool WriteTileBlocks(const QtTileBlocks &blocks, bool update = false);

private:
  bool ReadTileHeader();
  bool ReadTileFields();
  bool ReadTileBlocks();
  bool ReadTileUData();
  void UpdatePointSize();

  void CreateOrUpdateCompressor(bool bOnlyCreate) const;

  const QtPointEx3DVec *ReadBlockPrivate(quint32 block);
  bool ReadBlockPrivate(const quint32 block, QtPointEx3DVec &points, QtPointIndexes &indexes);
  bool ReadBlockPrivate(const quint32 block, QtPointIndexes &indexes);

  bool WriteTileHeader(const QtTileHeader &header);
  bool WriteTileUData(const QtTileUserData &data);
  bool WriteTileFields(const QtTileFields &fields);

  bool WritePointsBuffer(const QtPointEx3D &point, QByteArray &buffer, int offset = 0);
  bool WritePointsBuffer(const QtPointEx3DVec &points, QByteArray &buffer, int offset = 0);
  bool WritePointsPrivate(const QtPointIndexes &indexes, const QByteArray &buffer, bool bTiling = false);
  bool WritePointsPrivateEx(const QtPointIndexes &indexes, const QByteArray &buffer, bool bTiling = false);

  bool FillTilingBuffer(const QtPointEx3D &point);
  bool FillTilingBuffer(const QtPointEx3DVec &points);
  bool FillTilingBuffer(const QByteArray &points, int nNumOfPoints);
  bool FlushTilingBuffer(bool bForce = false);

  bool CheckCancel();
  bool CheckOpen(bool bNoTiling = true);
  bool CheckTiling(bool bBegin = false);
  bool CheckIndexing(bool bBegin = false);
  bool CheckFilling(bool bBegin = false);
  bool RaiseError(const QString &strError);
  void RegisterCallbacksOnTree();

  qint64 OffsetOfPoint(const QtPointIndex index, QtTileBlock &lastBlock, bool &bLastRead);

  static int    NumOfLayers(qint64 nNumOfPoints, uint nLevel);
  static int    LayerOfPoint(qint64 nPoint, int nNumOfLayers);
  static qint64 SizeOfLayer(qint64 nNumOfPoints, int nNumOfLayers, int nLayer);

  static void Bind(QtOctree *pTree, QtTileFile *pFile);
  static void Unbind(QtOctree *pTree);

private:
  /* 基础信息 */
  QFile          m_file;
  QtTileHeader   m_header;
  QtTileUserData m_udata;
  QtTileFields   m_fields;
  QtTileBlocks   m_blocks;
  int            m_nPointSize;
  QByteArray     m_arrWritePointsBuffer;

  QString m_strLastError;

  mutable QtPointCompressor *m_pPointCompressor;

  /* 分块 */
  bool m_bTiling;
  bool m_bIndexing;
  bool m_bFilling;

  qint64         m_nIndexingNumOfPoints;
  QByteArray     m_arrTilingPointsBuffer;
  QtPointIndexes m_vecTilingPointIndexBuffer;

  QtOctreeUtil                       m_clsTilingOctreeUtil;
  QtPointQuantizer                   m_clsTilingQuantizer;
  QMap<QtOctreeIndex, qint64>        m_mapTilingIndexToNum;
  QMap<QtOctreeIndex, QtOctreeIndex> m_mapTilingIndexToIndex;
  QMap<QtOctreeIndex, int>           m_mapTilingIndexToLayers;
  QMap<QtOctreeIndex, qint64>        m_mapTilingIndexToWritten;
  QMap<uint, qint64>                 m_mapTilingLayerToWritten;
  int                                m_nTilingPointBufferSize;
  StatisticsOptions                  m_clsTilingStatistics;
  QtProgressTask::Ptr                m_ptrTilingProgress;

  qint32 m_nTilingElevationMin;
  qint64 m_nTilingElevationNum;
  qint64 m_nTilingIntensityNum;
  double m_dbTilingElevationSum;
  double m_dbTilingIntensitySum;
  double m_dbTilingMinGpstime;
  double m_dbTilingMaxGpstime;

  /* 访问加速 */
  QMap<quint32, int> m_mapBlockToIndex;

  /* 八叉树/IO */
  QMutex    m_mtxIO;
  QtOctree *m_pOctree;

  /* 文件绑定 */
  static QMutex                       s_mtxBinding;
  static QMap<QtOctree*, QtTileFile*> s_mapOctreeToFile;
  //
  QtLruCache<qint32,QtPointEx3DVec> *m_pBlockPointsCache{nullptr};
};

#endif // QTTILEFILE_H
