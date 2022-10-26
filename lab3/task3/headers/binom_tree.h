#ifndef BINOM_TREE
#define BINOM_TREE

#include "employee.h"

typedef struct Node {
	struct Node **childs;
	size_t length;
	size_t degree;
	employee *key;
} node_tree;

typedef struct {
	node_tree *root;
} binom_tree;

int init_root_tree(binom_tree**, employee*);
int merge(binom_tree**, binom_tree*, binom_tree*, int (*cmp)(employee*, employee*));

#endif // BINOM_TREE