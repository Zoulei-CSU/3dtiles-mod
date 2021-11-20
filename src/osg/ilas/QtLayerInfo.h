#ifndef QTLAYERINFO_H
#define QTLAYERINFO_H

#include <QObject>
#include "QtAttributes.hpp"
#include "lasconv_global.h"

class QtLayer;

class LASCONV_EXPORT QtLayerInfo : public QObject
{
  Q_OBJECT
signals:
  void AttrChanged(const QString &strKey, const QString &strVal);

protected:
  explicit QtLayerInfo(QtLayer *pLayer, const QtStrAttributes &attrs = QtStrAttributes());
  virtual ~QtLayerInfo() {}

  void SetAttr(const QString &strKey, const QString &strVal);

public:
  QtLayer*    Layer() const;
  QStringList Attrs() const;

  QString Attr(const QString &strKey);

  QtStrAttributes Extract() const;

private:
  QtStrAttributes m_attrs;
  QtLayer        *m_pLayer;

  friend class QtLayer;
};

#endif // QTLAYERINFO_H
