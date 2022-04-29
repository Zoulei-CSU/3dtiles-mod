#ifndef QTPROGRESSCONTROLER_H
#define QTPROGRESSCONTROLER_H

#include <QObject>
#include <QMutex>
#include "lasconv_global.h"
#include "QtProgressTask.h"

class LASCONV_EXPORT QtProgressControler : public QObject
{
  Q_OBJECT
signals:
  void StateChanged(bool busy);
  void TaskRaised(QtProgressTask::Ptr task);

public:
  explicit QtProgressControler(QObject *parent = nullptr);

  bool IsBusy() const;  // 是否繁忙，即是否有运行中的任务（包括持续任务）
  bool IsIdle() const;  // 是否空闲，即是否可发起新的任务（包括持续任务）

  QtProgressTask::Ptr RaiseTask(bool bCancelable = true);   // 发起任务，若当前任务不为空，返回nullptr
  QtProgressTask::Ptr CurrentTask() const;                  // 当前任务，当任务状态不为running时，重置为nullptr

  QtProgressTask::Ptr RaiseContinuousTask(bool bCancelable = true); // 发起持续性任务（区别于普通任务，可用于操作互斥），若当前存在同类型任务，返回nullptr
  QtProgressTask::Ptr CurrentContinuousTask() const;                // 当前持续性任务，当前持续性任务状态不为running时，重置为nullptr

private slots:
  void OnTaskDone(int nID);
  void OnTaskBroken(int nID);
  void OnTaskCanceled(int nID);
  void OnTaskAboutToDestroy(int nID);

private:
  static int CreateTaskID();

  void ReleaseTask(int nID);
  QtProgressTask::Ptr RaiseTaskPrivate(bool bContinuous, bool bCancelable);

private:
  QMutex              m_mtxTaskLock;
  int                 m_nCurrentID;
  QtProgressTask::Ptr m_ptrCurrentTask;
  int                 m_nCurrentContID;
  QtProgressTask::Ptr m_ptrCurrentContTask;
};

#endif // QTPROGRESSCONTROLER_H
