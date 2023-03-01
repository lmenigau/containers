#pragma once

namespace ft {
struct Node {
  enum Node_Color { Red = false, Black = true };
  Node_Color color;
  struct Node *parent, *left, *right;

  Node() : color(Red), parent(0), left(0), right(0) {}
  Node(Node* l, Node* r) : color(Red), parent(0), left(l), right(r){};
  // Node(const Node& o)
  ////    : color(o.Red), parent(o.parent), left(o.left), right(o.right){};

  static Node* minimum(Node* x) {
    while (x->left)
      x = x->left;
    return x;
  }

  static Node* maximum(Node* x) {
    while (x->right)
      x = x->right;
    return x;
  }

  static Node* increment(Node* x) {
    if (x->right)
      return minimum(x->right);
    Node* y(x->parent);
    while (x == y->right) {
      x = y;
      y = y->parent;
    }
    if (x->right == y)
      return x;
    return y;
  }

  static Node* decrement(Node* x) {
    if (x->color == Red && x->parent->parent == x)
      return x->right;
    else if (x->left)
      return maximum(x->left);
    Node* y(x->parent);
    while (y && x == y->left) {
      x = y;
      y = y->parent;
    }
    return y;
  }
};

template <typename T>
struct Node_Val : Node {
  T value;
  Node_Val(const Node_Val& o) : Node(o), value(o.value) {}
  Node_Val(const T& val) : Node(), value(val) {}
};
}  // namespace ft
