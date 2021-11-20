#ifndef QTPOINTEX3DXYZCOMPRESSOR_H
#define QTPOINTEX3DXYZCOMPRESSOR_H

#include "QtPointCompressor.h"

class LASCONV_EXPORT QtPointEx3DXYZCompressor : public QtPointCompressor
{
  Q_OBJECT
public:
  explicit QtPointEx3DXYZCompressor(QObject *parent = nullptr);
  virtual ~QtPointEx3DXYZCompressor();

  bool Parallel() const;
  void SetParallel(bool bEnabled);

  QtPoint3D     CompressionParams() const;
  QtPointFields CompressionFields() const;

  virtual void ResetParams();
  virtual void Init(const QtPoint3D &min, const QtPoint3D &max, const QtPointFields &fields);
  virtual void Init(const QtPoint3D &params, const QtPointFields &fields);

  /* 判断是否已初始化或是否可压缩 */
  virtual bool Compressed() const override;

  /* 预估的压缩后的点数据大小（字节） */
  virtual int PredictedSize() const override;
  virtual int PredictedSize(const QtPointEx3D &pt) const override;

  /* 单点压缩，返回压缩后的大小，压缩失败或未实现则返回值小于等于0 */
  virtual int Read(QtPointEx3D &pt, const char *data) override;
  virtual int Write(const QtPointEx3D &pt, char *data) override;

  /* 点数组压缩，返回压缩后的大小，压缩失败或未实现则返回值小于等于0 */
  virtual int Read(QtPointEx3DVec &ptv, const char *data, const int size) override;
  virtual int Write(const QtPointEx3DVec &ptv, char *data) override;

private:
  bool m_bReadParallel;
  bool m_bCompressed;
  bool m_bCompressX;
  bool m_bCompressY;
  bool m_bCompressZ;
  int  m_nRedundant;

  QtPoint3D     m_ptParams;
  QtPointFields m_vecFields;

  const static qint16 s_nPadding;
};

#endif // QTPOINTEX3DXYZCOMPRESSOR_H
