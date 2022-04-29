#ifndef QTTILEUTIL_H
#define QTTILEUTIL_H

#include "QtTileFile.h"
#include "QtTileField.h"
#include "QtTileHeader.h"
#include "QtPointFields.h"
#include "lasconv_global.h"

class LASCONV_EXPORT QtTileUtil
{
public:
  static QtPointFields ConvertFields(const QtTileFields &fields);
  static QtTileFields  ConvertFields(const QtPointFields &fields);
  static QString       GenerateTempFilePath(const QString &prefix);
};

#endif // QTTILEUTIL_H
