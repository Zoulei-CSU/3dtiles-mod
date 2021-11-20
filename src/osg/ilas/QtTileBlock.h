#ifndef QTTILEBLOCK_H
#define QTTILEBLOCK_H

#include <QVector>
#include "lasconv_global.h"

#pragma pack(push)
#pragma pack(1)

struct LASCONV_EXPORT QtTileBlock
{
public:
  quint32 m_nIndex;     // 块索引
  quint32 m_nSize;      // 块大小
  quint64 m_nOffset;    // 块偏移

public:
  QtTileBlock();
};

#pragma pack(pop)

typedef QVector<QtTileBlock> QtTileBlocks;

QtTileBlocks& SortTileBlocks(QtTileBlocks &blocks);

#endif // QTTILEBLOCK_H
