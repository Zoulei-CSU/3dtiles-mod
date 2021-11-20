#ifndef QTLASSUMMARY_H
#define QTLASSUMMARY_H

#include <QObject>
#include "QtOctree.h"
#include "QtAttributes.hpp"

class LASsummary;

class QtLasSummary : public QObject
{
  Q_OBJECT
public:
  explicit QtLasSummary(QtOctree *tree = nullptr);
  virtual ~QtLasSummary();

  void Bind(QtOctree *tree);

  LASsummary& Summary();
  const LASsummary& Summary() const;

  QVariant Attribute(const QString &key) const;
  void SetAttribute(const QString &key, const QVariant &val);

private:
  LASsummary *m_sum;
  QtVarAttributes m_attrs;
};

#endif // QTLASSUMMARY_H
