#ifndef QTPOINTCLOUDDRIVERINPUT_H
#define QTPOINTCLOUDDRIVERINPUT_H

#include "QtPointCloud.h"
#include "QtFileDriverInput.h"


///
/// \brief 点云图层驱动输入参数基类
///
struct LASCONV_EXPORT QtPointCloudDriverInput : public QtFileDriverInput
{
public:
  QtPointCloudDriverInput();
  // 用户应设置图层路径
};

#endif // QTPOINTCLOUDDRIVERINPUT_H
