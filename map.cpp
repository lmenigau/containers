#include <signal.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "test.hpp"
#include "test_map.hpp"
#if IS_STD
namespace ft = std;
#include <map>
#define bst_print_dot(...)
#else
#include "map.hpp"
#include "print_dot.hpp"
#endif

ft::map<int, int> m;
void handler(int sig) { throw sig; };

void map_insert(long size) {
  alarm(1);
  signal(SIGALRM, handler);
  {
    typedef ft::pair<ft::map<int, int>::iterator, bool> res_t;

    res_t res = m.insert(ft::make_pair(10, 4));
    std::cout << *(res.first) << '\n';
    res = m.insert(ft::make_pair(11, 4));
    std::cout << *(res.first) << '\n';
  }
  {
    typedef ft::map<int, std::string> mapis;
    mapis m;
    typedef ft::pair<mapis::iterator, bool> res_t;
    res_t res = m.insert(ft::make_pair(20, "fioes"));
    std::cout << *(res.first) << '\n';
    res = m.insert(ft::make_pair(21, "fioes"));
    std::cout << *(res.first) << '\n';
  }
  {
    typedef ft::map<int, int> map;
    map m;
    // typedef pair<map::iterator, bool> res_t;
    typedef map::iterator it_t;

    for (int i = 0; i < size; i++) {
      map::value_type val(random() % size, i);
      std::cout << val << std::endl;
      try {
        m.insert(val);
      } catch (int) {
        bst_print_dot(m.get_rep().get_root(), i);
        exit(1);
      }
      // res_t res(m.insert(val));
      // std::cout << *res.first << ' ' << res.second << '\n';
    }
    bst_print_dot(m.get_rep().get_root(), 0);
    std::cout << "====forward=====\n";
    it_t i(m.begin());
    it_t l(m.end());
    for (; i != l; ++i) {
      std::cout << *i << '\n';
    }
    std::cout << "====backward=====\n";
    it_t b(m.end());
    l = m.begin();
    --b;
    for (i = b; i != l; --i) {
      std::cout << *i << '\n';
    }
    std::cout << *i << '\n';
  }
}
