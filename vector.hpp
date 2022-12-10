#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>
#include <memory>

#include "iterator.hpp"
#include "type_traits.h"

namespace ft {
template <class T, class Allocator = std::allocator<T> >
class vector {
 private:
  template <class It>
  struct VecIterator
      : iterator<bidirectional_iterator_tag, It, ptrdiff_t, It *, It &> {};
  typedef typename Allocator::template rebind<T>::other Ralloc;

 public:
  // types:
  typedef typename Allocator::reference reference;
  typedef typename Allocator::const_reference const_reference;
  typedef VecIterator<T> iterator;
  typedef VecIterator<const T> const_iterator;
  typedef size_t size_type;
  typedef typename iterator::difference_type difference_type;
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  // 23.2.4.1 construct/copy/destroy:
 private:
  Ralloc alloc;
  pointer vec;
  size_type _size;
  size_type _capacity;

 public:
  vector() : vec(0), _size(0), _capacity(0) {}

  explicit vector(const Allocator &a) : alloc(a), _size(0), _capacity(0) {
    std::cout << "ft_allocator_type\n";
  }
  explicit vector(size_type n, const T &value = T(),
                  const Allocator &a = Allocator())
      : vec(alloc.allocate(n)), _size(n), _capacity(n) {
    std::cout << "ft_size_type , T\n";
  }
  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &a = Allocator())
      : _size(0), _capacity(0) {
    std::cout << "ft_first, last\n";
    typedef typename is_integral<InputIterator>::value _Integral;
    _Integral();
  }
  vector(const vector<T, Allocator> &x) {}

  ~vector() { alloc.deallocate(vec, _capacity); }

  vector<T, Allocator> &operator=(const vector<T, Allocator> &x);
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last)
  {
    for (size_type i = 0 ; first < last; i++)
      alloc.construct(vec + i, *first);
  }
  void assign(size_type n, const T &u)
  {
    for (size_type i = 0; i <  n && i < _size; i++)
      alloc.construct(vec + i, u);
  };
  allocator_type get_allocator() const { return alloc; };

  // iterators:
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;

  // 23.2.4.2 capacity:
  size_type size() const { return _size; }
  size_type max_size() const { return alloc.max_size(); };
  void resize(size_type sz, T c = T());
  size_type capacity() const { return _capacity; }
  bool empty() const {}

  void reserve(size_type n) {
    pointer new_vec;
    if (n <= _capacity) return;
    new_vec = alloc.allocate(n);
    for (size_type i = 0; i < _size; i++) {
      vec[i] = new_vec[i];
      alloc.destroy(vec + i);
    }
    alloc.deallocate(vec, _capacity);
    _capacity = n;
    vec = new_vec;
  };

  // element access:
  reference operator[](size_type n) { return *(vec + n); }
  const_reference operator[](size_type n) const { return *(vec + n); }
  const_reference at(size_type n) const { return this[n]; }
  reference at(size_type n) { return this[n]; }
  reference front() { return vec; }
  const_reference front() const { return vec; }
  reference back() { return *(vec + _size - 1); }
  const_reference back() const { return back(); }

  // 23.2.4.3 modifiers:
  void push_back(const T &x) {
    if (_size == 0)
      reserve(1);
    else if (_size == _capacity)
      reserve(_size * 2);
    alloc.construct(vec + _size, x);
    _size++;
  };

  void pop_back();
  iterator insert(iterator position, const T &x);
  void insert(iterator position, size_type n, const T &x);
  template <class InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last);
  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);
  void swap(vector<T, Allocator> &);
  void clear();
};
}  // namespace ft
#endif
