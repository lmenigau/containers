#pragma once

#include "node.hpp"
#include "utility.hpp"

#include <ostream>

typedef ft::Node<ft::pair<const int, int> > bst_node;
void bst_print_dot(bst_node* tree, std::ostream& stream);
