#include "node.hpp"
#include "rbt_iterator.hpp"
#include "utility.hpp"

namespace ft {
#pragma once
template <class Key, class value_type, class Compare, class Allocator>
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
  node_type *get_root() { return header.parent; }

  explicit Rbtree(const Compare &comp = Compare(),
                  const Allocator &a = Allocator())
      : count(), comp(comp), alloc(a), header(&header, &header) {}

  node_type *create_node(const value_type &val) {
    node_type *n = alloc.allocate(1);
    alloc.construct(n, node_type(val));
    return n;
  }

  iterator find(const Key &x) {
    node_type *i(header.parent);
    node_type *parent(0);
    bool is_left(true);
    while (i) {
      parent = i;
      is_left = comp(x, i->value);
      i = is_left ? i->left : i->right;
    }
    if (parent && comp(i->value))
      ;
  }

  void erase(iterator position) { node_type *node = position.nodep; }

  iterator insert(iterator position, const value_type &x) {}

  pair<iterator, bool> insert(const value_type &x) {
    node_type *i(header.parent);
    node_type *parent(&header);
    bool is_left(true);
    while (i) {
      parent = i;
      is_left = comp(x, i->value);
      if (!is_left && !comp(i->value, x))
        return pair<iterator, bool>(iterator(parent), false);
      i = is_left ? i->left : i->right;
    }

    node_type *node = create_node(x);
    node->parent = parent;

    if (is_left) {
      parent->left = node;
      if (parent == &header) {
        header.parent = node;
        header.right = node;
      } else if (parent == header.left)
        header.left = node;
    } else {
      parent->right = node;
      if (parent == header.right)
        header.right = node;
    }
    rebalance(node);
    ++count;
    return pair<iterator, bool>(iterator(node), true);
  }

private:
  void right_rotate(node_type *const node) {
    node_type *const save = node->left;
    node->left = save->right;
    if (save->right)
      save->right->parent = node;
    save->parent = node->parent;
    if (node == header.parent)
      header.parent = save;
    else if (node == node->parent->right)
      node->parent->right = save;
    else
      node->parent->left = save;
    save->right = node;
    node->parent = save;
  }

  void left_rotate(node_type *const node) {
    node_type *const save = node->right;
    node->right = save->left;
    if (save->left)
      save->left->parent = node;
    save->parent = node->parent;
    if (node == header.parent)
      header.parent = save;
    else if (node == node->parent->left)
      node->parent->left = save;
    else
      node->parent->right = save;
    save->left = node;
    node->parent = save;
  }

  void rebalance(node_type *node) {
    while (node != header.parent && node->parent->color == node_type::Red) {
      node_type *const gp = node->parent->parent;
      if (node->parent == gp->left) {
        node_type *const u = gp->right;
        if (u && u->color == node_type::Red) {
          node->parent->color = node_type::Black;
          u->color = node_type::Black;
          gp->color = node_type::Red;
          node = gp;
        } else {
          if (node == node->parent->right) {
            node = node->parent;
            left_rotate(node);
          }
          node->parent->color = node_type::Black;
          gp->color = node_type::Red;
          right_rotate(gp);
        }
      } else {
        node_type *const u = gp->left;
        if (u && u->color == node_type::Red) {
          node->parent->color = node_type::Black;
          u->color = node_type::Black;
          gp->color = node_type::Red;
          node = gp;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            right_rotate(node);
          }
          node->parent->color = node_type::Black;
          gp->color = node_type::Red;
          left_rotate(gp);
        }
      }
    }
    header.parent->color = node_type::Black;
  }

public:
  iterator begin() { return iterator(header.left); };
  const_iterator begin() const { return iterator(header.left); }
  iterator end() { return iterator(&header); }
  const_iterator end() const { return iterator(&header); }
  size_t size() const { return count; }
};
} // namespace ft
