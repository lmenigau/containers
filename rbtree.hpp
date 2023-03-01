#include <iostream>
#include <map>
#include "node.hpp"
#include "rbt_iterator.hpp"
#include "utility.hpp"

namespace ft {
#pragma once
template <class Key, class value_type, class Compare, class Allocator>
class Rbtree {
 public:
  typedef Node_Val<value_type> Node_Val;
  typedef BTreeIterator<value_type> iterator;
  typedef ConstBTreeIterator<value_type> const_iterator;

 private:
  typedef typename value_type::second_type mapped_t;
  size_t count;
  Compare comp;
  Allocator alloc;
  Node header;

 public:
  Node_Val* get_root() { return (Node_Val*)(header.parent); }

  Rbtree(const Compare& comp, const Allocator& a = Allocator())
      : count(), comp(comp), alloc(a), header(&header, &header) {}

  Rbtree(const Rbtree& other) : comp(other.comp) { *this = other; }

  void swap(Rbtree& o) {
    std::swap(count, o.count);
    std::swap(comp, o.comp);
    std::swap(alloc, o.alloc);
    std::swap(header, o.header);
  }

  Node* copy(Node* orig, Node* parent) {
    if (!orig)
      return 0;
    Node* root = create_node(static_cast<Node_Val*>(orig)->value);
    root->color = orig->color;
    root->left = copy(orig->left, root);
    root->right = copy(orig->right, root);
    root->parent = parent;
    return root;
  }

  Rbtree& operator=(const Rbtree& src) {
    if (this == &src)
      return *this;
    if (header.parent)
      clear();
    count = src.count;
    comp = src.comp;
    alloc = src.alloc;
    header.parent = copy(src.header.parent, &header);
    if (header.parent) {
      header.left = Node::minimum(header.parent);
      header.right = Node::maximum(header.parent);
    } else
      header = Node(&header, &header);
    return *this;
  }

  ~Rbtree() { clear(); }
  Node_Val* create_node(const value_type& val) {
    Node_Val* n = alloc.allocate(1);
    alloc.construct(n, Node_Val(val));
    return n;
  }

  void destroy_node(Node_Val* node) {
    alloc.destroy(node);
    --count;
    alloc.deallocate(node, 1);
  }

  void destroy_all(Node* node) {
    if (!node)
      return;
    destroy_all(node->left);
    destroy_all(node->right);
    destroy_node(static_cast<Node_Val*>(node));
  }

  void clear() {
    destroy_all(header.parent);
    header = Node(&header, &header);
  };

  iterator lower_bound(const Key& x) const {
    Node* i(header.parent);
    iterator p(&header);
    while (i) {
      bool cond =
          comp(static_cast<Node_Val*>(i)->value, value_type(x, mapped_t()));
      if (!cond) {
        p = i;
        i = i->left;
      } else {
        i = i->right;
      }
    }
    return p;
  }

  iterator upper_bound(const Key& x) const {
    Node* i(header.parent);
    iterator p(&header);
    while (i) {
      bool cond =
          comp(value_type(x, mapped_t()), static_cast<Node_Val*>(i)->value);
      if (cond) {
        p = i;
        i = i->left;
      } else {
        i = i->right;
      }
    }
    return p;
  }

  iterator find(const Key& x) const {
    Node* i((Node_Val*)header.parent);
    bool is_left(true);
    while (i) {
      is_left =
          comp(value_type(x, mapped_t()), static_cast<Node_Val*>(i)->value);
      if (!is_left && !comp(((Node_Val*)i)->value, value_type(x, mapped_t())))
        return iterator(i);
      i = is_left ? i->left : i->right;
    }
    return (iterator(&header));
  }

  void rebalance_erase(Node* x_parent, Node* x, const Node* const y) {
    typedef Node Color;
    if (y->color != Color::Red) {
      while (x != header.parent && (x == 0 || x->color == Color::Black))
        if (x == x_parent->left) {
          Node* w = x_parent->right;
          if (w->color == Color::Red) {
            w->color = Color::Black;
            x_parent->color = Color::Red;
            left_rotate(x_parent);
            w = x_parent->right;
          }
          if ((w->left == 0 || w->left->color == Color::Black) &&
              (w->right == 0 || w->right->color == Color::Black)) {
            w->color = Color::Red;
            x = x_parent;
            x_parent = x_parent->parent;
          } else {
            if (w->right == 0 || w->right->color == Color::Black) {
              w->left->color = Color::Black;
              w->color = Color::Red;
              right_rotate(w);
              w = x_parent->right;
            }
            w->color = x_parent->color;
            x_parent->color = Color::Black;
            if (w->right)
              w->right->color = Color::Black;
            left_rotate(x_parent);
            break;
          }
        } else {
          // same as above, with right <-> left.
          Node* w = x_parent->left;
          if (w->color == Color::Red) {
            w->color = Color::Black;
            x_parent->color = Color::Red;
            right_rotate(x_parent);
            w = x_parent->left;
          }
          if ((w->right == 0 || w->right->color == Color::Black) &&
              (w->left == 0 || w->left->color == Color::Black)) {
            w->color = Color::Red;
            x = x_parent;
            x_parent = x_parent->parent;
          } else {
            if (w->left == 0 || w->left->color == Color::Black) {
              w->right->color = Color::Black;
              w->color = Color::Red;
              left_rotate(w);
              w = x_parent->left;
            }
            w->color = x_parent->color;
            x_parent->color = Color::Black;
            if (w->left)
              w->left->color = Color::Black;
            right_rotate(x_parent);
            break;
          }
        }
    }
    if (x)
      x->color = Color::Black;
  }

