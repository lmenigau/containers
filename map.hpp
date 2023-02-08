#ifndef MAP_HPP
#define MAP_HPP
#include <memory>
#include <utility>

#include "iterator.hpp"
#include "utility.hpp"

namespace ft {

template <typename T>
struct Node {
  enum Node_Color { Red, Black };
  Node_Color color;
  struct Node *parent, *left, *right;
  T value;
  Node(const T& val) : color(Red), parent(), left(), right(), value(val) {}
};

template <typename T>
struct BTreeIterator {
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T& reference;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef Node<T> Nt;
  typedef Nt* pointer;

  BTreeIterator(const pointer n) : ptr(n) {}
  T& operator*() {}

  pointer base() { return ptr; }

 private:
  pointer ptr;
};

template <class value_type, class Compare, class Allocator>
class Rbtree {
 public:
  typedef Node<value_type> node_type;
  typedef BTreeIterator<value_type> iterator;
  typedef BTreeIterator<const value_type> const_iterator;

 private:
  node_type* root;
  Compare comp;
  Allocator alloc;

 public:
  explicit Rbtree(const Compare& comp = Compare(),
                  const Allocator& a = Allocator())
      : comp(comp), alloc(a) {}

  node_type* create_node(const value_type& val) {
    node_type* n = alloc.allocate(1);
    alloc.construct(n, node_type(val));
    return n;
  }

  pair<iterator, bool> insert(const value_type& x) {
    if (!this->root) {
      this->root = create_node(x);
      return make_pair(iterator(this->root), true);
    }
    return make_pair(iterator(this->root), false);
  }
};

template <class Key, class T, class Compare = less<Key>,
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
  typedef typename Allocator::template rebind<Node<value_type> >::other
      node_allocator;
  typedef Rbtree<value_type, value_compare, node_allocator> rep_type;

  typedef typename rep_type::node_type node_type;

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
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  rep_type tree;

 public:
  explicit map(const Compare& comp = Compare(), const Allocator& = Allocator())
      : tree(value_compare(comp)) {}
  template <class InputIterator>
  map(InputIterator first, InputIterator last, const Compare& comp = Compare(),
      const Allocator& = Allocator());
  map(const map<Key, T, Compare, Allocator>& x);
  ~map() {}
  map<Key, T, Compare, Allocator>& operator=(
      const map<Key, T, Compare, Allocator>& x);
  // iterators:
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  // capacity:
  bool empty() const;
  size_type size() const;
  size_type max_size() const;
  // 23.3.1.2 element access:
  T& operator[](const key_type& x) {}
  // modifiers:
  pair<iterator, bool> insert(const value_type& x) { return tree.insert(x); }

  iterator insert(iterator position, const value_type& x);
  template <class InputIterator>
  void insert(InputIterator first, InputIterator last);
  void erase(iterator position);
  size_type erase(const key_type& x);
  void erase(iterator first, iterator last);
  void swap(map<Key, T, Compare, Allocator>&);
  void clear();
  // observers:
  key_compare key_comp() const;
  value_compare value_comp() const;
  // 23.3.1.3 map operations:
  iterator find(const key_type& x);
  const_iterator find(const key_type& x) const;
  size_type count(const key_type& x) const;
  iterator lower_bound(const key_type& x);
  const_iterator lower_bound(const key_type& x) const;
  iterator upper_bound(const key_type& x);
  const_iterator upper_bound(const key_type& x) const;
  pair<iterator, iterator> equal_range(const key_type& x);
  pair<const_iterator, const_iterator> equal_range(const key_type& x) const;
};
template <class Key, class T, class Compare, class Allocator>
bool operator==(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator<(const map<Key, T, Compare, Allocator>& x,
               const map<Key, T, Compare, Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator!=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator>(const map<Key, T, Compare, Allocator>& x,
               const map<Key, T, Compare, Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator>=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y);
template <class Key, class T, class Compare, class Allocator>
bool operator<=(const map<Key, T, Compare, Allocator>& x,
                const map<Key, T, Compare, Allocator>& y);
// specialized algorithms:
template <class Key, class T, class Compare, class Allocator>
void swap(map<Key, T, Compare, Allocator>& x,
          map<Key, T, Compare, Allocator>& y);
}  // namespace ft
#endif
