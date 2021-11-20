#ifndef QTPOINTCLOUDVECTOR_H
#define QTPOINTCLOUDVECTOR_H

#include "QtOctree.h"
#include "QtTempIndexFile.h"

///
/// \brief 点云的数组映射类，提供类似vector的只读操作
///
class LASCONV_EXPORT QtPointCloudVector : public QObject
{
  Q_OBJECT
public:
  typedef QtPoint3DTemplate<double> QtPoint;
  typedef QVector<QtPoint>          QtPointVec;

public:
  explicit QtPointCloudVector(QtOctree *tree, QObject *parent = nullptr);
  ~QtPointCloudVector();

  quint64   Size() const;       // 点云大小（点数量）
  QtOctree* Octree() const;     // 原始八叉树
  QString   LastError() const;  // 上次运行的出错信息

  // 按索引获取点，索引为映射的顺序投影
  QtPoint operator[](QtPointIndex i) const;

  // 映射索引到原始点云索引
  QtPointIndex   MapToOriginIndex(const QtPointIndex index);
  QtPointIndexes MapToOriginIndexes(const QtPointIndexes &vecIndexes);

protected:
  void Initialize();                                 // 初始化映射信息
  bool LoadBlockForIndex(QtPointIndex i);            // 加载本地点索引所对应的块数据
  bool RaiseError(const QString &strMessage);        // 发起错误信息

  int  BlockOfPointIndex(QtPointIndex i, QtPointIndex &min, QtPointIndex &max); // 获取点所在的块本地序号

protected:
  QtOctree     *m_pTree;              // 原始八叉树
  QVector<uint> m_vecBlockSizes;      // 保存所有块大小
  QVector<uint> m_vecBlockIndexes;    // 保存所有块索引
  QString       m_strLastError;       // 保存错误信息

  QtPointVec    m_vecLastBlock;       // 最后加载的块数据（点坐标）
  int           m_nLastBlockMin;      // 最后加载的块中的最小点索引
  int           m_nLastBlockMax;      // 最后加载的块中的最大点索引
  quint64       m_nPointCount;        // 点云大小（点数量）

  QtTempIndexFile m_clsIndexFile;
};

#endif // QTPOINTCLOUDVECTOR_H
