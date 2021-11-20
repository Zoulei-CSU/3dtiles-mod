#ifndef QTLASTILER_H
#define QTLASTILER_H

#include <QThread>
#include "QtProgressTask.h"
#include "QtTileFileThread.h"
#include "lasconv_global.h"

class LASreader;

///
/// \brief LAS点云格式与内部点云格式的转换器
///
class LASCONV_EXPORT QtLasTiler : public QThread
{
  Q_OBJECT
signals:
  void Started(QString strFile);
  void Finished(bool bResult, QString strFile, QString strMessage);

public:
  explicit QtLasTiler(bool bRunInThread, QObject *parent = nullptr); // 初始化，输入是否在线程执行，如果为否，则转换调用同步执行，否则异步执行
  virtual ~QtLasTiler();

  bool    IsRunning() const;  // 是否正在运行
  bool    Result() const;     // 运行结果
  QString LastError() const;  // 上一次运行错误

  bool Convert(const QString &strLasFile, QtProgressTask::Ptr progress); // 执行转换，输入las/laz文件路径，输出文件与源文件在同一路径，后缀不同

protected:
  virtual void run() override;

private:
  void SetRunningOutput(const QString &strOutput);
  void SetRunningResult(bool bResult, QtProgressTask::Ptr progress, const QString &strMessage = QString());

  void ConvertPrivate(const QString &strLasFile, QtProgressTask::Ptr progress);

  bool CheckCancel(QtProgressTask::Ptr &progress);
  void UpdateProgress(QtProgressTask::Ptr &progress, int nPercentage);
  void UpdateProgress(QtProgressTask::Ptr &progress, const QString &strMessage);

  void PrintElapsed(QTime &time, const QString &message = QString());
  void DeterminateFields(QtPointFields &fields, LASreader &reader);
  void Summarizing(LASreader &reader, QtTileHeader &header, QtProgressTask::Ptr &progress);
  void Indexing(LASreader &reader, QtProgressTask::Ptr &progress);
  void Filling(LASreader &reader, const QtPointFields &fields, QtProgressTask::Ptr &progress);

private:
  bool    m_bRunInThread;
  bool    m_bRunningFlag;
  bool    m_bCanceledFlag;
  bool    m_bRunningResult;
  QString m_strRunningOutput;
  QString m_strLastError;

  QString m_strLasFile;
  QtProgressTask::Ptr m_ptrProgress;
  QtTileFileThread    m_tileFileThread;
};

#endif // QTLASTILER_H
