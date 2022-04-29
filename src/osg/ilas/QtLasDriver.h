#ifndef QTLASDRIVER_H
#define QTLASDRIVER_H

#include "QtPointCloudDriver.h"
#include "lasconv_global.h"

///
/// \brief LAS点云数据驱动
///
class LASCONV_EXPORT QtLasDriver : public QtPointCloudDriver
{
  Q_OBJECT
public:
  explicit QtLasDriver(QObject *parent = nullptr);
  virtual ~QtLasDriver();

  /* 创建驱动对应的图层，注意：当前驱动实例不可与创建的图层实例有直接关联 */
  virtual QtLayer* Create(const QtStrAttributes &attrs) override;

  /* 点云专有接口 */
  virtual QtOctree* CreateOctree(const QString &file, bool bOnlyAttributes, QtProgressTask::Ptr task) override;

protected:
  bool IsOpen() const;
  bool Open(const QString &file);
  void Close();
  bool ReadPoints(const QtPointIndexes &indexes, QtPointEx3DVec &points);

private:
  static QtPointFields DefaultFields();

  void *m_reader;
};

#endif // QTLASDRIVER_H
