#pragma once

#include <cstddef>
#include <iterator>
#include "node.hpp"

namespace ft {
template <typename T>
struct BTreeIterator {
  typedef T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef T& reference;
  typedef T* pointer;
  typedef std::bidirectional_iterator_tag iterator_category;

  BTreeIterator() : nodep() {}
  BTreeIterator(const Node* n) : nodep(const_cast<Node*>(n)) {}
  // BTreeIterator(const BTreeIterator& it) : nodep(it.nodep) {}
  reference operator*() const { return ((Node_Val<T>*)nodep)->value; }
  pointer operator->() const { return &((Node_Val<T>*)&nodep)->value; }

  BTreeIterator& operator++() {
    nodep = Node::increment(nodep);
    return *this;
  }

  BTreeIterator operator++(int) {
    BTreeIterator tmp = *this;
    nodep = Node::increment(nodep);
    return tmp;
  }

  BTreeIterator& operator--() {
    nodep = Node::decrement(nodep);
    return *this;
  }

  BTreeIterator operator--(int) {
    BTreeIterator tmp = *this;
    nodep = Node::decrement(nodep);
    return tmp;
  }

  Node* nodep;
  friend bool operator==(const BTreeIterator& x, const BTreeIterator& y) {
    return x.nodep == y.nodep;
  }

  friend bool operator!=(const BTreeIterator& x, const BTreeIterator& y) {
    return x.nodep != y.nodep;
  }
};

template <typename T>
struct ConstBTreeIterator {
  typedef const T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef const T& reference;
  typedef const T* pointer;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef BTreeIterator<T> iterator;

  ConstBTreeIterator() : nodep() {}
  ConstBTreeIterator(const Node* n) : nodep(const_cast<Node*>(n)) {}

  ConstBTreeIterator(const iterator& it) : nodep(const_cast<Node*>(it.nodep)) {}
  reference operator*() const { return ((Node_Val<const T>*)nodep)->value; }
  pointer operator->() const { return &((Node_Val<const T>*)&nodep)->value; }

  ConstBTreeIterator& operator++() {
    nodep = Node::increment(nodep);
    return *this;
  }

  ConstBTreeIterator operator++(int) {
    ConstBTreeIterator tmp = *this;
    nodep = Node::increment(nodep);
    return tmp;
  }

  ConstBTreeIterator& operator--() {
    nodep = Node::decrement(nodep);
    return *this;
  }

  ConstBTreeIterator operator--(int) {
    ConstBTreeIterator tmp = *this;
    nodep = Node::decrement(nodep);
    return tmp;
  }

  Node* nodep;

  friend bool operator==(const ConstBTreeIterator& x,
                         const ConstBTreeIterator& y) {
    return x.nodep == y.nodep;
  }

  friend bool operator!=(const ConstBTreeIterator& x,
                         const ConstBTreeIterator& y) {
    return x.nodep != y.nodep;
  }
};

}  // namespace ft
