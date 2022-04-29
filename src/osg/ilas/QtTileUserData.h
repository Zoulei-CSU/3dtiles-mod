#ifndef QTTILEUSERDATA_H
#define QTTILEUSERDATA_H

#include <QVector>
#include "QtPointData.h"
#include "lasconv_global.h"

#pragma pack(push)
#pragma pack(1)

struct LASCONV_EXPORT QtTileUserData
{
public:
  explicit QtTileUserData(const QtPointFields &fields = QtPointFields());
  explicit QtTileUserData(const QtPointData &data);

  QtPointData& Data();
  const
  QtPointData& Data() const;

  bool   IsValid() const;
  qint64 RawDataSize() const;
  bool   WriteRawData(QByteArray &data) const;
  bool   ReadRawData(const QByteArray &data);

private:
  QtPointData m_data;
};

#pragma pack(pop)

#endif // QTTILEUSERDATA_H
