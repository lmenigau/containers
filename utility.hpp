#ifndef UTILITY_HPP
#define UTILITY_HPP
namespace ft {
  template <class T1, class T2>
  struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair();
    pair(const T1& x, const T2& y);
    template<class U, class V> pair(const pair<U, V> & p);
  };
  template <class T1, class T2>
    bool operator==(const pair<T1,T2>&, const pair<T1,T2>&);
  template <class T1, class T2>
    bool operator< (const pair<T1,T2>&, const pair<T1,T2>&);
  template <class T1, class T2>
    bool operator!=(const pair<T1,T2>&, const pair<T1,T2>&);
  template <class T1, class T2>
    bool operator> (const pair<T1,T2>&, const pair<T1,T2>&);
  template <class T1, class T2>
    bool operator>=(const pair<T1,T2>&, const pair<T1,T2>&);
  template <class T1, class T2>
    bool operator<=(const pair<T1,T2>&, const pair<T1,T2>&);
  template <class T1, class T2> pair<T1,T2> make_pair(const T1&, const T2&);
}
#endif
