#ifndef QTTILEWRITER_H
#define QTTILEWRITER_H

#include <QObject>
#include "QtTileFile.h"
#include "QtTempPointFile.h"
#include "QtProgressTask.h"

class LASCONV_EXPORT QtTileWriter : public QObject
{
  Q_OBJECT
public:
  explicit QtTileWriter(QObject *parent = nullptr);
  virtual ~QtTileWriter();

  bool    IsOpen();
  QString LastError() const;

  QtTileFields WritingFields() const;
  QtTileHeader WritingHeader() const;

  ///
  /// \brief Open 打开写入；
  /// \param file 输出的分块文件路径；
  /// \param fields 点数据使用的字段；
  /// \param header 初始文件头，建议至少提供scale/offset值，根据需要设置坐标系等值，其他值自动填充；
  /// \return 返回是否打开成功
  ///
  bool Open(const QString &file, const QtTileFields &fields, const QtTileHeader &header = QtTileHeader());

  ///
  /// \brief Write 将点数据写入缓冲；
  /// \param points 待写入的点数据，需保证所有点的字段与打开文件时的字段参数一致；
  /// \return 返回是否写入成功；
  ///
  bool Write(const QtPointEx3DVec &points);

  ///
  /// \brief CloseAndCancel 关闭并取消写入到分块文件；
  /// \return 固定返回true；
  ///
  bool CloseAndCancel();

  ///
  /// \brief CloseAndFlush 关闭并将缓冲数据写入到分块文件；
  /// \param task 进度更新任务；
  /// \param nStartProgress 起始进度，控制函数执行过程在进度区间中的起始值；
  /// \param nEndProgress 结束进度，控制函数执行过程在进度区间中的结束值；如果结束值小于等于起始值，函数过程不更新进度，只检查撤销标记；
  /// \return 返回是否写入分块文件成功，不管成功与否，文件都会关闭（IsOpen返回false）；
  ///
  bool CloseAndFlush(QtProgressTask::Ptr task, int nStartProgress = 0, int nEndProgress = 100);

private:
  bool WasCanceled() const;
  bool RaiseError(const QString &strError);
  void UpdateProgress(int nPercentage);
  void UpdateProgress(const QString &strMessage);

  static QString TempFileOf(const QString &strFile);

private:
  QtProgressTask::Ptr m_ptrProgressTask;

  QString         m_strLastError;
  QString         m_strFilePath;
  QtTileFile      m_clsTileFile;
  QtTempPointFile m_clsTempPointFile;

  QtTileFields m_clsWritingFields;
  QtTileHeader m_clsWritingHeader;
  QtPoint3D    m_clsMinPoint;
  QtPoint3D    m_clsMaxPoint;
  qint64       m_nNumOfPoint;
};

#endif // QTTILEWRITER_H
