#ifndef BINOM_TREE
#define BINOM_TREE

#include "types.h"

int init_root_tree(binom_tree**, mail*);
int merge(binom_tree**, binom_tree*, binom_tree*, int (*cmp)(mail*, mail*));
void free_bin_tree(node_tree**);

#endif // BINOM_TREE