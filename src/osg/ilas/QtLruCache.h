#ifndef QTLRUCACHE_H
#define QTLRUCACHE_H

#include <QMap>
#include <QList>
#include <QPair>

template<class key_type, class value_type>
class QtLruCache
{
  public:
    typedef QSet<key_type> map_type;
    typedef QVector<QPair<key_type,value_type *>> list_type;
    QtLruCache(size_t capacity):m_capacity(capacity)
    {
      m_list.reserve(capacity);
    }

    ~QtLruCache(void)
    {
      clear();
    }

    size_t size(void) const
    {
      return m_list.size();
    }

    size_t capacity(void) const
    {
      return m_capacity;
    }

    bool isEmpty(void) const
    {
      return m_list.isEmpty();
    }
    bool contains(const key_type &key)
    {
      if (m_map.find(key)!=m_map.end())
      {
        return true;
      }
      return false;
    }
    void insert(const key_type &key,value_type *pValue)
    {
      if (contains(key))
      {
        return;
      }
      if (m_list.size()>=m_capacity)
      {
        evict();
      }
      m_list.push_front(qMakePair(key,pValue));
    }
    value_type *get(const key_type &key)
    {
      for (long i=0; i<m_list.size(); ++i)
      {
        QPair<key_type,value_type *> &ele(m_list[i]);
        if (ele.first==key)
        {
          if (i)
          {
            for (long j=(i-1); j>=0; --j)
            {
              m_list[j+1] = m_list[j];
            }
            m_list[0] =ele;
          }
          return ele.second;
        }
      }
      return 0;
    }

    void clear(void)
    {
      for (long i=0; i<m_list.size(); ++i)
      {
        delete m_list[i].second;
      }
      m_list.clear();
    }

private:
    void evict(void)
    {
      long idx = m_list.size()-1;
      //
      QPair<key_type,value_type *> &ele(m_list[idx]);
      map_type::iterator iterKey =m_map.find(ele.first);
      if (iterKey!=m_map.end())
      {
        m_map.erase(iterKey);
      }
      delete ele.second;
      //
      m_list.resize(idx);
    }
    map_type m_map;
    list_type m_list;
    size_t m_capacity;
};

#endif // QTLRUCACHE_H
