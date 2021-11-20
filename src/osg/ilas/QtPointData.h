#ifndef QTPOINTDATA_H
#define QTPOINTDATA_H

#include <QSharedPointer>
#include <QVariant>
#include "QtPointFields.h"
#include "lasconv_global.h"

///
/// \brief 点字段数据
///
struct LASCONV_EXPORT QtPointData
{
public:
  QtPointData(const QtPointFields &fields);
  QtPointData(const QtPointData &other);
  ~QtPointData();

  QtPointData& operator=(const QtPointData &other);

  const QtPointFields& Fields() const;

  int         DataSize() const;
  int         FieldCount() const;
  QStringList FieldNames() const;
  int         IndexOf(const QString &name);

  QVariant Value(int i) const;
  QVariant Value(const QString &name) const;

  bool SetValue(int i, const QVariant &var);
  bool SetValue(const QString &name, const QVariant &var);

  uchar* ReadRawData(uchar *data);
  uchar* WriteRawData(uchar *data) const;

private:
  uchar        *m_buffer;
  QtPointFields m_fields;
};

#endif // QTPOINTDATA_H
