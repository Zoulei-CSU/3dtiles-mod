#ifndef QTPOINTCLOUD_H
#define QTPOINTCLOUD_H

#include <functional>
#include <QPointer>
#include "QtOctree.h"
#include "QtFileLayer.h"
#include "QtProgressTask.h"
#include "lasconv_global.h"

class LASCONV_EXPORT QtPointCloud : public QtFileLayer
{
  Q_OBJECT
public:
  typedef std::function<QtOctree*(const QString&,bool,QtProgressTask::Ptr)> CreateOctreeFunc;

public:
  #define DEFINE_POINT_CLOUD_FIELD(entry, type, name, size) \
  static QtPointField entry() { return QtPointField(type, name, size); }

  DEFINE_POINT_CLOUD_FIELD(FIELD_INTENSITY,           QtPointField::Type_UShort, "intensity",           0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_RGB,                 QtPointField::Type_UInt64, "rgb",                 0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_CLASS,               QtPointField::Type_Byte,   "class",               0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_GPSTIME,             QtPointField::Type_Double, "gpstime",             0)

  DEFINE_POINT_CLOUD_FIELD(FIELD_RETURN_NUM,          QtPointField::Type_Byte,   "return_num",          0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_NUM_OF_RETURN,       QtPointField::Type_Byte,   "num_of_return",       0)

  DEFINE_POINT_CLOUD_FIELD(FIELD_SCAN_DIRECTION_FLAG, QtPointField::Type_Bit,    "scan_direction_flag", 0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_EDGE_OF_FLIGHT_LINE, QtPointField::Type_Bit,    "edge_of_flight_line", 0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_SYNTHETIC_FLAG,      QtPointField::Type_Bit,    "synthetic_flag",      0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_KEYPOINT_FLAG,       QtPointField::Type_Bit,    "keypoint_flag",       0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_WITHHELD_FLAG,       QtPointField::Type_Bit,    "withheld_flag",       0)

  DEFINE_POINT_CLOUD_FIELD(FIELD_SCAN_ANGLE_RANK,     QtPointField::Type_UShort, "scan_angle_rank",     0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_USER_DATA,           QtPointField::Type_Byte,   "user_data",           0)
  DEFINE_POINT_CLOUD_FIELD(FIELD_POINT_SOURCE_ID,     QtPointField::Type_Byte,   "point_source_id",     0)

public:
  explicit QtPointCloud(QObject *parent = nullptr);
  virtual ~QtPointCloud();

  /* 初始化接口 */
  virtual bool Initialize() override;
  virtual bool InitFromAttributes(const QtStrAttributes &attrs) override;

  /* 专有接口 */
  QtOctree* GetOrCreateOctree(QtProgressTask::Ptr task = QtProgressTask::Ptr(nullptr), bool bOnlyAttributes = false);
  void      DestroyOctree();

  void RegisterCreateOctreeFunc(CreateOctreeFunc func);

private:
  bool               m_bInit;
  QPointer<QtOctree> m_ptrOctree;
  CreateOctreeFunc   m_funcCreateOctree;
};

#endif // QTPOINTCLOUD_H
