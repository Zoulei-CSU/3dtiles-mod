#ifndef QTLASPOINTCONVERTER_H
#define QTLASPOINTCONVERTER_H

#include <QVector>
#include <QPair>
#include "QtPointFields.h"
#include "QtPointCloud.h"

class LASpoint;

///
/// \brief LAS点数据结构与本地点数据结构的转换器
///
struct LASCONV_EXPORT QtLasPointConverter
{
public:
  enum FieldFlag
  {
    FF_Intensity,
    FF_RGB,
    FF_Class,
    FF_GPSTime,

    FF_ReturnNum,
    FF_NumOfReturn,

    FF_ScanDirectionFlag,
    FF_EdgeOfFlightLine,
    FF_SyntheticFlag,
    FF_KeypointFlag,
    FF_WithheldFlag,

    FF_ScanAngleRank,
    FF_UserData,
    FF_PointSourceID,
  };

public:
  QtLasPointConverter(const QtPointFields &fields);

  void ConvertToLasPoint(LASpoint &laspoint, const QtPointEx3D &pt);
  void ConvertFromLasPoint(const LASpoint &laspoint, QtPointEx3D &pt);

private:
  using FieldPair = QPair<int,int>;
  QVector<FieldPair> m_vecIndexToFlag;
};

#endif // QTLASPOINTCONVERTER_H
