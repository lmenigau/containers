#ifndef NS
#define NS ft
#endif

#include <iostream>
#include <cstdlib>
#include <string>

#include "test.hpp"

int main(int ac, char **av) {
  if (ac ==2)
    srandom(std::atoi(av[1]));
  map_insert();
  // insert_fill();
  // assign_fill();
}
