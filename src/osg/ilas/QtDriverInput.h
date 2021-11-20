
#ifndef QTDRIVERINPUT_H
#define QTDRIVERINPUT_H

#include "QtLayer.h"

#include "QtAttributes.hpp"
#include "lasconv_global.h"

///
/// \brief 驱动输入，保存生成图层的参数，不同的驱动应实现不同的参数接口
///
struct LASCONV_EXPORT QtDriverInput
{
public:
  QtDriverInput();

  QtLayer::LayerTypeFlag LayerType() const;
  void SetLayerType(QtLayer::LayerTypeFlag type);

  QString LayerFormat() const;
  void SetLayerFormat(const QString &format);

  // 参数获取/参数设置
  // 子类应提供明确意义的参数读写接口，并内部调用此读写接口
  QString Attribute(const QString &strKey) const;
  void SetAttribute(const QString &strKey, const QString &strVal);

  // 提取成通用属性，作为驱动实质的输入
  QtStrAttributes Extract() const;

private:
  QtStrAttributes m_attrs;
};

#endif // QTDRIVERINPUT_H
