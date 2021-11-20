#ifndef QTTEMPBASEFILE_H
#define QTTEMPBASEFILE_H

#include <QObject>
#include <QFile>
#include "lasconv_global.h"

class LASCONV_EXPORT QtTempBaseFile : public QObject
{
  Q_OBJECT
public:
  enum DataTypeFlag
  {
    DTF_Unknown,
    DTF_Double,
    DTF_Float,
    DTF_Int8,
    DTF_UInt8,
    DTF_Int16,
    DTF_UInt16,
    DTF_Int32,
    DTF_Int64,
    DTF_UInt32,
    DTF_UInt64,
  };

public:
  explicit QtTempBaseFile(QObject *parent = nullptr);
  ~QtTempBaseFile();

  DataTypeFlag DataType() const;
  QString      LastError() const;
  QString      FileName() const;
  qint64       WrittenCount() const;

  bool IsOpen() const;
  bool AtEnd() const;
  bool Open(DataTypeFlag type, const QString &strFile = QString(), bool bCreate = true);
  bool Reopen(DataTypeFlag type, const QString &strFile = QString(), bool bCreate = true);
  bool Write(void *data, int nNum = 1);
  int  Read(void *data, int nMax = 1000000);
  bool Seek(qint64 index);
  bool Flush();
  void Close(bool bRemove = true);

  static int SizeOfType(DataTypeFlag type);

private:
  bool RaiseError(const QString &strError);

private:
  QFile        m_file;
  QString      m_strLastError;
  DataTypeFlag m_eDataType;
  qint64       m_nWritten;
  int          m_nDataSize;
  int          m_nBufferedCount;
  QByteArray   m_arrOutputBuffer;
};

#endif // QTTEMPBASEFILE_H
