#ifndef QTPROGRESSTASK_H
#define QTPROGRESSTASK_H

#include <QObject>
#include <QMutex>
#include <QSharedPointer>
#include "lasconv_global.h"

class QtProgressControler;

class LASCONV_EXPORT QtProgressTask : public QObject
{
  Q_OBJECT
signals:
  void Canceled(int nID);                               // 任务取消信号，任务状态变为已取消（Canceled）时发出
  void CancelHung(int nID);                             // 取消挂起信号，调用取消接口但当前为不可取消任务时发出
  void CancelCleared(int nID);                          // 取消清除信号，当被挂起的取消信号被清除时发出
  void CancelableChanged(int nID, bool bCancelable);    // 可取消状态信号，当可取消状态被改变时发出
  void ProgressBroken(int nID, QString strMessage);     // 任务中断信号，任务状态变为已中断（Broken）时发出
  void ProgressDone(int nID, QString strMessage);       // 任务完成信号，任务状态变为已完成（Done）时发出
  void ProgressUpdated(int nID, int nPercent);          // 进度更新信号，进度百分比被更新时发出
  void MessageUpdated(int nID, QString strMessage);     // 信息更新信号，进度信息被更新时发出
  void StateChanged(int nID, int nState);               // 状态变化信号，状态变化时发出，且优先于其他信号发出
  void AboutToDestroy(int nID);                         // 销毁信号，实例被销毁（delete）时发出

public:
  enum TaskStateFlag
  {
    State_Running,  // 正在运行，初始状态
    State_Done,     // 正常完成
    State_Broken,   // 运行中断/出错
    State_Canceled, // 用户取消
  };

  typedef QSharedPointer<QtProgressTask> Ptr;

protected:
  // 进度任务由进度管理器创建并分配有效id
  explicit QtProgressTask(int nID, bool bContinuous = false, bool bCancelable = true, QObject *parent = nullptr);

public:
  virtual ~QtProgressTask();

  int  TaskID() const;                     // 任务唯一ID
  bool IsContinuous() const;               // 是否持续任务

  bool IsCancelable() const;               // 任务是否可取消
  void SetCancelable(bool bCancelable);    // 设置可取消属性

  bool HasHangingCancel() const;           // 是否有挂起的取消信号
  void ClearHangingCancel();               // 清除挂起的取消信号

  void Update(const QString &strMessage);  // 更新进度信息
  void Update(int nPercent);               // 更新进度百分比
  void Update(int nCurrent, int nTotal);   // 更新进度，自动计算百分比

  void Done(const QString &strMessage);    // 正常完成，重置状态为done
  void Break(const QString &strMessage);   // 运行中断，重置状态为broken
  void Cancel();                           // 用户取消，重置状态为canceled

  int  TaskState() const;                  // 任务状态
  bool IsRunning() const;                  // 是否正在运行（running）
  bool WasDone() const;                    // 是否正常完成（done）
  bool WasBroken() const;                  // 是否运行中断（broken）
  bool WasCanceled() const;                // 是否已被取消（canceled）

  QString CurrentMessage() const;          // 当前的进度信息
  int     CurrentProgress() const;         // 当前的进度百分比

private:
  bool UpdateTaskState(int nState);

private:
  int     m_nTaskID;
  int     m_nTaskState;
  bool    m_bContinuous;
  bool    m_bCancelable;
  bool    m_bHangingCancel;
  int     m_nCurrentProgress;
  QString m_strCurrentMessage;
  QMutex  m_mtxStateLock;

  friend class QtProgressControler;
};

Q_DECLARE_METATYPE(QtProgressTask::Ptr)

#endif // QTPROGRESSTASK_H
