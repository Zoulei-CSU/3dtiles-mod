#ifndef QTPOINTCOMPRESSOR_H
#define QTPOINTCOMPRESSOR_H

#include <QObject>
#include "QtPoint3D.hpp"
#include "lasconv_global.h"

#define POINT_COMPRESSOR_TYPE_INVALID  (0)
#define POINT_COMPRESSOR_TYPE_EX3D_XYZ (1)

///
/// \brief 点数据压缩器基类接口
///
class LASCONV_EXPORT QtPointCompressor : public QObject
{
  Q_OBJECT
public:
  explicit QtPointCompressor(int nType = POINT_COMPRESSOR_TYPE_INVALID, QObject *parent = nullptr);
  virtual ~QtPointCompressor();

  int CompressorType() const;

  /* 判断是否已初始化或是否可压缩, 子类必须重载该接口 */
  virtual bool Compressed() const;

  /* 预估的压缩后的点数据大小（字节）, 子类至少应重载一种点类型接口 */
  virtual int PredictedSize() const;
  virtual int PredictedSize(const QtPointEx3D &pt) const;
  virtual int PredictedSize(const QtPoint3D &pt) const;
  virtual int PredictedSize(const QtPointEx3Dd &pt) const;
  virtual int PredictedSize(const QtPoint3Dd &pt) const;

  /* 单点压缩，返回压缩后的大小，压缩失败或未实现则返回值小于等于0 */
  virtual int Read(QtPointEx3D &pt, const char *data);
  virtual int Write(const QtPointEx3D &pt, char *data);

  virtual int Read(QtPoint3D &pt, const char *data);
  virtual int Write(const QtPoint3D &pt, char *data);

  virtual int Read(QtPointEx3Dd &pt, const char *data);
  virtual int Write(const QtPointEx3Dd &pt, char *data);

  virtual int Read(QtPoint3Dd &pt, const char *data);
  virtual int Write(const QtPoint3Dd &pt, char *data);

  /* 点数组压缩，返回压缩后的大小，压缩失败或未实现则返回值小于等于0 */
  virtual int Read(QtPointEx3DVec &ptv, const char *data, const int size);
  virtual int Write(const QtPointEx3DVec &ptv, char *data);

  virtual int Read(QtPoint3DVec &ptv, const char *data, const int size);
  virtual int Write(const QtPoint3DVec &ptv, char *data);

  virtual int Read(QtPointEx3DdVec &ptv, const char *data, const int size);
  virtual int Write(const QtPointEx3DdVec &ptv, char *data);

  virtual int Read(QtPoint3DdVec &ptv, const char *data, const int size);
  virtual int Write(const QtPoint3DdVec &ptv, char *data);

private:
  int Unimplemented() const;

private:
  int m_nCompressorType;
};

#endif // QTPOINTCOMPRESSOR_H
