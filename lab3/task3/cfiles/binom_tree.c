#include <stdlib.h>
#include "../headers/binom_tree.h"

int init_root_tree(binom_tree **tree, employee *item) {
	*tree = (binom_tree*)malloc(sizeof(binom_tree));
	if (*tree) {
		(*tree)->root = (node_tree*)malloc(sizeof(node_tree));
		if ((*tree)->root) {
			(*tree)->root->key = item;
			(*tree)->root->childs = NULL;
			(*tree)->root->length = 0;
			(*tree)->root->degree = 0;

			return SUCCESS;
		} else {
			return NO_MEMORY;
		}
	} else {
		return NO_MEMORY;
	}
}

//cmp возвращает 1, если 1 аргумент больше/меньше 2

int merge(binom_tree **res_merge, binom_tree *tree_1, binom_tree *tree_2, int (*cmp)(employee*, employee*)) {
	if (cmp(tree_1->root->key, tree_2->root->key)) {
		*res_merge = tree_1;
		if ((*res_merge)->root->degree == (*res_merge)->root->length) {
			(*res_merge)->root->length =  (*res_merge)->root->length ? (*res_merge)->root->length * 2 : 2;
			(*res_merge)->root->childs = (node_tree**)realloc((*res_merge)->root->childs, sizeof(node_tree*) * (*res_merge)->root->length);
			if (!((*res_merge)->root->childs)) {
				return NO_MEMORY;
			}
		}
		(*res_merge)->root->childs[(*res_merge)->root->degree++] = tree_2->root;
		free(tree_2);
	} else {
		*res_merge = tree_2;
		if ((*res_merge)->root->degree == (*res_merge)->root->length) {
			(*res_merge)->root->length =  (*res_merge)->root->length ? (*res_merge)->root->length * 2 : 2;
			(*res_merge)->root->childs = (node_tree**)realloc((*res_merge)->root->childs, sizeof(node_tree*) * (*res_merge)->root->length);
			if (!((*res_merge)->root->childs)) {
				return NO_MEMORY;
			}
		}
		(*res_merge)->root->childs[(*res_merge)->root->degree++] = tree_1->root;
		free(tree_1);
	}
}