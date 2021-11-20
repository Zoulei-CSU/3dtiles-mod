#ifndef QTOCTREEBLOCK_H
#define QTOCTREEBLOCK_H

#include <QObject>
#include "QtPoint3D.hpp"
#include "QtOctreeIndex.h"
#include "lasconv_global.h"

class QtOctreeData;

#pragma pack(push)
#pragma pack(1)

///
/// \brief 八叉树网格空间中的单个网格块
///
class LASCONV_EXPORT QtOctreeBlock : public QObject
{
  Q_OBJECT
protected:
  explicit QtOctreeBlock(const QtOctreeIndex &idx, QObject *parent = 0);
  virtual ~QtOctreeBlock();

  void Lock();
  void Unlocked();

public:
  bool Initialize(const QtPointIndexes &ids, bool compress = true);
  void Squeeze();

  bool IsCompressed() const;
  void Compress();

  bool IsLocked() const;
  bool Contains(const QtPointIndex idx) const;
  bool WasInitialized() const;

  int            Size() const;
  QtOctreeIndex  Index() const;
  QtPointIndexes PointIndexes() const;
  QtPointIndex   PointIndexAt(int i) const;

protected:
  void BindData(QtOctreeData *pData);
  void AddIndex(QtPointIndex idx);
  int  RemoveIndex(QtPointIndex idx);

private:
  bool RaiseError(const QString &strError);

private:
  bool    m_bLocked;
  QString m_strLastError;

  QtOctreeIndex  m_clsBlockIndex;
  QtPointIndexes m_clsPointIndexes;
  QtOctreeData  *m_pBindingData;

  bool    m_bCompressed;
  quint64 m_nCompressOffset;
  QVector<quint32> m_vecCompressedIndexes;

  friend class QtOctree;
  friend class QtOctreeData;
};

#pragma pack(pop)

typedef QVector<QtOctreeBlock*> QtOctreeBlockVec;

#endif // QTOCTREEBLOCK_H
