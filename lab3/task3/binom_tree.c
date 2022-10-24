#include <stdio.h>
#include <stdlib.h>
#include "binom_tree.h"

binom_tree *init_root_tree(employee *key) {
	binom_tree *tree = (binom_tree*)malloc(sizeof(binom_tree));
	tree->root = (Node_tree*)malloc(sizeof(Node_tree));
	tree->root->key = key;
	tree->root->childs = NULL;
	tree->root->degree = 0;
	tree->root->length = 0;

	return tree;
}

binom_tree *merge(binom_tree *tree_1, binom_tree *tree_2, int (*cmp)(employee*, employee*)) {
	if (cmp(tree_1->root->key, tree_2->root->key)) {
		if (tree_1->root->degree == tree_1->root->length) {
			tree_1->root->length =  tree_1->root->length ? tree_1->root->length * 2 : 2;
			tree_1->root->childs = (Node_tree**)realloc(tree_1->root->childs, sizeof(Node_tree*) * tree_1->root->length);
		}
		
		tree_1->root->childs[tree_1->root->degree++] = tree_2->root;
		
		return tree_1;
	}

	if (tree_2->root->degree == tree_2->root->length) {
		tree_2->root->length =  tree_2->root->length ? tree_2->root->length * 2 : 2;
		tree_2->root->childs = (Node_tree**)realloc(tree_2->root->childs, sizeof(Node_tree*) * tree_2->root->length);
	}

	tree_2->root->childs[tree_2->root->degree++] = tree_1->root;

	return tree_2;
}