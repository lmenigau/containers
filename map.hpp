#ifndef MAP_HPP
#define MAP_HPP
#include "algorithm.hpp"
#include "iterator.hpp"
#include "rbtree.hpp"
#include "utility.hpp"

namespace ft {

template <class Key,
          class T,
          class Compare = std::less<Key>,
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
    bool operator()(const value_type& x, const value_type& y) const {
      return comp(x.first, y.first);
    }
  };

 private:
  typedef typename Allocator::template rebind<Node_Val<value_type> >::other
      node_allocator;
  typedef Rbtree<Key, value_type, value_compare, node_allocator> rep_type;

  typedef typename rep_type::Node_Val node_type;

 public:
  typedef Compare key_compare;
  typedef Allocator allocator_type;
  typedef typename Allocator::reference reference;
  typedef typename Allocator::const_reference const_reference;
  typedef typename rep_type::iterator iterator;                // See 23.1
  typedef typename rep_type::const_iterator const_iterator;    // See 23.1
  typedef size_t size_type;                                    // See 23.1
  typedef typename iterator::difference_type difference_type;  // See 23.1
  typedef typename Allocator::pointer pointer;
  typedef typename Allocator::const_pointer const_pointer;
  typedef ft::reverse_iterator<iterator> reverse_iterator;
  typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

  rep_type tree;

 public:
  rep_type& get_rep() { return tree; }
  explicit map(const Compare& comp = Compare(), const Allocator& = Allocator())
      : tree(value_compare(comp)) {}
  template <class InputIterator>
  map(InputIterator first,
      InputIterator last,
      const Compare& comp = Compare(),
      const Allocator& = Allocator())
      : tree(value_compare(comp)) {
    for (; first != last; ++first)
      tree.insert(*first);
  }
  map(const map& x) : tree(x.tree) {}
  map<Key, T, Compare, Allocator>& operator=(
      const map<Key, T, Compare, Allocator>& x) {
    if (this != &x)
      tree = x.tree;
    return *this;
  }
  ~map() { clear(); }
  // iterators:
  iterator begin() { return tree.begin(); }
  const_iterator begin() const { return tree.begin(); }
  iterator end() { return tree.end(); }
  const_iterator end() const { return tree.end(); }
  reverse_iterator rbegin() { return reverse_iterator(tree.end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(tree.end());
  }
  reverse_iterator rend() { return reverse_iterator(tree.begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(tree.begin());
  }

  // capacity:
  bool empty() const { return size() == 0; };
  size_type size() const { return tree.size(); }
  size_type max_size() const { return tree.max_size(); }

  // 23.3.1.2 element access:
  T& operator[](const key_type& x) {
    value_type v(x, mapped_type());
    iterator it = tree.insert(v).first;
    return it->second;
  }
  // modifiers:
  pair<iterator, bool> insert(const value_type& x) { return tree.insert(x); }

  iterator insert(iterator position, const value_type& x) {
    return tree.insert(position, x);
  }

  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    for (; first != last; ++first)
      tree.insert(*first);
  }
  void erase(iterator position) { tree.erase(position); }

  size_type erase(const key_type& x) {
    iterator it(tree.find(x));
    if (it == tree.end())
      return 0;
    tree.erase(it);
    return 1;
  }

  void erase(iterator first, iterator last) {
    while (first != last) {
      erase(first++);
    }
  }

  void swap(map<Key, T, Compare, Allocator>& other) { tree.swap(other.tree); }

  void clear() { tree.clear(); }

  // observers:
  key_compare key_comp() const { return key_compare(); };
  value_compare value_comp() const { return value_compare(key_compare()); };

  // 23.3.1.3 map operations:
  iterator find(const key_type& x) { return tree.find(x); }
  const_iterator find(const key_type& x) const { return tree.find(x); }
  size_type count(const key_type& x) const { return tree.find(x) != end(); }
  iterator lower_bound(const key_type& x) { return tree.lower_bound(x); }
  const_iterator lower_bound(const key_type& x) const {
    return tree.lower_bound(x);
  }
  iterator upper_bound(const key_type& x) { return tree.upper_bound(x); }
  const_iterator upper_bound(const key_type& x) const {
    return tree.upper_bound(x);
  }
  pair<iterator, iterator> equal_range(const key_type& x) {
    return ft::make_pair(lower_bound(x), upper_bound(x));
  }
  pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
    return ft::make_pair(lower_bound(x), upper_bound(x));
  };
};

template <class Key, class T, class Compare, class Allocator>
bool operator==(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y) {
  return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}
template <class Key, class T, class Compare, class Allocator>
bool operator<(const map<Key, T, Compare, Allocator>& x,
               const map<Key, T, Compare, Allocator>& y) {
  return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}
template <class Key, class T, class Compare, class Allocator>
bool operator!=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y) {
  return !(x == y);
}
template <class Key, class T, class Compare, class Allocator>
bool operator>(const map<Key, T, Compare, Allocator>& x,
               const map<Key, T, Compare, Allocator>& y) {
  return y < x;
}
template <class Key, class T, class Compare, class Allocator>
bool operator>=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y) {
  return !(x < y);
}
template <class Key, class T, class Compare, class Allocator>
bool operator<=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y) {
  return !(y < x);
}

// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void swap(map<Key, T, Compare, Allocator>& x,
          map<Key, T, Compare, Allocator>& y) {
  x.swap(y);
}
}  // namespace ft
#endif
