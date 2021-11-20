#ifndef QTOCTREENODE_H
#define QTOCTREENODE_H

#include <QObject>
#include "QtOctreeBBox.h"
#include "QtOctreeIndex.h"
#include "QtOctreeBlock.h"
#include "lasconv_global.h"

class QtOctree;

#pragma pack(push)
#pragma pack(1)

///
/// \brief 八叉树节点
///
class LASCONV_EXPORT QtOctreeNode : public QObject
{
  Q_OBJECT
public:
  using NodeBBox  = QtOctreeBBox;
  using NodeIndex = QtOctreeIndex;

protected:
  explicit QtOctreeNode(QtOctree *pTree, const NodeBBox &bbox, const NodeIndex &idx, QtOctreeNode *pParent = 0);
  virtual ~QtOctreeNode();

  bool Split();
  void Merge();

  QtOctreeNode* SplitWithIndex(const NodeIndex &child);

public:
  bool IsRoot() const;
  bool IsLeaf() const;
  bool IsInter() const;

  QtOctreeBBox  BBox() const;
  QtOctreeIndex Index() const;

  QtOctree*               Tree() const;
  QtOctreeNode*           Parent() const;
  QVector<QtOctreeNode*>  Children() const;
  QVector<QtOctreeBlock*> BindingBlocks() const;

private:
  NodeBBox  m_bbox;
  NodeIndex m_index;

  QtOctree     *m_pTree;
  QtOctreeNode *m_pParent;

  friend class QtOctree;
};

#pragma pack(pop)

#endif // QTOCTREENODE_H
