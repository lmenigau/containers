
#include <iostream>
#ifndef NS
#define NS ft
#endif

#include <map>

#include "map.hpp"

using namespace NS;

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, pair<T1, T2> &p) {
  return os << '(' << p.first << ", " << p.second << ')';
}

void map_insert() {
  map<int, int> m;
  typedef pair<map<int, int>::iterator, bool> res_t;

  res_t res = m.insert(make_pair(10, 4));
  std::cout << *(res.first);
}
