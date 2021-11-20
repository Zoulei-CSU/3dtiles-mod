#ifndef QTOCTREE_H
#define QTOCTREE_H

#include <QObject>
#include "QRegion.h"
#include <QMap>
#include <QMutex>
#include <QRectF>
#include <QPolygonF>
#include <QMatrix4x4>
#include <functional>
#include "QtOctreeBBox.h"
#include "QtOctreeBlock.h"
#include "QtOctreeData.h"
#include "QtOctreeIndex.h"
#include "QtOctreeNode.h"
#include "QtOctreeUtil.h"
#include "lasconv_global.h"

///
/// \brief 点云八叉树
///
class LASCONV_EXPORT QtOctree : public QObject
{
  Q_OBJECT
public:
  /*
   * 八叉树数据操作的回调函数定义；
   * 通过注册回调函数，能够将点云底层的数据驱动与八叉树实现分离；
   *
   * 目前实现中提供了静态模式和动态模式：
   * 1、静态模式：最常用的模式，也是默认的模式；初始化后树结构不再更新（点属性可修改，需要注册对应回调），通常用于经过分块预处理的数据；
   * 2、动态模式：初始化后可以动态添加和删除数据（需要注册对应回调），所有修改通过回调与底层驱动同步；
   */
  typedef std::function<bool(const QtOctreeIndex&, const QtPointIndexes&)>                        AddBlockFunc;      // 添加块，参数：块号，点索引集
  typedef std::function<bool(const QtOctreeIndex&)>                                               RemoveBlockFunc;   // 移除块，参数：块号
  typedef std::function<bool(const QtOctreeIndex&, const QtPointIndexes&, const QtPointEx3DVec&)> AddPointsFunc;     // 添加点，参数：块号，点索引集，点数据
  typedef std::function<bool(const QtPointIndexes&, const QtOctreeIndexVec&)>                     RemovePointsFunc;  // 移除点，参数：点索引集，块范围（为空则搜索所有）
  typedef std::function<bool(const QtOctreeIndex&, QtPointEx3DVec&)>                              ReadBlockFunc;     // 读取块，参数：块号，点数据
  typedef std::function<bool(const QtPointIndexes&, QtPointEx3DVec&)>                             ReadPointsFunc;    // 读取点，参数：点索引集，点数据
  typedef std::function<bool(const QtOctreeIndex&, const QtPointEx3DVec&)>                        UpdatePointsFunc;  // 更新点，参数：块号，点数据（数据量必须和原来一致）
  typedef std::function<bool(const QString&, QVariant&)>                                          GetAttributeFunc;  // 读属性，参数：属性名，属性值
  typedef std::function<bool(const QString&, const QVariant&)>                                    SetAttributeFunc;  // 写属性，参数：属性名，属性值

public:
  explicit QtOctree(bool bStatic, const QtOctreeBBox &bbox, const QString &file, QObject *parent = nullptr);
  virtual ~QtOctree();

  QString LastError() const;
  QString SourceFile() const;

  /* 是否为静态/动态模式 */
  bool IsStatic() const;
  bool IsDynamic() const;
  bool IsInitializing() const;

  /* 动态模式下，块数据内存保留限制（块数） */
  int  DynamicLimit() const;                     // 动态缓存块数限制
  void SetDynamicLimit(int nLimit);              // 设置动态缓存块数限制
  void AutoDynamicLimit(int nPoints = 1000000);  // 根据点数和平均块大小自动设置限制

  /* 初始化 */
  bool BeginInitializing();                                                           // 开始初始化
  bool InitNodesToLevel(uint nLevel = QtOctreeIndex::MAX_LEVEL());                    // 创建节点，到最大层数结束
  bool InitNodesOnBlock();                                                            // 根据当前已添加的块反向（自下而上）创建节点
  bool InitMergeToLevel(uint nLevel);                                                 // 统一合并到指定层级
  bool InitMergeToLimit(int nLimit);                                                  // 尽可能合并已添加的节点，并将单块数据量限制在阈值下
  bool InitAddBlock(const QtOctreeIndex &idx, QtOctreeBlock **block);                 // 添加一个块
  bool InitRemoveBlock(const QtOctreeIndex &idx);                                     // 移除一个块
  bool InitRemoveBlockForce(const QtOctreeIndex &idx);                                // 强制移除一个块，即使块已锁定
  bool InitAddPoint(const QtOctreeIndex &idx, const QtPointEx3D &pt, QtPointIndex i); // 添加点到指定block，block不存在时自动创建
  void EndInitializing();                                                             // 结束初始化

  /* 坐标转换器 */
  QtPointQuantizer* Quantizer() const;

  /* 属性 */
  QtOctreeBBox     BBox() const;
  QtOctreeNode*    Root() const;
  int              Depth() const;

  /* 获取块信息 */
  int              BlockCount() const;                           // 块数量
  QtOctreeBlockVec Blocks() const;                               // 获取所有块信息
  QtOctreeBlockVec Blocks(const QtOctreeIndex &idx) const;       // 搜索指定索引的块信息，模糊搜索，仅匹配块号
  QtOctreeBlockVec Blocks(const QtOctreeBBox &bbox) const;       // 搜索与包围盒有交集的块信息
  QtOctreeBlockVec Blocks(const QtPoint3D &pt, int dist) const;  // 搜索点周边的块信息
  QtOctreeBlock*   Block(const QtOctreeIndex &idx) const;        // 搜索指定索引的块信息，精确搜索，匹配块号和层号
  bool             HasBlock(const QtOctreeIndex &idx) const;     // 判断是否包含指定索引的块信息

