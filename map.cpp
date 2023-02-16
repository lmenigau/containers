
#include <signal.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "debug.hpp"

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

map<int, int> m;
void handler(int sig) { throw sig; };

void map_insert() {
  alarm(1);
  signal(SIGALRM, handler);
  {
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
    // typedef pair<map::iterator, bool> res_t;
    typedef map::iterator it_t;

    srandom(std::time(0));
    for (int i = 0; i < 128; i++) {
      map::value_type val(random() % 128, i);
      std::cout << val << std::endl;
      try {
        m.insert(val);
      } catch (int) {
#ifndef IS_STD
        bst_print_dot(m.get_rep().get_root(), stderr);
#endif
        exit(1);
      }
      // res_t res(m.insert(val));
      // std::cout << *res.first << ' ' << res.second << '\n';
    }
#ifndef IS_STD
    bst_print_dot(m.get_rep().get_root(), stderr);
#endif
    std::cout << "--------------\n";
    it_t i(m.begin());
    it_t l(m.end());
    for (; i != l; ++i) {
      std::cout << *i << '\n';
    }
    // it_t b(m.end());
    // l = m.begin();
    // b--;
    // for (i = b; i != l; --i) {
    //   std::cout << *i << '\n';
    // }
    // std::cout << *i << '\n';
  }
}
