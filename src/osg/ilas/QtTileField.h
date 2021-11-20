#ifndef QTTILEFIELD_H
#define QTTILEFIELD_H

#include <QVector>
#include "lasconv_global.h"

#pragma pack(push)
#pragma pack(1)

struct LASCONV_EXPORT QtTileField
{
public:
  char    m_strFieldName[32]; // 字段名称
  quint8  m_nFieldType;       // 字段类型
  quint32 m_nFieldSize;       // 字段大小

public:
  QtTileField();
};

#pragma pack(pop)

typedef QVector<QtTileField> QtTileFields;

#endif // QTTILEFIELD_H