  // 投影范围搜索，region为二维平面范围，mat为投影矩阵，ortho为正射投影多边形
  QtOctreeBlockVec BlocksInRegion(const QRegion &region, const QMatrix4x4 &mat, const QPolygonF &ortho = QPolygonF());

  /* 添加/移除数据，仅在动态模式下且驱动支持时可用 */
  QtOctreeBlock*   AddBlock(const QtOctreeIndex &idx, const QtPointIndexes &ids);
  bool             RemoveBlock(const QtOctreeIndex &idx);
  bool             AddPoints(const QtOctreeIndex &block, const QtPointIndexes &ids, const QtPointEx3DVec &points);
  bool             RemovePoints(const QtPointIndexes &points, const QtOctreeIndexVec &blocks);
  bool             UpdatePoints(const QtOctreeIndex &block, const QtPointEx3DVec &points);

  bool             GetAttribute(const QString &name, QVariant &value);
  bool             SetAttribute(const QString &name, const QVariant &value);

  /* 请求/释放块数据，支持动态加载 */
  QtOctreeData*    RequireBlock(const QtOctreeIndex &idx);        // 请求单个块数据
  void             ReleaseBlock(const QtOctreeIndex &idx);        // 释放单个块数据
  bool             IsBlockLocked(const QtOctreeIndex &idx);       // 判断块是否已锁定

  QtOctreeDataVec  RequireBlocks(const QtOctreeIndexVec &blocks); // 一次请求多个块数据，提高效率
  QtOctreeDataVec  RequireBlocks(const QtOctreeBlockVec &blocks);
  void             ReleaseBlocks(const QtOctreeIndexVec &blocks); // 一次释放多个块数据，提高效率
  void             ReleaseBlocks(const QtOctreeBlockVec &blocks);

  /* 数据操作驱动接口注册，仅在动态模式下有效 */
  void RegisterAddBlockFunc(AddBlockFunc func);
  void RegisterRemoveBlockFunc(RemoveBlockFunc func);
  void RegisterAddPointsFunc(AddPointsFunc func);
  void RegisterRemovePointsFunc(RemovePointsFunc func);
  void RegisterReadBlockFunc(ReadBlockFunc func);
  void RegisterReadPointsFunc(ReadPointsFunc func);
  void RegisterUpdatePointsFunc(UpdatePointsFunc func);
  void RegisterGetAttributeFunc(GetAttributeFunc func);
  void RegisterSetAttributeFunc(SetAttributeFunc func);

private:
  bool CheckDynamic();
  bool CheckInitializing();
  bool MergeNodes(const QVector<QtOctreeNode*> vecNodes);
  bool RaiseError(const QString &strError);

  QtOctreeBlock* AddBlockPrivate(const QtOctreeIndex &idx);
  bool           RemoveBlockPrivate(const QtOctreeIndex &idx, bool force = true);

  QtOctreeData*  RequireBlockPrivate(const QtOctreeIndex &idx);
  void           ReleaseBlockPrivate(const QtOctreeIndex &idx);

  void ConvexHull(QVector<QPoint> &vertexes);

  template<typename T>
  void Append(QVector<T> &vecDest, QVector<T> &vecSrc);
  bool OctreeIntersects(const QPolygonF &frist, const QPolygonF &p) const;

private:
  bool    m_bStatic;
  bool    m_bInitializing;
  int     m_nDynamicLimit;
  QString m_strSourceFile;
  QString m_strLastError;

  QtPointQuantizer                   *m_pQuantizer;       // 存储值与真实值的转换器
  QtOctreeBBox                        m_clsBBox;          // 八叉树包围盒
  QtOctreeNode                       *m_pRootNode;        // 根节点
  QMap<QtOctreeIndex, QtOctreeBlock*> m_mapIdxToBlock;    // 块索引到块实例的映射
  QMap<QtOctreeIndex, QtOctreeData*>  m_mapIdxToData;     // 块索引到块数据的映射
  QSet<QtOctreeIndex>                 m_setLockedData;    // 被锁定的块数据索引
  QVector<QtOctreeIndex>              m_vDataStack;       // 已加载的块数据栈
  QVector<qint64>                     m_vLevelStatistic;  // 节点等级统计，添加/删除块时更新，主要用于查询八叉树最大深度
  QMutex                              m_mtxDataLock;      // 块数据操作锁
  QMutex                              m_mtxInitLock;      // 初始化操作锁

  AddBlockFunc     m_funcAddBlock;       // 添加块回调
  RemoveBlockFunc  m_funcRemoveBlock;    // 移除块回调
  AddPointsFunc    m_funcAddPoints;      // 添加点回调
  RemovePointsFunc m_funcRemovePoints;   // 移除点回调
  ReadBlockFunc    m_funcReadBlock;      // 读取块回调
  ReadPointsFunc   m_funcReadPoints;     // 读取点回调
  UpdatePointsFunc m_funcUpdatePoints;   // 更新点回调
  GetAttributeFunc m_funcGetAttribute;   // 读属性回调
  SetAttributeFunc m_funcSetAttribute;   // 写属性回调
};

#endif // QTOCTREE_H
