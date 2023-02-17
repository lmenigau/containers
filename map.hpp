#ifndef MAP_HPP
#define MAP_HPP
#include <iostream>
#include <memory>
#include <utility>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "utility.hpp"
#include "rbtree.hpp"

namespace ft {

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<pair<const Key, T> > >
class map {
public:
  // types:
  typedef Key key_type;
  typedef T mapped_type;
  typedef pair<const Key, T> value_type;
  class value_compare : public binary_function<value_type, value_type, bool> {
    friend class map;

  protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}

  public:
    bool operator()(const value_type &x, const value_type &y) const {
      return comp(x.first, y.first);
    }
  };

private:
  typedef typename Allocator::template rebind<Node<value_type> >::other
      node_allocator;
  typedef Rbtree<Key, value_type, value_compare, node_allocator> rep_type;

  typedef typename rep_type::node_type node_type;

public:
  typedef Compare key_compare;
  typedef Allocator allocator_type;
  typedef typename Allocator::reference reference;
  typedef typename Allocator::const_reference const_reference;
  typedef typename rep_type::iterator iterator;               // See 23.1
  typedef typename rep_type::const_iterator const_iterator;   // See 23.1
  typedef size_t size_type;                                   // See 23.1
  typedef typename iterator::difference_type difference_type; // See 23.1
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  rep_type tree;

public:
  rep_type &get_rep() { return tree; }
  explicit map(const Compare &comp = Compare(), const Allocator & = Allocator())
      : tree(value_compare(comp)) {}
  template <class InputIterator>
  map(InputIterator first, InputIterator last, const Compare &comp = Compare(),
      const Allocator & = Allocator());
  map(const map<Key, T, Compare, Allocator> &x);
  ~map() {}
  map<Key, T, Compare, Allocator> &
  operator=(const map<Key, T, Compare, Allocator> &x);
  // iterators:
  iterator begin() { return tree.begin(); };
  const_iterator begin() const { return tree.begin(); }
  iterator end() { return tree.end(); }
  const_iterator end() const { return tree.end(); }
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  // capacity:
  bool empty() const;
  size_type size() const { return tree.size(); }
  size_type max_size() const { return tree.size(); };
  // 23.3.1.2 element access:
  T &operator[](const key_type &x) {}
  // modifiers:
  pair<iterator, bool> insert(const value_type &x) { return tree.insert(x); }

  iterator insert(iterator position, const value_type &x) {
    return tree.insert(x);
  }
  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {}
  void erase(iterator position);
  size_type erase(const key_type &x);
  void erase(iterator first, iterator last);
  void swap(map<Key, T, Compare, Allocator> &);
  void clear();
  // observers:
  key_compare key_comp() const;
  value_compare value_comp() const;
  // 23.3.1.3 map operations:
  iterator find(const key_type &x) { tree.find(x); };
  const_iterator find(const key_type &x) const;
  size_type count(const key_type &x) const;
  iterator lower_bound(const key_type &x);
  const_iterator lower_bound(const key_type &x) const;
  iterator upper_bound(const key_type &x);
  const_iterator upper_bound(const key_type &x) const;
  pair<iterator, iterator> equal_range(const key_type &x);
  pair<const_iterator, const_iterator> equal_range(const key_type &x) const;
};

template <class Key, class T, class Compare, class Allocator>
bool operator==(const map<Key, T, Compare, Allocator> &x,
                const map<Key, T, Compare, Allocator> &y) {
  return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}
template <class Key, class T, class Compare, class Allocator>
bool operator<(const map<Key, T, Compare, Allocator> &x,
               const map<Key, T, Compare, Allocator> &y) {
  return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template <class Key, class T, class Compare, class Allocator>
bool operator!=(const map<Key, T, Compare, Allocator> &x,
                const map<Key, T, Compare, Allocator> &y) {
  return !(x == y);
}
template <class Key, class T, class Compare, class Allocator>
bool operator>(const map<Key, T, Compare, Allocator> &x,
               const map<Key, T, Compare, Allocator> &y) {
  return y < x;
}
template <class Key, class T, class Compare, class Allocator>
bool operator>=(const map<Key, T, Compare, Allocator> &x,
                const map<Key, T, Compare, Allocator> &y) {
  return !(x < y);
}
template <class Key, class T, class Compare, class Allocator>
bool operator<=(const map<Key, T, Compare, Allocator> &x,
                const map<Key, T, Compare, Allocator> &y) {
  return !(y < x);
}
// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void swap(map<Key, T, Compare, Allocator> &x,
          map<Key, T, Compare, Allocator> &y) {
  x.swap(y);
}
} // namespace ft
#endif
