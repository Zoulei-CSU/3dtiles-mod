#ifndef QTPOINTCLOUDDRIVER_H
#define QTPOINTCLOUDDRIVER_H

#include <QVariant>
#include "QtDriver.h"
#include "QtPoint3D.hpp"
#include "QtPointCloud.h"
#include "lasconv_global.h"

///
/// \brief 点云图层驱动基类
///
class LASCONV_EXPORT QtPointCloudDriver : public QtDriver
{
  Q_OBJECT
public:
  explicit QtPointCloudDriver(QObject *parent = nullptr);

  /* 创建驱动对应的图层，注意：当前驱动实例不可与创建的图层实例有直接关联 */
  virtual QtLayer* Create(const QtStrAttributes &attrs) = 0;

  /* 点云专有接口 */
  virtual QtOctree* CreateOctree(const QString &file, bool bOnlyAttributes, QtProgressTask::Ptr task) = 0;
};

#endif // QTPOINTCLOUDDRIVER_H
