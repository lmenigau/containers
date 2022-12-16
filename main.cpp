#include <iostream>
#include <vector>

#include "allocator.hpp"
#include "vector.hpp"

#ifndef NS
#define NS ft
#endif
void clear();

int main() {
  const int max_elements = 32;

  std::cout << "using reserve: \n";
  {
    NS::vector<int, allocator<int> > v1;
    v1.reserve(max_elements);

    for (int n = 0; n < max_elements; ++n) v1.push_back(n);
  }

  std::cout << "not using reserve: \n";
  {
    NS::vector<int, allocator<int> > v1;

    for (int n = 0; n < max_elements; ++n) {
      if (v1.size() == v1.capacity()) {
      }
      std::cout << "push:" << n << " size:" << v1.size()
                << " capa:" << v1.capacity() << "\n";
      v1.push_back(n);
    }
    for (int n = 0; n < max_elements; ++n) {
      std::cout << "v: " << v1[n] << '\n';
    }
  }
  clear();
}
