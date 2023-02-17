#pragma once

#if IS_STD
namespace ft = std;
#else
#include "utility.hpp"
#endif
template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, ft::pair<T1, T2> &p) {
  return os << '(' << p.first << ", " << p.second << ')';
}
