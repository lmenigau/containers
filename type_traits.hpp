#pragma once
#include <limits>

namespace ft
{
  template<bool B, class T = void>
    struct enable_if {};

  template<class T>
    struct enable_if<true, T> { typedef T type; };

  template<class T, T v>
    struct integral_constant {
      static const T value = v;
      typedef T value_type;
      typedef integral_constant type; // using injected-class-name
      operator T() const { return value; }
      const T operator()() const { return value; } // since c++14
    };

  typedef integral_constant<bool, true> true_type;
  typedef integral_constant<bool, false> false_type;

  template< class T >
    struct is_integral : integral_constant<bool, std::numeric_limits<T>::is_integer > {};
}
