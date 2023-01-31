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
  insert_fill();
  assign_fill();
}
