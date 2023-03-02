#include <signal.h>
#include <sys/types.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include "algorithm.hpp"
#include "map.hpp"
#include "vector.hpp"

#include "test_map.hpp"

namespace ft {
template <class T1, class T2>
bool operator==(const ft::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}
}  // namespace ft

template <class Key, class Value>
struct test {
  typedef std::map<Key, Value> std_map;
  typedef ft::map<Key, Value> ft_map;
  typedef ft::pair<typename ft_map::iterator, bool> res_ft;
  typedef std::pair<typename std_map::iterator, bool> res_std;

  ft::vector<int> v;
  long count;
  test(long size) : count(1) {
    v.reserve(size);
    for (long i = 0; i < size; i++)
      v.push_back(i);
  }

  template <class Function>
  void operator()(long s, Function f) {
    std::next_permutation(v.begin(), v.end());
    // std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout,
    // "\n"));
    std_map std;
    ft_map ft;
    f(std, v, s);
    f(ft, v, s);
    if (!ft::equal(ft.begin(), ft.end(), std.begin())) {
      std::cout << __FILE__ << ":" << __LINE__ << " in " << __PRETTY_FUNCTION__
                << '\n';
      print(std);
      std::cout << "=========\n";
      print(ft);
      /* print_map(ft, ); */
      std::cout << "KO at " << count << "\n";
    }
    std::cout << count++ << '\n';
  }
};

void handler(int sig) {
  throw sig;
};

int main(int ac, char** av) {
  // signal(SIGALRM, handler);
  // alarm(2);
  long size = 16;
  if (ac >= 2)
    srandom(std::atoi(av[1]));
  if (ac >= 3)
    size = std::atol(av[2]);
  test<int, int> t(size);
  /* for (u_int i(0); i < size; i++) { */
  /* t(i, find_map()); */
  /* } */
  /* t(size, map_insert()); */
  for (int i = 0; i < 20; i++) {
    t(size, map_erase());
    t(size, map_insert());
    t(size, copy_construct());
    t(size, range_construct());
    t(size, range_erase());
    t(size, map_insert_hint());
    t(size, swap());
  }
}
