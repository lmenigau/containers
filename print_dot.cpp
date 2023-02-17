#include "print_dot.hpp"

void bst_print_dot_aux(bst_node* node, std::ostream& stream) {
  if (node->left) {
    if (node->color == bst_node::Red)
      stream << node->value.first << "[color=red];\n";
    stream << node->value.first << " -> " << node->left->value.first << ";\n";
    bst_print_dot_aux(node->left, stream);
  }
  if (node->right) {
    stream << node->value.first << " -> " << node->right->value.first << ";\n";
    bst_print_dot_aux(node->right, stream);
  }
}

void bst_print_dot(bst_node* tree, std::ostream& stream) {
  stream << "digraph BST {\n";
  stream <<   "node [style=\"filled\"];\n";
  stream <<  "    node [fontname=\"Arial\"];\n";

  if (!tree)
    stream << '\n';
  else if (!tree->right && !tree->left)
    stream << tree->value.first << ";\n" << tree->value.first;
  else
    bst_print_dot_aux(tree, stream);

  stream << "}\n";
}
