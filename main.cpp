#include <iostream>
#include <vector>

#include "allocator.hpp"
#include "vector.hpp"

#ifndef NS
#define NS ft
#endif
int main() {
  const int max_elements = 14;

  std::cout << "using reserve: \n";
  NS::vector<int, allocator<int> > v1;
  v1.reserve(max_elements);

  for (int n = 0; n < max_elements; ++n) v1.insert(v1.end(), n);

  std::cout << "not using reserve: \n";
  {
    NS::vector<int, allocator<int> > v2;

    for (int n = 0; n < max_elements; ++n) {
      std::cout << "push:" << n << " size:" << v2.size()
                << " capa:" << v2.capacity() << "\n";
      v2.insert(v2.end(), n);
    }
    // v2.resize(10);
    // for (int n = 0; n < max_elements; ++n) {
    //   if (v2.size() == v2.capacity()) {
    //   }
    //   std::cout << "push:" << n << " size:" << v2.size()
    //             << " capa:" << v2.capacity() << "\n";
    //   v2.insert(v2.end(), n);
    // }
    for (int n = 0; n < max_elements; ++n) {
      std::cout << "v: " << v2[n] << '\n';
    }
    v2.clear();
  }
}
