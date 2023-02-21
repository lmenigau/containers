#include <unistd.h>
#ifndef NS
#define NS ft
#endif

#include <cstdlib>
#include <iostream>
#include <string>

#include "test.hpp"

int main(int ac, char** av) {
  alarm(2);
  long size = 16;
  if (ac >= 2)
    srandom(std::atoi(av[1]));
  if (ac >= 3)
    size = std::atol(av[2]);
  find_map(size);
  map_insert(size);
  // insert_fill();
  // assign_fill();
}
