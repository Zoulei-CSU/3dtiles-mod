#ifndef QTTILEDRIVER_H
#define QTTILEDRIVER_H

#include "QtPointCloudDriver.h"

///
/// \brief 内部点云格式驱动
///
class LASCONV_EXPORT QtTileDriver : public QtPointCloudDriver
{
  Q_OBJECT
public:
  explicit QtTileDriver(QObject *parent = nullptr);

  /* 创建驱动对应的图层，注意：当前驱动实例不可与创建的图层实例有直接关联 */
  virtual QtLayer* Create(const QtStrAttributes &attrs) override;

  /* 点云专有接口 */
  virtual QtOctree* CreateOctree(const QString &file, bool bOnlyAttributes, QtProgressTask::Ptr task) override;
};

#endif // QTTILEDRIVER_H
