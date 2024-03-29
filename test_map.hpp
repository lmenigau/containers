#include <iterator>
#include "map.hpp"
#include "vector.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <typeinfo>
#include <vector>

#include "print_dot.hpp"

template <class T1, class T2>
std::ostream& operator<<(std::ostream& os, const ft::pair<T1, T2>& p) {
  return os << '(' << p.first << ", " << p.second << ')';
}

template <class C>
void safe_insert(C& c, typename C::value_type val) {
  try {
    c.insert(val);
  } catch (int) {
    bst_print_dot(c.get_rep().get_root());
    throw;
  }
}

template <>
void safe_insert<std::map<int, int> >(std::map<int, int>& c,
                                      std::map<int, int>::value_type val) {
  try {
    c.insert(val);
  } catch (int) {
    throw;
  }
}

template <class C>
void fill_random(C& c, long size) {
  std::cout << __FILE__ << ":" << __LINE__ << " in " << __PRETTY_FUNCTION__;
  for (long i = 0; i < size; i++) {
    typename C::key_type a = random() % size;
    safe_insert(c, typename C::value_type(a, i));
  }
}

template <class T1, class T2>
std::string toString(const ft::pair<T1, T2>& pair) {
  std::ostringstream str;
  str << "<" << pair.first << ", " << pair.second << ">";
  return str.str();
}

template <class C>
void pair_print(C& pair) {
  std::cout << "<" << pair.first << ", " << pair.second << ">\n";
}

template <class K, class V>
void print(std::map<K, V>& c) {
  std::for_each(c.begin(), c.end(),
                pair_print<typename std::map<K, V>::value_type>);
}

template <class K, class V>
void print(ft::map<K, V>& c) {
  std::for_each(c.begin(), c.end(),
                pair_print<typename ft::map<K, V>::value_type>);
}

template <class C>
void rev_print(C& c) {
  std::for_each(c.rbegin(), c.rend(), pair_print<C>);
}

struct find_map {
  template <class Map>
  void operator()(Map& m, long size) {
    typedef typename Map::iterator Iterator;
    typedef typename Map::key_type Key;

    fill_random(m, size);
    for (long i = 0; i < size; i++) {
      Key k(random() % size);
      Iterator it(m.find(k));
      if (it != m.end())
        std::cout << (it->first == k) << '\n';
      else
        std::cout << "end\n";
    }
  }
};

template <typename>
struct traits {};

template <class K, class V>
struct traits<std::map<K, V> > {
  typedef std::pair<typename std::map<K, V>::iterator, bool> res_t;
};

template <class K, class V>
struct traits<ft::map<K, V> > {
  typedef ft::pair<typename ft::map<K, V>::iterator, bool> res_t;
};

template <class K, class V>
void print_map(std::map<K, V>& m, int n) {}
template <class K, class V>
void print_map(ft::map<K, V>& m, int n) {
  bst_print_dot(m.get_rep().get_root(), n);
}

template <class C, class V>
void fill_vector(C& c, V& v, long size) {
  /* std::cout << __FILE__ << ":" << __LINE__ << " in " << __PRETTY_FUNCTION__;
   */
  for (long i = 0; i < size; i++) {
    typename C::key_type a = v[i];
    safe_insert(c, typename C::value_type(a, i));
  }
}

struct range_construct {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    fill_vector(m, v, size);
    Map copy(m.rbegin(), m.rend());
    for (long i = size; i > 0; i--)
      copy.erase(i);
    // print(copy);
    m = copy;
  }
};

struct swap {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    print(m);
    Map t;
    fill_vector(m, v, size);
    t.swap(m);
    print(m);
  }
};

struct copy_construct {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    fill_vector(m, v, size);
    Map copy(m);
    m.clear();
    m = copy;
  }
};

struct map_insert_hint {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    typename Map::iterator pos = m.begin();
    for (long i = 0; i < size; i++) {
      typename Map::value_type a(v[i], i);
      pos = m.insert(pos, a);
      print_map(m, i);
    }
  }
};

struct map_insert {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    {
      typedef typename traits<Map>::res_t res_t;
      res_t res = m.insert(typename Map::value_type(10, 4));
      res = m.insert(typename Map::value_type(11, 4));
    }
    fill_vector(m, v, size);
  }
};

struct range_erase {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    fill_vector(m, v, size);
    typename Map::iterator f = m.begin();
    typename Map::iterator l = m.end();
    std::advance(f, 3);
    std::advance(l, -2);
    m.erase(f, l);
  }
};

struct map_erase {
  template <class Map>
  void operator()(Map& m, ft::vector<int>& v, long size) {
    /* fill_random(m, size); */
    fill_vector(m, v, size);
    /* std::copy(v.begin(), v.end(), std::inserter(m, m.end())); */
    /* print(m); */
    m.insert(typename Map::value_type(100, 3));
    m.insert(typename Map::value_type(99, 3));
    // m.erase(100);
    m.erase((--m.end())->first);
    m.erase((--m.end())->first);
    for (long i = 0; i < size; i += 2)
      m.erase(i);
    m.insert(typename Map::value_type(25, 3));
    m.erase(25);
    m.insert(typename Map::value_type(-3, 3));
    m.erase(-3);
    m.insert(typename Map::value_type(13, 3));
    m.erase(-3);
  }
};
