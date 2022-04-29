#ifndef QTOCTREEDATA_H
#define QTOCTREEDATA_H

#include <QObject>
#include "QtPoint3D.hpp"
#include "QtOctreeBlock.h"
#include "QtPointQuantizer.hpp"
#include "lasconv_global.h"

class QtOctreeBlock;

#pragma pack(push)
#pragma pack(1)

///
/// \brief 八叉树网格空间数据
///
class LASCONV_EXPORT QtOctreeData : public QObject
{
  Q_OBJECT
protected:
  explicit QtOctreeData(QtOctreeBlock *block);
  virtual ~QtOctreeData();

  void Lock();
  void Unlock();

  bool UpdatePoints(const QtPointEx3DVec &points);
  bool AddPoint(const QtPointEx3D &pt, QtPointIndex i);
  bool RemovePoints(const QtPointIndexes &ids);

public:
  QString LastError() const;

  bool WasInitialized() const;
  bool Initialize(const QtPointEx3DVec &pts);
  void Squeeze();

  bool IsLocked() const;
  bool Contains(const QtPointIndex idx) const;

  int            Size() const;
  QtOctreeIndex  Index() const;
  QtPointIndexes PointIndexes() const;

  QtOctreeBlock* Block() const;
  QtPointEx3DVec Points() const;
  QtPointEx3D    PointAt(int i) const;

  QtPointEx3DVec Extract(const QtPointIndexes &ids);

private:
  bool RaiseError(const QString &strError);

private:
  bool    m_bLock;
  QString m_strLastError;

  QtOctreeBlock *m_pBlock;
  QtPointEx3DVec m_vPoints;

  friend class QtOctree;
  friend class QtOctreeBlock;
};

#pragma pack(pop)

typedef QVector<QtOctreeData*> QtOctreeDataVec;

#endif // QTOCTREEDATA_H
