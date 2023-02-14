#ifndef MAP_HPP
#define MAP_HPP
#include <iostream>
#include <memory>
#include <utility>

#include "algorithm.hpp"
#include "iterator.hpp"
#include "utility.hpp"

namespace ft {

template <typename T>
struct Node {
  enum Node_Color { Red, Black };
  Node_Color color;
  struct Node *parent, *left, *right;
  T value;

  Node(Node* l, Node* r) : color(Red), parent(), left(l), right(r){};

  Node(const T& val) : color(Red), parent(), left(), right(), value(val) {}

  static Node* mininum(Node* x) {
    while (x->left) x = x->left;
    return x;
  }

  static Node* maximum(Node* x) {
    while (x->right) x = x->right;
    return x;
  }

  static Node* increment(Node* x) {
    if (x->right) return mininum(x->right);
    Node* y(x->parent);
    while (x == y->right) {
      x = y;
      y = y->parent;
    }
    if (x->right != y)
      return y;
    return x;
  }

  static Node* decrement(Node* x) {
    if (x->color == Red && x->parent->parent == x)
      return x->right;
    else if (x->left) return maximum(x->left);
    Node* y(x->parent);
    while (y && x == y->left) {
      x = y;
      y = y->parent;
    }
    return y;
  }
};

template <typename T>
struct BTreeIterator {
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T& reference;
  typedef T* pointer;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef Node<T> Node;

  BTreeIterator(Node* n) : nodep(n) {}
  reference operator*() { return nodep->value; }

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
  typedef T value_type;
  typedef ptrdiff_t difference_type;
  typedef T& reference;
  typedef T* pointer;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef Node<T> Node;
  typedef BTreeIterator<T> iterator;

  ConstBTreeIterator(Node* n) : nodep(n) {}

  ConstBTreeIterator(const iterator& it) : nodep(it.nodep) {}
  reference operator*() { return nodep->value; }

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

template <class value_type, class Compare, class Allocator>
class Rbtree {
 public:
  typedef Node<value_type> node_type;
  typedef BTreeIterator<value_type> iterator;
  typedef ConstBTreeIterator<value_type> const_iterator;

 private:
  size_t count;
  Compare comp;
  Allocator alloc;
  node_type header;

 public:
  explicit Rbtree(const Compare& comp = Compare(),
                  const Allocator& a = Allocator())
      : count(), comp(comp), alloc(a), header(&header, &header) {}

  node_type* create_node(const value_type& val) {
    node_type* n = alloc.allocate(1);
    alloc.construct(n, node_type(val));
    return n;
  }

  pair<iterator, bool> insert(const value_type& x) {
    node_type* i(header.parent);
    node_type* parent(0);

    while (i) {
      parent = i;
      if (comp(x, i->value))
        i = i->left;
      else if (comp(i->value, x))
        i = i->right;
      else
        return pair<iterator, bool>(iterator(i), false);
    }
    node_type* node = create_node(x);
    if (!parent) {
      node->parent = &header;
      node->color = node_type::Black;
      header.parent = header.right = header.left = node;
      return pair<iterator, bool>(iterator(node), true);
    }
    if (comp(x, parent->value)) {
      parent->left = node;
      if (parent == header.left) header.left = node;
    } else {
      parent->right = node;
      if (parent == header.right) header.right = node;
    }
    node->parent = parent;
    ++count;
    return pair<iterator, bool>(iterator(node), true);
  }
  iterator begin() { return iterator(header.left); };
  const_iterator begin() const { return iterator(header.left); }
  iterator end() { return iterator(&header); }
  const_iterator end() const { return iterator(&header); }
  size_t size() const { return count; }
};

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
