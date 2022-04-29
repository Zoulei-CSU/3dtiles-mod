#ifndef QTPOINTFIELD_H
#define QTPOINTFIELD_H

#include <functional>
#include <QPair>
#include <QMap>
#include <QSharedPointer>
#include "lasconv_global.h"

///
/// \brief 点数据字段信息
///
struct LASCONV_EXPORT QtPointField
{
public:
  enum TypeFlag
  {
    Type_Invalid,
    Type_Bool,    // bool
    Type_Bit,     // bool
    Type_Byte,    // uchar
    Type_Short,   // short
    Type_UShort,  // ushort
    Type_Int32,   // qint32
    Type_Int64,   // qint64
    Type_UInt32,  // quint32
    Type_UInt64,  // quint64
    Type_Float,   // float
    Type_Double,  // double
    Type_String,  // QString
    Type_Binary,  // QByteArray
  };

  typedef std::function<void(int)> Callback;

public:
  QtPointField(TypeFlag type = Type_Invalid, const QString &name = QString(), int size = 0);

  bool operator==(const QtPointField &other) const;
  bool operator!=(const QtPointField &other) const;
  
  bool IsValid() const;

  TypeFlag Type() const;
  void SetType(TypeFlag type);

  QString Name() const;
  void SetName(const QString &name);

  int Size() const;
  void SetSize(int size);

  int BitSize() const;
  int ByteSize() const;

  void RegisterCallback(void *host, Callback cb, int mark);
  void UnregisterCallback(void *host);

  static int BitSizeOf(TypeFlag type);
  static int ByteSizeOf(TypeFlag type, int size);

private:
  void InvokeCallbacks();

private:
  TypeFlag m_type;
  QString  m_name;
  int      m_size;
  int      m_bitsize;
  int      m_bytesize;

  typedef QPair<Callback,int>       CallbackPack;
  typedef QMap<void*, CallbackPack> CallbackBinding;
  CallbackBinding m_callbacks;
};

Q_DECLARE_METATYPE(QtPointField)
Q_DECLARE_METATYPE(uchar)
Q_DECLARE_METATYPE(short)
Q_DECLARE_METATYPE(ushort)

#endif // QTPOINTFIELD_H
