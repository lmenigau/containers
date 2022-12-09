#include <iostream>
#include <vector>

#include "allocator.hpp"
#include "vector.hpp"

// minimal C++11 allocator with debug output

#ifndef NS
#define NS ft
#endif

int main() {
  const int max_elements = 32;

  std::cout << "using reserve: \n";
  {
    NS::vector<int, NAlloc<int> > v1;
    v1.reserve(max_elements);

    for (int n = 0; n < max_elements; ++n) v1.push_back(n);
  }

  std::cout << "not using reserve: \n";
  {
    NS::vector<int, NAlloc<int> > v1;

    for (int n = 0; n < max_elements; ++n) {
      if (v1.size() == v1.capacity()) {
        std::cout << "size() == capacity() == " << v1.size() << '\n';
      }
      v1.push_back(n);
    }
  }
}
