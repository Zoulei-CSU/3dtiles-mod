#ifndef QTOCTREEINDEX_H
#define QTOCTREEINDEX_H

#include <QVector>
#include <QMetaType>
#include "QtPoint3D.hpp"
#include "lasconv_global.h"

#pragma pack(push)
#pragma pack(1)

///
/// \brief 节点索引，由 块号-树层级-块层级 三个分部构成32位无符号索引
///
/// 块号：块在网格空间中的XYZ坐标，经过树层级式编码形成；
/// 树层级：块在八叉树中的层级；
/// 块层级：八叉树节点，即单个空间网格中，内部进行金字塔数据拆分的“扩展”层级，不存在于常规八叉树的概念中；
///
struct LASCONV_EXPORT QtOctreeIndex
{
public:
  static uint MAX_BLOCK(); // 支持的最大块号
  static uint MAX_LEVEL(); // 支持的最大树层级（八叉树分层）
  static uint MAX_LAYER(); // 支持的最大块层级（叶节点垂直分层）

  typedef QtPoint3DTemplate<uint> Grid; // 三维网格空间分块位置

public:
  explicit QtOctreeIndex(uint index = 0);                     // 从索引值初始化
  explicit QtOctreeIndex(uint child, QtOctreeIndex *parent);  // 根据子节点序号，从父节点索引计算
  explicit QtOctreeIndex(Grid pos, uint level, uint layer);   // 从空间分块位置计算
  explicit QtOctreeIndex(uint block, uint level, uint layer); // 从块号、树层级、块层级合成

  bool operator<(const QtOctreeIndex &other) const;   // 比较索引值
  bool operator==(const QtOctreeIndex &other) const;  // 比较索引值，当索引各组成部分完全相等时，即视为相等
  operator uint() const;                              // 强制转换uint类型

  bool IsRoot() const;                                 // 是否为根节点索引，即所有分部为0
  bool IsValid() const;                                // 是否为无效的索引，即所有分部为0
  bool AncestorOf(const QtOctreeIndex &other) const;   // 判断是否为另一索引的父级
  bool BlockEqualTo(const QtOctreeIndex &other) const; // 判断是否块号相等（不比较其他分部）

  uint Index() const; // 索引值
  uint Block() const; // 索引-块号
  uint Level() const; // 索引-树层级
  uint Layer() const; // 索引-块层级

  Grid Location() const;

  uint LocalBlock() const;  // 索引所在层级的块序号（0-7）

  QtOctreeIndex Parent() const; // 父级索引

  QVector<QtOctreeIndex> Path() const;      // 当前索引（包含）到根节点（包含）的索引序列
  QVector<QtOctreeIndex> Split() const;     // 当前索引的子级索引
  QVector<QtOctreeIndex> Neighbors() const; // 获取三维空间中相邻的索引值（小于等于26个）

  static uint Assemble(uint block, uint level, uint layer);                   // 从块号-树层级-块层级计算索引值
  static void Disassemble(uint index, uint &block, uint &level, uint &layer); // 从索引值计算块号-树层级-块层级

  static uint AssembleBlockCoord(int level, uint x, uint y, uint z);                   // 指定层级的八叉树下，对叶节点的空间网格坐标进行编码，生成空间网格块号
  static void DisassembleBlockCoord(int level, uint block, uint &x, uint &y, uint &z); // 指定层级的八叉树下，解析空间网格块号，得到叶节点在空间网格的坐标

private:
  uint m_index;
};

Q_DECLARE_METATYPE(QtOctreeIndex)

#pragma pack(pop)

extern uint qHash(const QtOctreeIndex &key, uint seed = 0);

typedef QVector<QtOctreeIndex> QtOctreeIndexVec;

#endif // QTOCTREEINDEX_H
