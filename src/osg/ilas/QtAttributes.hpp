#ifndef QTATTRIBUTES_H
#define QTATTRIBUTES_H

#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QSet>
#include <functional>

template <class T>
struct QtAttributes
{
public:
  typedef std::function<void(const QString&, const T&)> Callback;

public:
  QStringList Attributes() const
  {
    return m_mapKeyToVal.keys();
  }

  void Clear()
  {
    if(m_callback == nullptr)
    {
      m_mapKeyToVal.clear();
    }
    else
    {
      foreach(const QString &key, Attributes())
        DelAttribute(key);
    }
  }

  inline bool HasAttribute(const QString &key) const
  {
    return m_mapKeyToVal.contains(key);
  }

  inline void DelAttribute(const QString &key)
  {
    if(!HasAttribute(key))
      return;
    m_mapKeyToVal.remove(key);
    if(m_callback != nullptr)
      m_callback(key, T());
  }

  inline T Attribute(const QString &key, const T &def = T()) const
  {
    if(!HasAttribute(key))
      return def;
    return m_mapKeyToVal.value(key, def);
  }

  inline void SetAttribute(const QString &key, const T &val)
  {
    if(m_mapKeyToVal.value(key) == val)
      return;
    m_mapKeyToVal.insert(key, val);
    if(m_callback != nullptr)
      m_callback(key, val);
  }

  inline void RegisterCallback(Callback cb)
  {
    m_callback = cb;
  }

  QtAttributes<T>& operator=(const QtAttributes<T> &other)
  {
    if(&other == this)
      return *this;

    if(m_callback == nullptr)
    {
      this->m_mapKeyToVal = other.m_mapKeyToVal;
    }
    else
    {
      QSet<QString> keys = QSet<QString>::fromList(Attributes());
      QMap<QString, T>::const_iterator cur = other.m_mapKeyToVal.cbegin();
      QMap<QString, T>::const_iterator end = other.m_mapKeyToVal.cend();
      while(cur != end)
      {
        SetAttribute(cur.key(), cur.value());
        keys.remove(cur.key());
        ++cur;
      }

      foreach(const QString &key, keys)
        DelAttribute(key);
    }

    return *this;
  }

private:
  Callback         m_callback;
  QMap<QString, T> m_mapKeyToVal;
};

typedef QtAttributes<QString>  QtStrAttributes;
typedef QtAttributes<QVariant> QtVarAttributes;

#endif // QTATTRIBUTES_H
