#pragma once
#include <iostream>

template <class T>
class NAlloc : std::allocator<T> {
 public:
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef T *pointer;
  typedef const T *const_pointer;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T value_type;
  template <class U>
  struct rebind {
    typedef NAlloc<U> other;
  };
  NAlloc() throw() { std::cout << "Nalloc constructor\n"; }
  NAlloc(const NAlloc &) throw() { std::cout << "Nalloc copy constructor\n"; }
  template <class U>
  NAlloc(const NAlloc<U> &) throw() {
    std::cout << "Nalloc template copy constructor\n";
  }
  ~NAlloc() throw() {}
  pointer address(reference x) const {
    std::cout << "Nalloc address\n";
    return &x;
  }
  const_pointer address(const_reference x) const {
    std::cout << "const Nalloc address\n";
  }
  pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0) {
    n *= sizeof(T);
    T *p = static_cast<T *>(::operator new(n));
    std::cout << "allocating " << n << " bytes\n";
    return p;
  }
  void deallocate(pointer p, size_type n) {
    std::cout << "deallocating " << n * sizeof *p << " bytes\n";
    ::operator delete(p);
  }
  size_type max_size() const throw() {
    std::cout << "max_size\n";
    return -1;
  }
  void construct(pointer p, const T &val) {
    std::cout << "construct\n";
    new ((void *)p) T(val);
  }
  void destroy(pointer p) {
    std::cout << "destroy\n";
    ((T *)p)->~T();
  }
};

template <class T, class U>
bool operator==(const NAlloc<T> &, const NAlloc<U> &) {
  return true;
}
template <class T, class U>
bool operator!=(const NAlloc<T> &, const NAlloc<U> &) {
  return false;
}
