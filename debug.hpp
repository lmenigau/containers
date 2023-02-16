#pragma once
#include <bits/types/FILE.h>

#include <cstdio>

#include "map.hpp"

typedef ft::Node<ft::pair<const int, int> > bst_node;

void bst_print_dot_null(int key, int nullcount, FILE* stream) {
  // fprintf(stream, "    null%d [shape=point];\n", nullcount);
  // fprintf(stream, "    %d -> null%d;\n", key, nullcount);
}

void bst_print_dot_aux(bst_node* node, FILE* stream) {
  static int nullcount = 0;

  if (node->left) {
    if (node->color == bst_node::Red)
      fprintf(stream, "    %d [color=red] ;\n", node->value.first);
    fprintf(stream, "    %d -> %d ;\n", node->value.first,
            node->left->value.first);
    bst_print_dot_aux(node->left, stream);
  } else
    bst_print_dot_null(node->value.first, nullcount++, stream);

  if (node->right) {
    fprintf(stream, "    %d -> %d;\n", node->value.first,
            node->right->value.first);
    bst_print_dot_aux(node->right, stream);
  } else
    bst_print_dot_null(node->value.first, nullcount++, stream);
}

void bst_print_dot(bst_node* tree, FILE* stream) {
  fprintf(stream, "digraph BST {\n");
  fprintf(stream, "node [style=\"filled\"];");
  fprintf(stream, "    node [fontname=\"Arial\"];\n");

  if (!tree)
    fprintf(stream, "\n");
  else if (!tree->right && !tree->left)
    fprintf(stream, "    %d;\n", tree->value.first);
  else
    bst_print_dot_aux(tree, stream);

  fprintf(stream, "}\n");
}
