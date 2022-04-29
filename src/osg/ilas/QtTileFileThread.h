#ifndef QTTILEFILETHREAD_H
#define QTTILEFILETHREAD_H

#include <QThread>
#include <QQueue>
#include <QMutex>
#include "QtTileFile.h"

class LASCONV_EXPORT QtTileFileThread : public QThread
{
  Q_OBJECT
public:
  // 开始分块操作参数
  struct BeginTilingParams
  {
  public:
    QString             m_path;     // 文件路径
    QtTileHeader        m_header;   // 文件头信息
    QtTileFields        m_fields;   // 文件字段信息
    QtProgressTask::Ptr m_progress; // 进度控制任务

  public:
    BeginTilingParams() : m_progress(nullptr) {}
  };

  // 结束分块操作参数
  struct EndTilingParams
  {
  public:
    bool m_bForce; // 是否强制结束

  public:
    EndTilingParams(bool bForce = false) : m_bForce(bForce) {}
  };

  // 开始索引操作参数
  struct BeginIndexingParams
  {
  public:
  public:
    BeginIndexingParams() {}
  };

  // 结束索引操作参数
  struct EndIndexingParams
  {
  public:
    qint64 m_nMergeingThreshold;  // 块合并阈值（点数）

  public:
    EndIndexingParams() : m_nMergeingThreshold(-1) {}
  };

  // 开始填充操作参数
  struct BeginFillingParams
  {
  public:
    int m_nBufferSize;                       // 缓冲区大小（字节）
    QtTileFile::StatisticsOptions m_options; // 统计选项

  public:
    BeginFillingParams() : m_nBufferSize(10000) {}
  };

  // 结束填充操作参数
  struct EndFillingParams
  {
  public:
  public:
    EndFillingParams() {}
  };

signals:
  void ErrorOccured(const QString strMessage);

public:
  explicit QtTileFileThread(QObject *parent = nullptr);

  int  FillingQueue() const;   // 填充数据队列大小
  int  IndexingQueue() const;  // 索引数据队列大小

  void BeginTiling(const BeginTilingParams &params);                              // 开始分块

  void BeginIndexing(const BeginIndexingParams &params = BeginIndexingParams());  // 开始索引
  bool DoIndexing(const QtPoint3D &point);                                        // 输入索引数据
  bool DoIndexing(const QtPoint3DVec &points);                                    // 输入索引数据
  bool EndIndexing(const EndIndexingParams &params = EndIndexingParams());        // 结束索引

  bool BeginFilling(const BeginFillingParams &params = BeginFillingParams());     // 开始填充
  bool DoFilling(const QtPointEx3D &point);                                       // 输入填充数据
  bool DoFilling(const QtPointEx3DVec &points);                                   // 输入填充数据
  bool EndFilling(const EndFillingParams &params = EndFillingParams());           // 结束填充

  bool EndTiling(const EndTilingParams &params = EndTilingParams());              // 结束分块

protected:
  virtual void run() override;

private:
  // 操作标记
  enum OperationFlag
  {
    OF_BeginTiling,
    OF_BeginIndexing,
    OF_DoIndexing,
    OF_EndIndexing,
    OF_BeginFilling,
    OF_DoFilling,
    OF_EndFilling,
    OF_EndTiling,
  };

  void RaiseErrorIfNotCanceled();
  void FillRawDataBuffer(const QtPointEx3DVec &points);

private:
  bool        m_bTerminated;
  QtTileFile  m_clsTileFile;

  QMutex      m_mtxOperLock;
  QQueue<int> m_queOperations;

  BeginTilingParams   m_clsBeginTilingParams;
  EndTilingParams     m_clsEndTilingParams;
  BeginIndexingParams m_clsBeginIndexingParams;
  EndIndexingParams   m_clsEndIndexingParams;
  BeginFillingParams  m_clsBeginFillingParams;
  EndFillingParams    m_clsEndFillingParams;

  QMutex m_mtxIndexingLock;
  QtPoint3DVec m_vecIndexingBuffer;

  QMutex m_mtxFillingLock;
  int    m_nFillingCount;
  QByteArray m_arrFillingBuffer;

  QtProgressTask::Ptr m_ptrTilingProgress;
};

#endif // QTTILEFILETHREAD_H
