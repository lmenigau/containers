#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <functional>
#include "map.hpp"

#include "test_map.hpp"

template <class Function>
static void exect(long size, Function f) {
  f(size);
}

template <class Key, class Value>
struct test {
  typedef std::map<Key, Value> std_map;
  typedef ft::map<Key, Value> ft_map;
  typedef ft::pair<typename ft_map::iterator, bool> res_ft;
  typedef std::pair<typename std_map::iterator, bool> res_std;
  typedef void (&Func)(long);
  size_t size;
  std_map std;
  ft_map ft;

  template <class Function>
  test(long s, Function f) : size(s), std(), ft() {
    f(ft, size);
    f(std, size);
  }
};

void handler(int sig) {
  throw sig;
};

int main(int ac, char** av) {
  signal(SIGALRM, handler);
  alarm(2);
  long size = 16;
  if (ac >= 2)
    srandom(std::atoi(av[1]));
  if (ac >= 3)
    size = std::atol(av[2]);
  test<int, int> t(size, find_map());
  test<int, int> t2(size, map_insert());
  // insert_fill();
  // assign_fill();
}
