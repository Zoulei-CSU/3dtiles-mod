#ifndef QTOCTREEBBOX_H
#define QTOCTREEBBOX_H

#include <QList>
#include "QtPoint3D.hpp"
#include "lasconv_global.h"

#pragma pack(push)
#pragma pack(1)

///
/// \brief 八叉树网格空间包围盒
///
struct LASCONV_EXPORT QtOctreeBBox
{
public:
  QtOctreeBBox(const QtPoint3D &pt = QtPoint3D());
  QtOctreeBBox(const QtPoint3D &min, const QtPoint3D &max);

  QtOctreeBBox operator+(const QtOctreeBBox &other) const;
  QtOctreeBBox operator+=(const QtOctreeBBox &other) const;

  bool IsValid() const;                              // 包围盒是否有效（最大最小不相等）
  void Update(const QtPoint3D &pt);                  // 扩展包围盒，使之包含指定点
  void Update(const QtOctreeBBox &other);            // 扩展包围盒，使之等于两者之和
  bool Contains(const QtPoint3D &pt) const;          // 是否包含点
  bool Contains(const QtOctreeBBox &other) const;          // 是否包含点
  bool Intersect(const QtOctreeBBox &other) const;          // 是否包含点
  bool CollideWith(const QtOctreeBBox &other) const; // 是否与另一包围盒碰撞

  QVector<QtOctreeBBox> Split() const; // 计算八个切分包围盒

  QtPoint3D Min() const;
  QtPoint3D Max() const;
  QtPoint3D Size() const;
  QtPoint3D Center() const;

  QVector<QtPoint3D> Vertexes() const; // 计算包围盒的八个顶点

private:
  QtPoint3D m_min;
  QtPoint3D m_max;
  QtPoint3D m_size;
};

#pragma pack(pop)

#endif // QTOCTREEBBOX_H
