#include <map>
#include "map.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

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
  for (long i = 0; i < size; i++) {
    int a = random() % size;
    std::cout << i << "\n";
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
void pair_print(typename C::value_type& pair) {
  std::cout << "<" << pair.first << ", " << pair.second << ">\n";
}

template <class C>
void print(C& c) {
  std::for_each(c.begin(), c.end(), pair_print<C>);
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
    print(m);
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
void print_map(std::map<K, V>& m) {}
template <class K, class V>
void print_map(ft::map<K, V>& m) {
  bst_print_dot(m.get_rep().get_root(), 0);
}

struct map_insert {
  template <class Map>
  void operator()(Map& m, long size) {
    {
      // typedef typename P::template pair<typename Map::iterator, bool> res_t;

      typedef typename traits<Map>::res_t res_t;
      res_t res = m.insert(typename Map::value_type(10, 4));
      pair_print<Map>(*(res.first));
      res = m.insert(typename Map::value_type(11, 4));
      pair_print<Map>(*(res.first));
    }
    {
      Map m;
      typedef typename traits<Map>::res_t res_t;
      res_t res =
          m.insert(typename Map::value_type(20, typename Map::mapped_type()));
      pair_print<Map>(*(res.first));
      res = m.insert(typename Map::value_type(21, typename Map::mapped_type()));
      pair_print<Map>(*(res.first));
    }
    {
      std::bind2nd(std::modulus<int>(), 2);
      Map m;
      // typedef pair<map::iterator, bool> res_t;
      fill_random(m, size);
      std::cout << "====forward=====\n";
      print(m);
      std::cout << "====backward=====\n";
      rev_print(m);
      print_map(m);
      // bst_print_dot(m.get_rep().get_root(), 0);
    }
  }
};
