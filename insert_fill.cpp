
#include <iostream>
#include <vector>

#include "allocator.hpp"
#include "vector.hpp"

#ifndef NS
#define NS ft
#endif

void insert_fill() {
    const NS::vector<int, allocator<int> > v2(10, 30);
    std::cout << "insert fill end : \n";
    //v2.insert(v2.end(), 10, 999999);
    std::cout << "insert fill begin : \n";
    //v2.insert(v2.begin(), 10, 88888);
}
