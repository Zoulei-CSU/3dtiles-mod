#ifndef QTTEMPPOINTFILE_H
#define QTTEMPPOINTFILE_H

#include <QObject>
#include <QFile>
#include "QtPoint3D.hpp"

class LASCONV_EXPORT QtTempPointFile : public QObject
{
  Q_OBJECT
public:
  explicit QtTempPointFile(QObject *parent = nullptr);
  ~QtTempPointFile();

  QString LastError() const;
  QString FileName() const;

  bool IsOpen() const;
  bool AtEnd() const;
  bool Open(const QString &strFile, const QtPointFields &fields, bool bCreate = true);
  bool Write(const QtPointEx3DVec &points);
  bool Read(QtPointEx3DVec &points, int nMax = 1000000);
  int  Read(QByteArray &points, int nMax = 1000000);
  bool Peek(QtPointEx3DVec &points, int nMax = 1000000);
  int  Peek(QByteArray &points, int nMax = 1000000);
  bool Seek(qint64 point);
  bool Flush();
  void Close(bool bRemove = true);

private:
  bool RaiseError(const QString &strError);

private:
  QFile         m_file;
  QtPointFields m_fields;

  QString    m_strLastError;
  int        m_nPointSize;
  int        m_nBufferedCount;
  QByteArray m_arrOutputBuffer;
};

#endif // QTTEMPPOINTFILE_H
