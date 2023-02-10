
#include <iostream>
#include <cstdlib>

#ifndef IS_STD
#include "map.hpp"
using namespace ft;
#else
#include <map>
using namespace std;
#endif

template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, pair<T1, T2> &p) {
  return os << '(' << p.first << ", " << p.second << ')';
}

void map_insert() {
  {
    map<int, int> m;
    typedef pair<map<int, int>::iterator, bool> res_t;

    res_t res = m.insert(make_pair(10, 4));
    std::cout << *(res.first) << '\n';
    res = m.insert(make_pair(11, 4));
    std::cout << *(res.first) << '\n';
  }
  {
    typedef map<int, std::string> mapis;
    mapis m;
    typedef pair<mapis::iterator, bool> res_t;
    res_t res = m.insert(make_pair(20, "fioes"));
    std::cout << *(res.first) << '\n';
    res = m.insert(make_pair(21, "fioes"));
    std::cout << *(res.first) << '\n';
  }
  {
    typedef map<int, int> map;
    map m;
    typedef pair<map::iterator, bool> res_t;
    typedef map::iterator it_t;

    for (int i = 0; i < 9; i++)
    {
      res_t res = m.insert(make_pair(rand(), i));
      std::cout << *(res.first) << '\n';
    }
    for (it_t i(m.begin()); i != m.end(); ++i)
    {
      std::cout << i.ptr << ' ' << *i << '\n';
    }
  }
}
