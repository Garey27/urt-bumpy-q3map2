
#if !defined(INCLUDED_CONTAINER_CACHE_H)
#define INCLUDED_CONTAINER_CACHE_H

#include <cstddef>
#include "container/hashtable.h"
#include "memory/allocator.h"

template<typename Type, typename Parameter>
class DefaultCreationPolicy
{
public:
  Type* construct(const Parameter& parameter)
  {
    return New<Type>().scalar(parameter);
  }
  void destroy(Type* p)
  {
    Delete<Type>().scalar(p);
  }
};

template<typename Type>
class SharedValue
{
  typedef Type value_type;
  typedef value_type* pointer;
  typedef value_type& reference;

  std::size_t m_count;
  pointer m_value;

public:
  SharedValue()
    : m_count(0), m_value(0)
  {
  }
  ~SharedValue()
  {
    ASSERT_MESSAGE(m_count == 0 , "destroying a referenced object\n");
  }
  void set(pointer value)
  {
    m_value = value;
  }
  pointer get()
  {
    return m_value;
  }
  std::size_t increment()
  {
    return ++m_count;
  }
  std::size_t decrement()
  {
    ASSERT_MESSAGE(!empty(), "destroying a non-existent object\n");
    return --m_count;
  }
  std::size_t count()
  {
    return m_count;
  }
  bool empty()
  {
    return m_count == 0;
  }
  reference operator*() const
  {
    ASSERT_NOTNULL(m_value);
    return *m_value;
  }
  pointer operator->() const
  {
    return &(operator*());
  }
};



/// \brief Caches values that are uniquely identified by a key.
///
/// - Automatically removes objects that are no longer referenced.
///
/// \param Key Uniquely identifies each element.
/// \param Cached The type to be cached. Must define a constructor that accepts \c Key.
template<typename Key, typename Cached, typename Hasher, typename KeyEqual = std::equal_to<Key>, typename CreationPolicy = DefaultCreationPolicy<Cached, Key> >
class HashedCache : public CreationPolicy
{
  typedef SharedValue<Cached> Element;
  typedef HashTable<Key, Element, Hasher, KeyEqual> map_type;

  map_type m_map;

public:
  explicit HashedCache(const CreationPolicy& creation = CreationPolicy())
    : CreationPolicy(creation), m_map(256)
  {
  }
  ~HashedCache()
  {
    ASSERT_MESSAGE(empty(), "HashedCache::~HashedCache: not empty");
  }

  typedef typename map_type::iterator iterator;
  typedef typename map_type::value_type value_type;

  iterator begin()
  {
    return m_map.begin();
  }
  iterator end()
  {
    return m_map.end();
  }

  bool empty() const
  {
    return m_map.empty();
  }

  iterator find(const Key& key)
  {
    return m_map.find(key);
  }

  void capture(iterator i)
  {
    (*i).value.increment();
  }
  void release(iterator i)
  {
    if((*i).value.decrement() == 0)
    {
      CreationPolicy::destroy((*i).value.get());
      m_map.erase(i);
    }
  }

#if 1
  Element& capture(const Key& key)
  {
    Element& elem = m_map[key];
    if(elem.increment() == 1)
    {
      elem.set(CreationPolicy::construct(key));
    }
    return elem;
  }
#else
  value_type& capture(const Key& key)
  {
    iterator i = m_map.find(key);
    if(i == m_map.end())
    {
      Element element;
      element.set(CreationPolicy::construct(key));
      i = m_map.insert(key, element);
    }
    (*i).value.increment();
    return (*i);
  }
#endif
  void release(const Key& key)
  {
    iterator i = m_map.find(key);
    ASSERT_MESSAGE(i != m_map.end(), "releasing a non-existent object\n");
    release(i);
  }

  void clear()
  {
    m_map.clear();
  }
};


#endif
