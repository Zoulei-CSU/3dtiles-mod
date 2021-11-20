#ifndef QTPOINTTEMPLATE_H
#define QTPOINTTEMPLATE_H

#include <QVector.h>
#include "QtPointFields.h"
#include "QtPointData.h"

template<typename T>
struct QtPoint3DTemplate
{
public:
  QtPoint3DTemplate(T x=0, T y=0, T z=0) : m_x(x), m_y(y), m_z(z) {}

  T X() const { return m_x; }
  T Y() const { return m_y; }
  T Z() const { return m_z; }

  T& XRef() { return m_x; }
  T& YRef() { return m_y; }
  T& ZRef() { return m_z; }

  T& operator[](int i) { return m_p[i]; }
  const T operator[](int i) const { return m_p[i]; }

private:
  union
  {
    struct
    {
      T m_x;
      T m_y;
      T m_z;
    };
    T m_p[3];
  };
}; // struct QtPointTemplate

template<typename T>
struct QtPointEx3DTemplate : public QtPoint3DTemplate<T>
{
public:
  QtPointEx3DTemplate(T x=0, T y=0, T z=0, const QtPointFields &fields = QtPointFields()) : QtPoint3DTemplate(x, y, z), m_data(fields) {}

  QtPointData* Data() const { return const_cast<QtPointData*>(&m_data); }
  int          DataSize() const { return m_data.DataSize() + 3*sizeof(T); }

  uchar* ReadRawData(uchar *data)
  {
    m_x = *((T*)(data));
    m_y = *((T*)(data+sizeof(T)));
    m_z = *((T*)(data+2*sizeof(T)));
    return m_data.ReadRawData(data+3*sizeof(T));
  }

  uchar* WriteRawData(uchar *data) const
  {
    memcpy(data, &m_x, sizeof(T));
    memcpy(data+sizeof(T), &m_y, sizeof(T));
    memcpy(data+2*sizeof(T), &m_z, sizeof(T));
    return m_data.WriteRawData(data+3*sizeof(T));
  }

private:
  QtPointData m_data;
};

typedef QtPoint3DTemplate<int>   QtPoint3D;
typedef QVector<QtPoint3D>       QtPoint3DVec;
Q_DECLARE_METATYPE(QtPoint3D)
Q_DECLARE_METATYPE(QtPoint3DVec)

typedef QtPointEx3DTemplate<int> QtPointEx3D;
typedef QVector<QtPointEx3D>     QtPointEx3DVec;
Q_DECLARE_METATYPE(QtPointEx3D)
Q_DECLARE_METATYPE(QtPointEx3DVec)

typedef quint64                  QtPointIndex; //高32位是块索引，低32位是块内索引
typedef QVector<QtPointIndex>    QtPointIndexes;
Q_DECLARE_METATYPE(QtPointIndex)
Q_DECLARE_METATYPE(QtPointIndexes)

typedef QtPoint3DTemplate<double> QtPoint3Dd;
typedef QVector<QtPoint3Dd>       QtPoint3DdVec;
Q_DECLARE_METATYPE(QtPoint3Dd)
Q_DECLARE_METATYPE(QtPoint3DdVec)

typedef QtPointEx3DTemplate<double> QtPointEx3Dd;
typedef QVector<QtPointEx3Dd>       QtPointEx3DdVec;
Q_DECLARE_METATYPE(QtPointEx3Dd)
Q_DECLARE_METATYPE(QtPointEx3DdVec)

#define INVALID_POINT_INDEX QtPointIndex(LLONG_MAX)

#endif // QTPOINTTEMPLATE_H
