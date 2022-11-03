#ifndef PREF_TREE_H
#define PREF_TREE_H

#include "types.h"

int init_node_pref_tree(node_pref_tree**, char);
int binary_search(node_pref_tree**, int, char);
int have_lexem(pref_tree*, char*);
void set_ptr_lexem(bin_tree*, pref_tree**, char*);
int cmp(const void*, const void*);
int add_pref_lexem(node_bin_tree*, pref_tree**, char*);
void free_pref_tree(node_pref_tree**);

#endif // PREF_TREE_H