#ifndef QTPOINTFIELDS_HPP
#define QTPOINTFIELDS_HPP

#include <QStringList>
#include <QVector>
#include <QSharedPointer>
#include <QAtomicInt>
#include "QtPointField.h"
#include "lasconv_global.h"

///
/// \brief 点数据字段信息，内部实现写时复制，通常一份点云数据共享一个字段信息
///
struct LASCONV_EXPORT QtPointFields
{
public:
  QtPointFields();
  QtPointFields(const QtPointFields &other);
  QtPointFields(const QList<QtPointField> &fields);
  QtPointFields& operator=(const QtPointFields &other);
  bool operator==(const QtPointFields &other) const;
  bool operator!=(const QtPointFields &other) const;
  ~QtPointFields();

  QtPointField At(int i) const;

  QtPointField& operator[](int i);
  QtPointField  operator[](int i) const;

  int  Count() const;      // 字段数量
  uint DataSize() const;   // 字段数据大小

  QStringList Names() const;               // 字段名列表
  int TypeOf(int i) const;                 // 根据索引，查询字段类型
  int TypeOf(const QString &name) const;   // 根据名称，查询字段类型
  int IndexOf(const QString &name) const;  // 根据名称，查询字段索引

  void Add(const QtPointField &field);     // 添加字段
  void Remove(int i);                      // 移除字段

  QtPointField Get(int i) const;
  void Set(int i, const QtPointField &field);

  bool DataOffset(int i, int &bitoffset, int &byteoffset) const; // 根据索引，计算字段数据偏移

private:
  struct Impl
  {
  public:
    Impl(const Impl &other);
    Impl(const QList<QtPointField> &fields = QList<QtPointField>());
    ~Impl();

    void Ref();    // 引用，引用计数加一
    void Unref();  // 解引用，引用计数减一

    QtPointField& operator[](int i);
    const
    QtPointField& operator[](int i) const;

    int  Count() const;
    uint DataSize() const;

    QStringList Names() const;
    int TypeOf(int i) const;
    int TypeOf(const QString &name) const;
    int IndexOf(const QString &name) const;

    void Add(const QtPointField &field);
    void Remove(int i);

    QtPointField Get(int i) const;
    void Set(int i, const QtPointField &field);

    bool DataOffset(int i, int &bitoffset, int &byteoffset);

  private:
    void UpdateIndices();
    void UpdateOffsets(int from = 0);
    void UpdateCallbacks(int from = 0);

  private:
    QList<QtPointField>    m_fields;
    QVector<int>           m_bitoffsets;
    QVector<int>           m_byteoffsets;
    uint                   m_datasize;
    QtPointField::Callback m_callback;
    QMap<QString, int>     m_indices;
    QAtomicInt             m_referenced;
  };

  void CopyOnWrite();

private:
  bool  m_copy;
  Impl* m_impl;
};

Q_DECLARE_METATYPE(QtPointFields)

#endif // QTPOINTFIELDS_HPP
