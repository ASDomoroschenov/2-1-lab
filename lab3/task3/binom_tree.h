#ifndef BINOM_TREE
#define BINOM_TREE

#include "employee.h"

typedef struct Node {
	struct Node **childs;
	size_t length;
	size_t degree;
	employee *key;
} Node_tree;

typedef struct {
	Node_tree *root;
} binom_tree;

binom_tree *init_root_tree(employee*);
binom_tree *merge(binom_tree*, binom_tree*, int (*cmp)(employee*, employee*));

#endif // BINOM_TREE