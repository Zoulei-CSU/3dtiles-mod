#ifndef QTDRIVER_H
#define QTDRIVER_H

#include <QObject>
#include <QStringList>
#include "QtLayer.h"
#include "QtAttributes.hpp"
#include "lasconv_global.h"

///
/// \brief 图层数据驱动基类
///
class LASCONV_EXPORT QtDriver : public QObject
{
  Q_OBJECT
public:
  explicit QtDriver(QObject *parent = nullptr);

  QString LastError() const;

  /* 获取/设置支持的格式 */
  QStringList SupportedFormats() const;
  bool        Supported(const QString &strFormat) const;

  /* 创建驱动对应的图层，注意：当前驱动实例不可与创建的图层实例有直接关联 */
  virtual QtLayer* Create(const QtStrAttributes &attrs) = 0;

protected:
  bool RaiseError(const QString &strError);
  void Support(const QStringList &lstFormats);

private:
  QString     m_strLastError;
  QStringList m_lstSupportFormats;
};

#endif // QTDRIVER_H
