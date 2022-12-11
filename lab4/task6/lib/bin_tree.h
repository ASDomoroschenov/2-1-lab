#ifndef BIN_TREE_EXP_H
#define BIN_TREE_EXP_H

#include "types.h"
#include "stack.h"
#include "check.h"
#include "string.h"
#include "expression.h"

int init_node_bin_tree(node_bin_tree**, char*);
int make_bin_tree_exp(bin_tree**, char*);
void free_node(node_bin_tree**);
void free_tree_nodes(node_bin_tree**);
void free_bin_tree(bin_tree**);

#endif // BIN_TREE_EXP_H