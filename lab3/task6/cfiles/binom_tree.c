#include <stdlib.h>
#include <stdio.h>
#include "../headers/binom_tree.h"

int init_root_tree(binom_tree **tree, mail *item) {
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

int merge(binom_tree **res_merge, binom_tree *tree_1, binom_tree *tree_2, int (*cmp)(mail*, mail*)) {
	if (!tree_1) {
		*res_merge = tree_2;
	} else {
		if (!tree_2) {
			*res_merge = tree_1;
		} else {
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
	}
}

void free_bin_tree(node_tree **root) {
	if (*root) {
		for (int i = 0; i < (*root)->degree; i++) {
			free_bin_tree(&((*root)->childs[i]));
		}
		if ((*root)->childs) {
			free((*root)->childs);
		}
		free(*root);
	}
}