  Node* erase(iterator position) {
    Node* const z = position.nodep;
    Node* y = z;
    Node* child = 0;
    Node* x_parent = 0;

    if (y->left == 0)        // z has at most one non-null child. y == z.
      child = y->right;      // x might be null.
    else if (y->right == 0)  // z has exactly one non-null child. y == z.
      child = y->left;       // x is not null.
    else {
      // z has two non-null children.  Set y to
      y = y->right;  //   z's successor.  x might be null.
      while (y->left != 0)
        y = y->left;
      child = y->right;
    }
    if (y != z) {
      // relink y in place of z.  y is z's successor
      z->left->parent = y;
      y->left = z->left;
      if (y != z->right) {
        x_parent = y->parent;
        if (child)
          child->parent = y->parent;
        y->parent->left = child;  // y must be a child of left
        y->right = z->right;
        z->right->parent = y;
      } else
        x_parent = y;
      if (header.parent == z)
        header.parent = y;
      else if (z->parent->left == z)
        z->parent->left = y;
      else
        z->parent->right = y;
      y->parent = z->parent;
      std::swap(y->color, z->color);
      y = z;
      // y now points to node to be actually deleted
    } else {  // y == z
      x_parent = y->parent;
      if (child)
        child->parent = y->parent;
      if (header.parent == z)
        header.parent = child;
      else if (z->parent->left == z)
        z->parent->left = child;
      else
        z->parent->right = child;
      if (header.left == z) {
        if (z->right == 0)  // z->left must be null also
          header.left = z->parent;
        // makes header.left == header if z == header.parent
        else
          header.left = Node::minimum(child);
      }
      if (header.right == z) {
        if (z->left == 0)  // z->right must be null also
          header.right = z->parent;
        // makes header.right == header if z == header.parent
        else  // x == z->left
          header.right = Node::maximum(child);
      }
    }
    rebalance_erase(x_parent, child, y);
    destroy_node(static_cast<Node_Val*>(y));
    return y;
  }

  iterator insert(bool is_left, Node* parent, const value_type& x) {
    Node_Val* node = create_node(x);
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
    rebalance_insert(node);
    ++count;
    return (iterator(node));
  }

  pair<iterator, bool> insert(const value_type& x) {
    Node* i(header.parent);
    Node* parent(&header);
    bool is_left(true);
    while (i) {
      parent = i;
      is_left = comp(x, ((Node_Val*)i)->value);
      if (!is_left && !comp(((Node_Val*)i)->value, x))
        return ft::make_pair(iterator(i), false);
      i = is_left ? i->left : i->right;
    }
    return ft::make_pair(insert(is_left, parent, x), true);
  }

  iterator insert(iterator pos, const value_type& x) {
    if (!pos.nodep->left && comp(x, *pos))
      return insert(true, pos.nodep, x);
    if (!pos.nodep->right && comp(x, *pos))
      return insert(false, pos.nodep, x);
    return insert(x).first;
  }

 private:
  void right_rotate(Node* const node) {
    Node* const save = node->left;
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

  void left_rotate(Node* const node) {
    Node* const save = node->right;
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

  void rebalance_insert(Node* node) {
    while (node != header.parent && node->parent->color == Node::Red) {
      Node* const gp = node->parent->parent;
      if (node->parent == gp->left) {
        Node* const u = gp->right;
        if (u && u->color == Node::Red) {
          node->parent->color = Node::Black;
          u->color = Node::Black;
          gp->color = Node::Red;
          node = gp;
        } else {
          if (node == node->parent->right) {
            node = node->parent;
            left_rotate(node);
          }
          node->parent->color = Node::Black;
          gp->color = Node::Red;
          right_rotate(gp);
        }
      } else {
        Node* const u = gp->left;
        if (u && u->color == Node::Red) {
          node->parent->color = Node::Black;
          u->color = Node::Black;
          gp->color = Node::Red;
          node = gp;
        } else {
          if (node == node->parent->left) {
            node = node->parent;
            right_rotate(node);
          }
          node->parent->color = Node::Black;
          gp->color = Node::Red;
          left_rotate(gp);
        }
      }
    }
    header.parent->color = Node::Black;
  }

 public:
  iterator begin() { return iterator(header.left); };
  const_iterator begin() const { return const_iterator(header.left); }
  iterator end() { return iterator(&header); }
  const_iterator end() const { return const_iterator(&header); }
  size_t size() const { return count; }
  size_t max_size() const { return alloc.max_size(); }
};
}  // namespace ft
