#ifndef QTFILEDRIVERINPUT_H
#define QTFILEDRIVERINPUT_H

#include "QtFileLayer.h"
#include "QtDriverInput.h"


///
/// \brief 文件驱动输入，增加文件路径接口，对应文件图层基类
///
class LASCONV_EXPORT QtFileDriverInput : public QtDriverInput
{
public:
  explicit QtFileDriverInput();

  QString FilePath() const;
  void SetFilePath(const QString &path);
};

#endif // QTFILEDRIVERINPUT_H
