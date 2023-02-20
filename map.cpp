#include <signal.h>
#include "test_map.hpp"
#include "test.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <iterator>
#include <sstream>

#if IS_STD
namespace ft = std;
#include <map>
#define bst_print_dot(...)
#else
#include "map.hpp"
#include "print_dot.hpp"
#endif

using std::ostream;

ft::map<int, int> m;
void handler(int sig) {
  throw sig;
};

template <class C>
void safe_insert(C& c, typename C::value_type& val) {
  try {
    c.insert(val);
  } catch (int) {
    bst_print_dot(c.get_rep().get_root());
    throw;
  }
}

template <class C>
void fill_random(C& c, long size) {
  std::generate_n(std::inserter(c, c.begin()), size, random);
  for (long i = 0; i < size; i++) {
    typename C::value_type val(random() % size, i);
    safe_insert(c, val);
  }
}

template <class T1, class T2>
std::string toString(const ft::pair<T1, T2>& pair) {
  std::ostringstream str;
  str << "<" << pair.first << ", " << pair.second << ">";
  return str.str();
}

template <class T1, class T2>
void pair_print(const ft::pair<T1, T2>& pair) {
  std::cout << "<" << pair.first << ", " << pair.second << ">\n";
}

template <class C>
void print(C& c) {
  typedef typename C::value_type vt;
  std::for_each(c.begin(), c.end(),
                pair_print<typename vt::first_type, typename vt::second_type>);
}

template <class C>
void rev_print(C& c) {
  typedef typename C::value_type vt;
  std::for_each(c.rbegin(), c.rend(),
                pair_print<typename vt::first_type, typename vt::second_type>);
}

void find_map(long size) {
  typedef ft::map<int, int> Map;
  typedef Map::iterator Iterator;
  typedef Map::key_type Key;

  Map m;
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

void map_insert(long size) {
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
    fill_random(m, size);
    bst_print_dot(m.get_rep().get_root(), 0);
    std::cout << "====forward=====\n";
    print(m);
    std::cout << "====backward=====\n";
    rev_print(m);
  }
}
