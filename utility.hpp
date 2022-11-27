#ifndef UTILITY_HPP
#define UTILITY_HPP
namespace ft
{

  template <class Arg1, class Arg2, class Result> struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
  };

  template <class T> struct less : binary_function<T, T, bool> {
    bool operator()(const T &x, const T &y) const;
  };

  template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair();
    pair(const T1 &x, const T2 &y);
    template <class U, class V> pair(const pair<U, V> &p);
  };

  template <class T1, class T2>
    bool operator==(const pair<T1, T2> &, const pair<T1, T2> &);
  template <class T1, class T2>
    bool operator<(const pair<T1, T2> &, const pair<T1, T2> &);
  template <class T1, class T2>
    bool operator!=(const pair<T1, T2> &, const pair<T1, T2> &);
  template <class T1, class T2>
    bool operator>(const pair<T1, T2> &, const pair<T1, T2> &);
  template <class T1, class T2>
    bool operator>=(const pair<T1, T2> &, const pair<T1, T2> &);
  template <class T1, class T2>
    bool operator<=(const pair<T1, T2> &, const pair<T1, T2> &);
  template <class T1, class T2> pair<T1, T2> make_pair(const T1 &, const T2 &);
} // namespace ft
#endif
