
#include <iostream>
#include <vector>

#include "allocator.hpp"
#include "vector.hpp"

#ifndef NS
#define NS ft
#endif

void insert_fill() {
    NS::vector<int, allocator<int> > v2(10, 30);
    std::cout << "insert fill end : \n";
    v2.insert(v2.end(), 10, 999999);
    for (size_t i  = 0; i < v2.size();i++)
      std::cout << v2[i] << "\n";
    std::cout << "insert fill begin : \n";
    v2.insert(v2.begin(), 10, 88888);
    for (size_t i  = 0; i < v2.size();i++)
      std::cout << v2[i] << "\n";

    std::cout << "reserve\n";
    v2.reserve(100);
    std::cout << "insert fill end : \n";
    v2.insert(v2.end(), 10, 999999);
    for (size_t i  = 0; i < v2.size();i++)
      std::cout << v2[i] << "\n";
    std::cout << "insert fill begin : \n";
    v2.insert(v2.begin(), 10, 88888);
    for (size_t i  = 0; i < v2.size();i++)
      std::cout << v2[i] << "\n";
}
