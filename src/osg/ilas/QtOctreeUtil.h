#ifndef QTOCTREEUTIL_H
#define QTOCTREEUTIL_H

#include "QtOctreeBBox.h"
#include "QtOctreeIndex.h"
#include "QtPoint3D.hpp"
#include "lasconv_global.h"

typedef QVector<qint64> QtClassSummary;
Q_DECLARE_METATYPE(QtClassSummary)

///
/// \brief 八叉树工具类
///
class LASCONV_EXPORT QtOctreeUtil
{
public:
  // 构造函数，需指定点云包围盒以及点云层数，计算时默认使用该层数
  QtOctreeUtil(const QtOctreeBBox &bbox, int level = QtOctreeIndex::MAX_LEVEL());

  //根据顶点和八叉树层级计算所在八叉树的路径编码
  QtOctreeIndex    CoordToIndex(const QtPoint3D &pt, int level = -1) const;   // 坐标值反算在指定层数下的节点索引，若层数小于0，则使用默认值
  QtOctreeIndex    CoordToIndex(const QtPointEx3D &pt, int level = -1) const; // 坐标值反算在指定层数下的节点索引，若层数小于0，则使用默认值
  QtOctreeBBox     BBoxOfIndex(const QtOctreeIndex &index) const;             // 当前最大包围盒下，索引值计算对应节点的包围盒
  QtOctreeIndexVec IndexesInBBox(const QtOctreeBBox &bbox) const;

public:
  // 定义属性名接口的宏
#define DEFINE_OCTREE_ATTRIBUTE(name, value) static QString name() { return QString(value); }

  // 点云属性名，需驱动接口支持
  DEFINE_OCTREE_ATTRIBUTE(ATTR_CRS,          "crs")          // short
  DEFINE_OCTREE_ATTRIBUTE(ATTR_FIELDS,       "fields")       // QtPointFields
  DEFINE_OCTREE_ATTRIBUTE(ATTR_NUM_POINT,    "num_point")    // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_X_SCALE,      "x_scale")      // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_Y_SCALE,      "y_scale")      // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_Z_SCALE,      "z_scale")      // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_X_OFFSET,     "x_offset")     // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_Y_OFFSET,     "y_offset")     // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_Z_OFFSET,     "z_offset")     // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_MIN_X,        "min_x")        // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_MIN_Y,        "min_y")        // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_MIN_Z,        "min_z")        // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_MAX_X,        "max_x")        // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_MAX_Y,        "max_y")        // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_MAX_Z,        "max_z")        // double
  DEFINE_OCTREE_ATTRIBUTE(ATTR_CLASSES,      "classes")      // QtClassSummary

private:
  QtOctreeBBox m_bbox;
  int          m_nLevel;
  double       m_dbGridX;
  double       m_dbGridY;
  double       m_dbGridZ;
};

#endif // QTOCTREEUTIL_H
