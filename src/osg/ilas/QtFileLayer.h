#ifndef QTFILELAYER_H
#define QTFILELAYER_H

#include <QColor>
#include "QtLayer.h"

///
/// \brief 文件类图层基类
///
class LASCONV_EXPORT QtFileLayer : public QtLayer
{
  Q_OBJECT
public:
  DEFINE_LAYER_INFO_KEY(KEY_FILE_PATH, "file_path")
  DEFINE_LAYER_INFO_KEY(KEY_FILE_ICONNAME, "file_iconname")
  DEFINE_LAYER_INFO_KEY(KEY_FILE_COLOR, "file_color")
  DEFINE_LAYER_INFO_KEY(KEY_FILE_SIZE, "file_size")
  DEFINE_LAYER_INFO_KEY(KEY_FILE_TYPE, "file_type")
  DEFINE_LAYER_INFO_KEY(KEY_FILE_NODEMODE, "file_nodemode")

public:
  explicit QtFileLayer(LayerTypeFlag type, QObject *parent = nullptr);
  virtual ~QtFileLayer();

  /* 初始化接口 */
  virtual bool Initialize() = 0;
  virtual bool InitFromAttributes(const QtStrAttributes &attrs) = 0;

  /* 图层属性 */
  QString FileName() const;

  QString FilePath() const;
  void    SetFilePath(const QString &strPath);

  QString IconName() const;
  void    SetIconName(const QString &strPath);

  QColor  FileColor() const;
  void    SetFileColor(const QColor color);

  int     FileSize() const;
  void    SetFileSize(int nSize);

  int     FileNodeMode() const;
  void    SetFileNodeMode(int nNodeMode);

  int     FileType() const;
  void    SetFileType(int nType);

protected:
  static QString ExtractFormatFromPath(const QString &strPath);
};

#endif // QTFILELAYER_H
