#ifndef QTVEC3_HPP
#define QTVEC3_HPP

#include <QtMath>

template<typename T>
struct QtVec3Template
{
public:
  QtVec3Template(T x = 0, T y = 0, T z = 0) : m_x(x), m_y(y), m_z(z) {}

  T X() const { return m_x; }
  T Y() const { return m_y; }
  T Z() const { return m_z; }

  T& XRef() { return m_x; }
  T& YRef() { return m_y; }
  T& ZRef() { return m_z; }

  T& operator[](int i) { return m_p[i]; }
  const T operator[](int i) const { return m_p[i]; }

  inline T Length() const
  {
    return qSqrt(m_x * m_x + m_y * m_y + m_z * m_z);
  }

  inline QtVec3Template Normalized() const
  {
    T l = length();
    return Vec3(m_x / l, m_y / l, m_z / l);
  }

  inline void operator+=(const QtVec3Template &v)
  {
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
  }

  inline QtVec3Template operator/(const T &a) const
  {
    return QtVec3Template(m_x / a, m_y / a, m_z / a);
  }

  inline QtVec3Template operator-(const QtVec3Template &v) const
  {
    return QtVec3Template(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
  }

  inline QtVec3Template operator+(const QtVec3Template &v) const
  {
    return QtVec3Template(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
  }

  inline QtVec3Template operator*(const T &a) const
  {
    return QtVec3Template(m_x * a, m_y * a, m_z * a);
  }

  inline QtVec3Template operator-() const
  {
    return QtVec3Template(-m_x, -m_y, -m_z);
  }

  inline QtVec3Template Cross(const QtVec3Template &v) const
  {
    return QtVec3Template(m_y * v.m_z - m_z * v.m_y,
                  m_z * v.m_x - m_x * v.m_z,
                  m_x * v.m_y - m_y * v.m_x);
  }

  inline double Dot(const QtVec3Template &v) const
  {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
  }

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
};

typedef QtVec3Template<double> QtVec3;

#endif // QTVEC3_HPP
