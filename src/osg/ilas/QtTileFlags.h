#ifndef QTTILEFLAGS_H
#define QTTILEFLAGS_H

#include <QtGlobal>
#include <QByteArray>
#include "lasconv_global.h"

struct LASCONV_EXPORT QtTileFlags
{
public:
  QtTileFlags(int nCapacity = 0);
  QtTileFlags(const QtTileFlags &other);
  ~QtTileFlags();

  QtTileFlags& operator=(const QtTileFlags &other);

  char* Buffer() const;
  int   BufferSize() const;

  bool AllTrue(int count = -1) const;
  bool AllFalse(int count = -1) const;

  int NumOfTrue() const;
  int NumOfFalse() const;

  bool Get(int i) const;
  void Set(int i, bool flag);
  void Fill(bool flag);
  void Fill(bool flag, int i, int size = -1);

  static int SizeOfCapacity(int nCapacity);

private:
  int        m_nCapacity;
  QByteArray m_arrBuffer;
};

#endif // QTTILEFLAGS_H
