#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>
#include <memory>
#include <stdexcept>

#include "iterator.hpp"
#include "type_traits.hpp"

namespace ft {
template <class T, class Allocator = std::allocator<T> >
class vector {
 private:
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

 public:
  iterator begin() { return vec; }
  const_iterator begin() const { return vec; }
  iterator end() { return vec + _size; }
  const_iterator end() const { return vec + _size; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { return reverse_iterator(begin()); }

  size_type capacity() const { return _capacity; }
  bool empty() const { return _size == 0; }
  size_type size() const { return _size; }
  size_type max_size() const { return alloc.max_size(); }

  void clear() {
    for (size_t i = 0; i < _size; i++) {
      alloc.destroy(vec + i);
    }
    _size = 0;
  }

 private:
  template <typename Integer>
  void init_dispatch(Integer n, const T &value, true_type) {
    if (size_type(n) > max_size())
      throw std::length_error("ft::vector::vector length_error");
    vec = alloc.allocate(n);
    _capacity = n;
    size_type i(0);
    try {
      for (; i < (size_type)n; i++) {
        alloc.construct(vec + i, value);
      }
    } catch (...) {
      for (; i >= 0; i--) {
        alloc.destroy(vec + i);
        throw;
      }
    }
    _size = n;
  }

  template <typename InputIterator>
  void init_range(InputIterator first, InputIterator last,
                  std::input_iterator_tag) {
    try {
      for (; first != last; first++) {
        push_back(*first);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  template <typename InputIterator>
  void init_range(InputIterator first, InputIterator last,
                  std::forward_iterator_tag) {
    vec = alloc.allocate(std::distance(first, last));
    try {
      for (InputIterator it(first); it != last; it++, _size++)
        alloc.construct(vec + _size, *it);
    } catch (...) {
      clear();
      throw;
    }
  }

  template <typename InputIterator>
  void init_dispatch(InputIterator first, InputIterator last, false_type) {
    init_range(first, last,
               typename iterator_traits<InputIterator>::iterator_category());
  }

  template <typename Integer>
  void assign_dispatch(Integer ni, const T &value, true_type) {
    size_type n(ni);
    if (n > _capacity) {
      vector tmp(n, value);
      swap(tmp);
    } else if (n >= _size) {
      std::fill(begin(), end(), value);
      size_type i(_size);
      try {
        for (; i < n; i++) alloc.construct(vec + i, value);
      } catch (...) {
        for (size_type j(_size); j < i; j++) alloc.destroy(vec + j);
        throw;
      }
      _size = n;
    } else if (n < _size) {
      std::fill_n(vec, n, value);
      for (size_type i(_size - n); i < _size; i++) alloc.destroy(vec + i);
      _size = n;
    }
  }

  template <class InputIterator>
  void assign_range(InputIterator first, InputIterator last,
                    std::input_iterator_tag) {
    pointer i(vec);
    for (; i < end() && first != last; ++first) *i = *first;
    if (first != last)
      insert_range(end(), first, last, std::input_iterator_tag());
    else {
      for (; i < end(); i++) alloc.destroy(i);
    }
  }

  template <class InputIterator>
  void assign_range(InputIterator first, InputIterator last,
                    std::forward_iterator_tag) {
    size_type len(std::distance(first, last));
    if (len > _capacity) {
      pointer new_vec(alloc.allocate(len));
      pointer new_end(new_vec);
      try {
        for (; first != last; ++first, new_end++)
          alloc.construct(new_end, *first);
      } catch (...) {
        for (pointer i(new_vec); i < new_end; i++) alloc.destroy(i);
        alloc.deallocate(new_vec, len);
      }
      clear();
      alloc.deallocate(vec, _capacity);
      vec = new_vec;
      _size = len;
      _capacity = len;
    } else {
      pointer i(vec);
      for (; i < end() && first != last; ++first) *i = *first;
      if (first != last) {
        for (pointer j(i); j < end(); j++) alloc.destroy(j);
        try {
          for (; first != last; ++first, i++) alloc.construct(i, *first);
        } catch (...) {
          for (pointer j(vec + _size); j < i; j++) alloc.destroy(j);
        }
        _size = len;
      } else {
        for (; i < end(); i++) alloc.destroy(i);
      }
    }
  }

  template <class InputIterator>
  void assign_dispatch(InputIterator first, InputIterator last, false_type) {
    assign_range(first, last,
                 typename iterator_traits<InputIterator>::iterator_category());
  }

  template <typename Integer>
  iterator insert_dispatch(iterator position, Integer n, const T &x,
                           true_type) {
    if (_size + n <= _capacity) {
      if (position == end())
        for (Integer i(0); i < n; i++) alloc.construct(end() + i, x);
      else {
        for (Integer i(0); i < n; i++)
          alloc.construct(end() - i, *(end() - i - 1));
        T x_copy = x;
        std::copy_backward(position, end(), end() + n);
        for (Integer i(0); i < n; i++) alloc.construct(position + i, x_copy);
      }
    } else {
      size_type new_cap = grow_check(n, "ft::vector insert length_error");
      pointer new_vec = alloc.allocate(new_cap);
      pointer new_end(new_vec);
      pointer old(vec);
      try {
        for (; old < position; old++, new_end++) alloc.construct(new_end, *old);
        for (size_type j(0); j < size_type(n); j++, new_end++)
          alloc.construct(new_end, x);
        for (; old < end(); old++, new_end++) alloc.construct(new_end, *old);
      } catch (...) {
        for (pointer j(new_vec); j < new_end; j++) alloc.destroy(j);
        alloc.deallocate(new_vec, new_cap);
        throw;
      }
      for (size_t j = 0; j < _size; j++) alloc.destroy(vec + j);
      if (vec) alloc.deallocate(vec, _capacity);
      vec = new_vec;
      _capacity = new_cap;
    }
    _size += n;
    return position;
  }

  template <class InputIterator>
  void insert_range(iterator position, InputIterator first, InputIterator last,
                    std::input_iterator_tag) {
    if (position == end()) {
      for (; first != last; ++first) insert(end(), *first);
    } else {
      vector tmp(first, last);
      insert(position, tmp.begin(), tmp.end());
    }
  }

  template <class InputIterator>
  iterator insert_range(iterator position, InputIterator first,
                        InputIterator last, std::forward_iterator_tag) {
    size_type n(std::distance(first, last));
    if (_size + n <= _capacity) {
      if (position == end())
        for (pointer i(position); first != last; i++, ++first)
          alloc.construct(i, *first);
      else {
        for (size_type i(0); i < n; i++)
          alloc.construct(end() - i, *(end() - i - 1));
        std::copy_backward(position, end(), end() + n);
        for (pointer i(position); first != last; i++, ++first)
          alloc.construct(i, *first);
      }
    } else {
      size_type new_cap = grow_check(n, "ft::vector insert length_error");
      pointer new_vec = alloc.allocate(new_cap);
      pointer new_end(new_vec);
      pointer old(vec);
      iterator new_pos;
      try {
        for (; old < position; old++, new_end++) alloc.construct(new_end, *old);
        new_pos = new_end;
        for (; first != last; ++first, new_end++)
          alloc.construct(new_end, *first);
        for (; old < end(); old++, new_end++) alloc.construct(new_end, *old);
      } catch (...) {
        for (pointer j(new_vec); j < new_end; j++) alloc.destroy(j);
        alloc.deallocate(new_vec, new_cap);
        throw;
      }
      for (size_t j = 0; j < _size; j++) alloc.destroy(vec + j);
      if (vec) alloc.deallocate(vec, _capacity);
      vec = new_vec;
      _capacity = new_cap;
      _size += n;
      return new_pos;
    }
    _size += n;
    return position;
  }

  template <class InputIterator>
  iterator insert_dispatch(iterator position, InputIterator first,
                       InputIterator last, false_type) {
    typedef typename iterator_traits<InputIterator>::iterator_category tag;
    return insert_range(position, first, last, tag());
  }

 public:
  vector() : vec(0), _size(0), _capacity(0) {}

  explicit vector(const Allocator &a)
      : alloc(a), vec(0), _size(0), _capacity(0) {}
  explicit vector(size_type n, const T &value = T(),
                  const Allocator &a = Allocator())
      : alloc(a) {
    init_dispatch(n, value, true_type());
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last,
         const allocator_type &a = Allocator())
      : alloc(a) {
    init_dispatch(first, last, is_integral<InputIterator>());
  }

  vector(const vector &x)
      : alloc(x.alloc),
        vec(alloc.allocate(x._size)),
        _size(0),
        _capacity(_size) {
    pointer d(vec);
    try {
      for (pointer s(x.vec); s < x.end(); s++, s++) alloc.construct(d, *s);
    } catch (...) {
      for (pointer j(vec); j < d; j++) alloc.destroy(j);
      throw;
    }
    _size = x._size;
  }

  ~vector() {
    clear();
    if (vec) alloc.deallocate(vec, _capacity);
  }

  vector<T, Allocator> &operator=(const vector<T, Allocator> &x) {
    _size = x._size;

    return *this;
  }

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {
    assign_dispatch(first, last, is_integral<InputIterator>());
  }

  void assign(size_type n, const T &u) { assign_dispatch(n, u, true_type()); }

  allocator_type get_allocator() const { return alloc; }

  void resize(size_type sz, T c = T()) {
    if (sz > _size)
      insert_dispatch(end(), sz - _size, c, true_type());
    else {
      for (pointer i(vec + sz); i < end(); ++i) alloc.destroy(i);
      _size = sz;
    }
  }

 private:
  size_type grow_check(size_t n, const char *s) {
    if (max_size() - _size < n) throw std::length_error(s);
    const size_type len = _size + (std::max)(_size, n);
    return (len < _size || len > max_size()) ? max_size() : len;
  }

 public:
  void reserve(size_type n) {
    if (n > max_size())
      throw std::length_error("ft::vector::reserve length_error");
    if (n <= _capacity) return;
    pointer new_vec(alloc.allocate(n));
    size_type i(0);
    try {
      for (; i < _size; i++) alloc.construct(new_vec + i, vec[i]);
    } catch (...) {
      for (size_type j(0); j < i; i++) alloc.destroy(new_vec + j);
      alloc.deallocate(new_vec, n);
      throw;
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
    if (n >= _size) throw std::out_of_range("ft::vector out of bound access");
    return *(vec + n);
  }

  const_reference at(size_type n) const {
    if (n >= _size) throw std::out_of_range("ft::vector out of bound access");
    return vec[n];
  }
  reference at(size_type n) { return vec[n]; }
  reference front() { return *vec; }
  const_reference front() const { return *vec; }
  reference back() { return *(vec + _size - 1); }
  const_reference back() const { return *(vec + _size - 1); }

  // 23.2.4.3 modifiers:
  void push_back(const T &x) { insert(end(), x); }

  void pop_back() {
    _size--;
    alloc.destroy(vec + _size);
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
      size_type new_cap = grow_check(1, "ft::vector insert length_error");
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
      if (vec) alloc.deallocate(vec, _capacity);
      vec = new_vec;
      _capacity = new_cap;
    }
    _size++;
    return position;
  }

  iterator insert(iterator position, size_type n, const T &x) {
    return insert_dispatch(position, n, x, true_type());
  }
  template <class InputIterator>
  iterator insert(iterator position, InputIterator first, InputIterator last) {
    return insert_dispatch(position, first, last, is_integral<InputIterator>());
  };

  iterator erase(iterator position) {
    alloc.destroy(position);
    if (position != end()) std::copy(position + 1, end(), position);
    return position;
  };

  iterator erase(iterator first, iterator last) {
    iterator it(last);
    for (it = first; it != last; it++) alloc.destroy(it);
    if (last != end()) std::copy(last, end(), first);
    return last;
  }
  void swap(vector &other) {
    std::swap(vec, other.vec);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
  }
};
}  // namespace ft
#endif
