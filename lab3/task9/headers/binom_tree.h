#ifndef BINOM_TREE
#define BINOM_TREE

#include "types.h"

int init_root_tree(binom_tree**, T*);
T *get_max_tree(node_tree*);
int merge(binom_tree**, binom_tree*, binom_tree*, int (*cmp)(T*, T*));
void free_bin_tree(node_tree**);

#endif // BINOM_TREE