#pragma once

#include "node.hpp"
#include "utility.hpp"

#include <fstream>
#include <sstream>

template <class T>
void bst_print_null(std::string const& lr,
                    ft::Node_Val<T>* node,
                    std::ostream& os) {
  os << lr << node->value.first << "[style=invis];\n";
  os << node->value.first << " -> " << lr << node->value.first
     << "[style=invis];\n";
}

template <class T>
void bst_print_dot_aux(ft::Node_Val<T>* node, std::ostream& os) {
  if (node->color == ft::Node::Red)
    os << node->value.first << "[color=red];\n";
  if (node->left) {
    os << node->value.first << " -> "
       << ((ft::Node_Val<T>*)node->left)->value.first << ";\n";
    bst_print_dot_aux((ft::Node_Val<T>*)node->left, os);
  } else
    bst_print_null("l", node, os);
  if (node->right) {
    os << node->value.first << " -> "
       << ((ft::Node_Val<T>*)node->right)->value.first << ";\n";
    bst_print_dot_aux((ft::Node_Val<T>*)node->right, os);
  } else
    bst_print_null("r", node, os);
}

template <class Node>
void bst_print_dot(Node* tree, long n = 0) {
  std::ostringstream conv;
  conv << n;
  std::ofstream stream(("tree" + conv.str() + ".gv").c_str());

  stream << "digraph RBT {\n"
            "  nodesep=0.1;\n"
            "  ranksep=0.3;\n"
            "  margin=0.3;\n"
            "  node [shape=ellipse, fontname=Monospace];\n"
            "  edge [arrowsize=2];\n"
            "  node [style=filled, shape=circle];\n"
            "  edge [arrowsize=0.8]\n"
            "  subgraph main {\n";

  if (!tree)
    stream << '\n';
  else if (!tree->right && !tree->left)
    stream << tree->value.first << ";\n" << tree->value.first;
  else
    bst_print_dot_aux(tree, stream);

  stream << "}\n"
            "}\n";
}
