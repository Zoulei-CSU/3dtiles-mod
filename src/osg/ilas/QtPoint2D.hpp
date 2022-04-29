#ifndef QTPOINT2D_HPP
#define QTPOINT2D_HPP

#include <QMetaType>

template<typename T>
struct QtPoint2DTemplate
{
public:
  QtPoint2DTemplate(T x=0, T y=0) : m_x(x), m_y(y) {}

  T X() const { return m_x; }
  T Y() const { return m_y; }

  T& XRef() { return m_x; }
  T& YRef() { return m_y; }

  T& operator[](int i) { return m_p[i]; }
  const T operator[](int i) const { return m_p[i]; }

private:
  union
  {
    struct
    {
      T m_x;
      T m_y;
    };
    T m_p[2];
  };
};

typedef QtPoint2DTemplate<int> QtPoint2D;
typedef QVector<QtPoint2D>     QtPoint2DVec;
Q_DECLARE_METATYPE(QtPoint2D)
Q_DECLARE_METATYPE(QtPoint2DVec)

typedef QtPoint2DTemplate<double> QtPoint2Dd;
typedef QVector<QtPoint2Dd>       QtPoint2DdVec;
Q_DECLARE_METATYPE(QtPoint2Dd)
Q_DECLARE_METATYPE(QtPoint2DdVec)

#endif // QTPOINT2D_HPP
