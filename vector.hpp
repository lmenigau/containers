#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>
#include <memory>
#include <stdexcept>

#include "iterator.hpp"
#include "type_traits.h"

namespace ft {
template <class T, class Allocator = std::allocator<T> >
class vector {
 private:
  template <class It>
  struct VecIterator
      : iterator<random_access_iterator_tag, It, ptrdiff_t, It *, It &> {};
  typedef typename Allocator::template rebind<T>::other Ralloc;

 public:
  // types:
  typedef typename Allocator::reference reference;
  typedef typename Allocator::const_reference const_reference;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef size_t size_type;
  typedef typename iterator_traits<iterator>::difference_type difference_type;
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
  // 23.2.4.1 construct/copy/destroy:
 private:
  Ralloc alloc;
  pointer vec;
  size_type _size;
  size_type _capacity;
  template <typename Integer>
  void assign_dispatch(Integer n, const T &value, true_type) {
    assign(n, value);
  }
  template <class InputIterator>
  void assign_dispatch(InputIterator first, InputIterator last, false_type) {
    for (; first < last; first++) {
      push_back(*first);
    }
  }
  template <typename Integer>
  void insert_dispatch(iterator position, Integer n, const T &x, true_type) {}

  template <class InputIterator>
  void insert_dispatch(iterator position, InputIterator first,
                       InputIterator last, false_type) {}

 public:
  vector() : vec(0), _size(0), _capacity(0) {}

  explicit vector(const Allocator &a)
      : alloc(a), vec(0), _size(0), _capacity(0) {}
  explicit vector(size_type n, const T &value = T(),
                  const Allocator &a = Allocator())
      : vec(alloc.allocate(n)), _size(n), _capacity(n) {
    assign(n, value);
  }
  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &a = Allocator())
      : _size(0), _capacity(0) {
    typedef typename is_integral<InputIterator>::value _Integral;
    assign_dispatch(first, last, _Integral());
  }
  vector(const vector<T, Allocator> &x) {}

  ~vector() {
    clear();
    alloc.deallocate(vec, _capacity);
  }

  vector<T, Allocator> &operator=(const vector<T, Allocator> &x);
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {
    typedef typename is_integral<InputIterator>::value _Integral;
    assign_dispatch(first, last, _Integral());
  }

  void assign(size_type n, const T &u) {
    for (size_type i = 0; i < n && i < _size; i++) alloc.construct(vec + i, u);
  }
  allocator_type get_allocator() const { return alloc; }

  // iterators:
  iterator begin() { return vec; }
  const_iterator begin() const { return vec; }
  iterator end() { return vec + _size; }
  const_iterator end() const { return vec + _size; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return reverse_iterator(begin()); }

  // 23.2.4.2 capacity:
  size_type size() const { return _size; }
  size_type max_size() const { return alloc.max_size(); }
  void resize(size_type sz, T c = T()){

  };
  size_type capacity() const { return _capacity; }
  bool empty() const { return _size == 0; }

 private:
  size_type grow_check(size_t n, const char *s) {
    if (max_size() - _size < n) throw std::length_error(s);
    const size_type len = _size + (std::max)(_size, n);
    return (len < _size || len > max_size()) ? max_size() : len;
  }

 public:
  void reserve(size_type n) {
    pointer new_vec;
    if (n <= _capacity) return;
    if (n > max_size()) throw std::length_error("");
    new_vec = alloc.allocate(n);
    for (size_type i = 0; i < _size; i++) {
      alloc.construct(new_vec + i, vec[i]);
    }
    for (size_type i = 0; i < _size; i++) {
      alloc.destroy(vec + i);
    }
    if (_capacity) alloc.deallocate(vec, _capacity);
    _capacity = n;
    vec = new_vec;
  }

  // element access:
  reference operator[](size_type n) { return *(vec + n); }
  const_reference operator[](size_type n) const {
    if (n >= _size) throw std::out_of_range("vector out of bound access");
    return *(vec + n);
  }
  const_reference at(size_type n) const {
    if (n >= _size) throw std::out_of_range("vector out of bound access");
    return this[n];
  }
  reference at(size_type n) { return this[n]; }
  reference front() { return vec; }
  const_reference front() const { return vec; }
  reference back() { return *(vec + _size - 1); }
  const_reference back() const { return back(); }

  // 23.2.4.3 modifiers:
  void push_back(const T &x) {
	  insert(end(), x);
  }

  void pop_back() {
    alloc.destroy(vec + _size - 1);
    _size--;
  };

 public:
  iterator insert(iterator position, const T &x) {
    if (_size < _capacity) {
      if (position == end())
        alloc.construct(end(), x);
      else {
        alloc.construct(end(), *(end() - 1));
        T x_copy = x;
        std::copy_backward(position, end(), end() + 1);
        *position = x_copy;
      }
    } else {
      size_type new_cap =
          grow_check(1, "ft::vector insert length_error");
      pointer new_vec = alloc.allocate(new_cap);
      difference_type i = 0;
      try {
        alloc.construct(new_vec + (position - vec), x);
        for (; i < position - vec; i++)
          alloc.construct(new_vec + i, *(vec + i));
        for (i++; vec + i < vec + _size; i++)
          alloc.construct(new_vec + i, *(vec + i));
      } catch (...) {
        for (size_t j = 0; j < i; j++) alloc.destroy(new_vec + j);
        alloc.deallocate(new_vec, new_cap);
        throw;
      }
      for (size_t j = 0; j < _size; j++) alloc.destroy(vec + j);
      if (vec)
        alloc.deallocate(vec, _size);
      vec = new_vec;
      _capacity = new_cap;
    }
    _size++;
    return position;
  }
  void insert(iterator position, size_type n, const T &x) {}
  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last) {
    typedef typename is_integral<InputIterator>::value _Integral;
    insert_dispatch(_Integral());
  };
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void swap(vector<T, Allocator> &);
  void clear() {
    for (size_t i = 0; i < _size; i++) {
      alloc.destroy(vec + i);
    }
    _size = 0;
  }
};
}  // namespace ft
#endif
