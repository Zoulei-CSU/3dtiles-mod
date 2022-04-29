#ifndef QTTILEFILEVECTOR_H
#define QTTILEFILEVECTOR_H

#include "QtPointCloudVector.h"
#include "QtTileFile.h"

///
/// \brief 分块文件的vector风格读取接口封装，内部以只读方式打开文件
///
class LASCONV_EXPORT QtTileFileVector : public QtPointCloudVector
{
  Q_OBJECT
public:
  explicit QtTileFileVector(const QString &strFile, QObject *parent = nullptr);
  ~QtTileFileVector();

  bool        IsOpen() const;    // 文件是否已打开成功
  QString     FileName() const;  // 文件名（路径）
  QtTileFile *TileFile() const;  // 分块文件实例，本实例析构时自动关闭文件并析构文件实例，注意：不可对文件进行关闭或写入操作；

private:
  static QtOctree* InitTileFile(const QString &strFile);

private:
  QtTileFile *m_pTileFile;
};

#endif // QTTILEFILEVECTOR_H
