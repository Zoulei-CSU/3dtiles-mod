#ifndef QTTILEHEADER_H
#define QTTILEHEADER_H

#include <QString>
#include "QtPointQuantizer.hpp"
#include "lasconv_global.h"

#define TILE_HEADER_ELEVATION_MIN (-20000)
#define TILE_HEADER_ELEVATION_MAX  (19999)
#define TILE_HEADER_RETURN_SIZE    (32)
#define TILE_HEADER_CLASS_SIZE     (256)
#define TILE_HEADER_INTENSITY_SIZE (80000)
#define TILE_HEADER_ELEVATION_SIZE (40000)
#define TILE_HEADER_EXTDATA_SIZE   (100)

#pragma pack(push)
#pragma pack(1)

struct LASCONV_EXPORT QtTileHeader
{
public:
  enum CRS
  {
    CRS_None   = 0,  // 无坐标参考系
    CRS_EPSG   = 1,  // EPSG编码（uint32）
    CRS_PROJ4  = 2,  // PROJ4（string）
    CRS_WKT    = 3,  // WKT（string）
  };

  enum ExtFlag
  {
    EF_AvgElevation  = 0,  // 高度平均值，double
    EF_StdElevation  = 1,  // 高度标准差，double
    EF_AvgIntensity  = 2,  // 强度平均值，double
    EF_StdIntensity  = 3,  // 强度标准差，double
    EF_MinGpstime    = 4,  // GPS时间最小值，double
    EF_MaxGpstime    = 5,  // GPS时间最大值，double
    EF_CustomElevMin = 6,  // 自定义高程统计最小值，int
    EF_CompressXYZ_X = 7,  // 坐标值压缩参数X，int
    EF_CompressXYZ_Y = 8,  // 坐标值压缩参数X，int
    EF_CompressXYZ_Z = 9,  // 坐标值压缩参数X，int
  };

  struct ExtData
  {
    qint8 m_nFlag;  // 扩展数据标记
    union           // 扩展数据值
    {
      double  m_dbDoubleValue;
      float   m_fFloatValue;
      qint8   m_nInt8Value;
      qint16  m_nInt16Value;
      qint32  m_nInt32Value;
      qint64  m_nInt64Value;
      quint8  m_nUint8Value;
      quint16 m_nUint16Value;
      quint32 m_nUInt32Value;
      quint64 m_nUInt64Value;
      bool    m_bBoolValue;

      float   m_arrFloatNums[2];

      qint8   m_arrInt8Nums[8];
      quint8  m_arrUint8Nums[8];

      qint16  m_arrInt16Nums[4];
      quint16 m_arrUint16Nums[4];

      qint16  m_arrInt32Nums[2];
      quint16 m_arrUint32Nums[2];
    };
  };

public:
  char    m_strSignature[32];  // 格式签名
  quint8  m_nMajorVersion;     // 主版本号
  quint8  m_nMinorVersion;     // 次版本号


  double  m_dbXScale;   // X缩放系数
  double  m_dbYScale;   // Y缩放系数
  double  m_dbZScale;   // Z缩放系数
  double  m_dbXOffset;  // X偏移值
  double  m_dbYOffset;  // Y偏移值
  double  m_dbZOffset;  // Z偏移值

  double  m_dbMinX;   // X最小值
  double  m_dbMinY;   // Y最小值
  double  m_dbMinZ;   // Z最小值
  double  m_dbMaxX;   // X最大值
  double  m_dbMaxY;   // Y最大值
  double  m_dbMaxZ;   // Z最大值

  quint8  m_nCRSFormat;         // 坐标系格式
  quint8  m_arrnCRSData[1024];  // 坐标系数据

  quint64 m_nNumOfPoints;  // 点总数
  quint32 m_nNumOfFields;  // 字段总数
  quint32 m_nNumOfBlocks;  // 分块总数

  quint32 m_nNumOfExtData;       // 扩展数据数量
  ExtData m_arrclsExtData[100];  // 扩展数据数组

  // 统计数据
  qint64 *m_arrnNumOfPointsByReturn/*[32]*/;       // 回波点数统计
  qint64 *m_arrnNumOfPointsByClass/*[256]*/;       // 分类点数统计
  qint64 *m_arrnNumOfPointsByIntensity/*[80000]*/; // 强度点数统计
  qint64 *m_arrnNumOfPointsByElevation/*[40000]*/; // 高程点数统计-20000至19999

public:
  QtTileHeader();
  QtTileHeader(const QtTileHeader &other);
  ~QtTileHeader();

  QtTileHeader& operator=(const QtTileHeader &other);

  static qint64 HeaderSize();
  static int    ElevationRangeMin();
  static int    ElevationRangeMax();

  static bool Read(QtTileHeader &header, const QByteArray &buffer);
  static void Write(const QtTileHeader &header, QByteArray &buffer);

  quint32 GetCRSAsEPSG() const;
  QString GetCRSAsProj4() const;
  QString GetCRSAsWKT() const;

  QtPointQuantizer CreateQuantizer() const;

  void AddNumOfPointsByReturn(int nReturn);
  void AddNumOfPointsByClass(int nClass);
  void AddNumOfPointsByIntensity(int nIntensity);
  void AddNumOfPointsByElevation(int nElevation, int offset = TILE_HEADER_ELEVATION_MIN);

  void ResetNumOfPointsByReturn();
  void ResetNumOfPointsByClass();
  void ResetNumOfPointsByIntensity();
  void ResetNumOfPointsByElevation();

  void SetCRSAsNone();
  void SetCRSAsEPSG(const quint32 nEPSG);
  void SetCRSAsProj4(const QString &strProj4);
  void SetCRSAsWKT(const QString &strWKT);

  bool AddExtData(const ExtData &data);
  bool GetExtData(int i, ExtData &data) const;
  int  FirstExtDataOf(qint8 nFlag) const;
  int  GetOrCreateExtData(qint8 nFlag);
  int  AddOrUpdateExtData(const ExtData &data);
  void ResetExtData();

private:
  void Init();
};

#pragma pack(pop)

#endif // QTTILEHEADER_H
