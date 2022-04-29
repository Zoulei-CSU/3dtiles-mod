#ifndef QTTEMPINDEXFILE_H
#define QTTEMPINDEXFILE_H

#include <QObject>
#include <QFile>
#include "QtPoint3D.hpp"

class LASCONV_EXPORT QtTempIndexFile : public QObject
{
  Q_OBJECT
public:
  explicit QtTempIndexFile(QObject *parent = nullptr);
  ~QtTempIndexFile();

  QString LastError() const;
  QString FileName() const;

  bool IsOpen() const;
  bool AtEnd() const;
  bool Open(const QString &strFile, bool bCreate = true);
  bool Write(const QtPointIndexes &indexes);
  bool Read(QtPointIndexes &indexes, int nMax = 1000000);
  bool Seek(qint64 point);
  bool SeekAndRead(qint64 point, QtPointIndex &index);
  bool Flush();
  void Close(bool bRemove = true);

private:
  bool RaiseError(const QString &strError);

private:
  QFile      m_file;
  QString    m_strLastError;
  int        m_nIndexSize;
  int        m_nBufferedCount;
  QByteArray m_arrOutputBuffer;
};

#endif // QTTEMPINDEXFILE_H
