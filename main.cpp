#include <iostream>
#include <vector>

#include "allocator.hpp"
#include "vector.hpp"

#ifndef NS
#define NS ft
#endif

void insert_fill();
void insert_single();
void assign_fill();

int main() {
  using ft::vector;
  vector<int> a(10, 4);
  vector<int> b(10, 3);
  
  typedef vector<int>::reverse_iterator rit;
  a = b;
  rit rf(b.rbegin());
  rit rl(b.rend());

  vector<int> v(10);
  
  v.erase(v.begin(), v.begin() + 3);
  while (rf != rl)
    std::cout << *(rf++) << "\n";

  insert_fill();
  assign_fill();
}
