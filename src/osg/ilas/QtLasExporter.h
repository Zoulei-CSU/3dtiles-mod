#ifndef QTLASEXPORTER_H
#define QTLASEXPORTER_H

#include <QThread>
#include "QtProgressTask.h"
#include "lasconv_global.h"
///
/// \brief 内部点云格式的LAS转换器
///
class LASCONV_EXPORT QtLasExporter : public QThread
{
  Q_OBJECT
signals:
  void Finished(bool bResult, QString strFile, QString strMessage);

public:
  explicit QtLasExporter(bool bRunInThread, QObject *parent = nullptr);
  virtual ~QtLasExporter();

  bool    IsRunning() const;
  bool    Result() const;
  QString LastError() const;

  bool Convert(const QString &strTileFile, const QString &strLasFile, QtProgressTask::Ptr progress);

protected:
  virtual void run() override;

private:
  void SetRunningResult(bool bResult, QtProgressTask::Ptr progress,
                        const QString &strFile,
                        const QString &strMessage = QString());

  void ConvertPrivate(const QString &strTileFile, const QString &strLasFile, QtProgressTask::Ptr progress);

  bool CheckCancel(QtProgressTask::Ptr &progress);
  void UpdateProgress(QtProgressTask::Ptr &progress, int nPercentage);
  void UpdateProgress(QtProgressTask::Ptr &progress, const QString &strMessage);

private:
  bool    m_bRunInThread;
  bool    m_bRunningFlag;
  bool    m_bCanceledFlag;
  bool    m_bRunningResult;
  QString m_strLastError;

  QString m_strLasFile;
  QString m_strTileFile;
  QtProgressTask::Ptr m_ptrProgress;
};

#endif // QTLASEXPORTER_H
