#pragma once

#include "node.hpp"
#include <iterator>

namespace ft {
template <typename T> struct BTreeIterator {
  typedef T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef T &reference;
  typedef T *pointer;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef Node<T> Node;

  BTreeIterator(Node *n) : nodep(n) {}
  reference operator*() { return nodep->value; }

  BTreeIterator &operator++() {
    nodep = Node::increment(nodep);
    return *this;
  }

  BTreeIterator operator++(int) {
    BTreeIterator tmp = *this;
    nodep = Node::increment(nodep);
    return tmp;
  }

  BTreeIterator &operator--() {
    nodep = Node::decrement(nodep);
    return *this;
  }

  BTreeIterator operator--(int) {
    BTreeIterator tmp = *this;
    nodep = Node::decrement(nodep);
    return tmp;
  }
  Node *nodep;
  friend bool operator==(const BTreeIterator &x, const BTreeIterator &y) {
    return x.nodep == y.nodep;
  }

  friend bool operator!=(const BTreeIterator &x, const BTreeIterator &y) {
    return x.nodep != y.nodep;
  }
};

template <typename T> struct ConstBTreeIterator {
  typedef T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef T &reference;
  typedef T *pointer;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef Node<T> Node;
  typedef BTreeIterator<T> iterator;

  ConstBTreeIterator(Node *n) : nodep(n) {}

  ConstBTreeIterator(const iterator &it) : nodep(it.nodep) {}
  reference operator*() { return nodep->value; }

  ConstBTreeIterator &operator++() {
    nodep = Node::increment(nodep);
    return *this;
  }
  ConstBTreeIterator operator++(int) {
    ConstBTreeIterator tmp = *this;
    nodep = Node::increment(nodep);
    return tmp;
  }

  ConstBTreeIterator &operator--() {
    nodep = Node::decrement(nodep);
    return *this;
  }

  ConstBTreeIterator operator--(int) {
    ConstBTreeIterator tmp = *this;
    nodep = Node::decrement(nodep);
    return tmp;
  }

  Node *nodep;

  friend bool operator==(const ConstBTreeIterator &x,
                         const ConstBTreeIterator &y) {
    return x.nodep == y.nodep;
  }

  friend bool operator!=(const ConstBTreeIterator &x,
                         const ConstBTreeIterator &y) {
    return x.nodep != y.nodep;
  }
};

} // namespace ft
