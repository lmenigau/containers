
#include <cstdlib>
#include <ctime>
#include <iostream>

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

    srandom(time(0));
    for (int i = 0; i < 1; i++) {
      res_t res(m.insert(make_pair(random(), i)));
      std::cout << *(res.first) << ' ' << res.second << '\n';
    }
    std::cout << "--------------\n";
    it_t i(m.begin());
    it_t l(m.end());
    for (; i != l; ++i) {
      std::cout << *i << '\n';
    }
    it_t b(m.end());
    l = m.begin();
    b--;
    for (i = b; i != l; --i) {
      std::cout << *i << '\n';
    }
    std::cout << *i << '\n';
  }
}
