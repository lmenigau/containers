#pragma once

namespace ft {
template <typename T>
struct Node {
  enum Node_Color { Red = false, Black = true };
  Node_Color color;
  struct Node *parent, *left, *right;
  T value;

  Node(Node* l, Node* r) : color(Red), parent(0), left(l), right(r){};

  Node(const T& val) : color(Red), parent(0), left(0), right(0), value(val) {}

  static Node* mininum(Node* x) {
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
      return mininum(x->right);
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
}  // namespace ft
