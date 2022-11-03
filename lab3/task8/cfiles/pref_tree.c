#include <stdlib.h>
#include <stdio.h>
#include "../headers/pref_tree.h"
#include "../headers/bin_tree.h"

int init_node_pref_tree(node_pref_tree **node, char c) {
	*node = (node_pref_tree*)malloc(sizeof(node_pref_tree));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->subtrees = NULL;
	(*node)->curr_size = 0;
	(*node)->size = 0;
	(*node)->lexeme_char = c;
	(*node)->lexem = NULL;

	return SUCCESS;
}

int binary_search(node_pref_tree **subtrees, int curr_size, char c) {
	if (subtrees) {
		int left = 0;
		int right = curr_size - 1;

		while (left <= right) {
			int middle = (left + right) >> 1;
			char middle_value = subtrees[middle]->lexeme_char;

			if (middle_value < c) {
				left = middle + 1;
			} else {
				if (middle_value > c) {
					right = middle - 1;
				} else {
					return middle;
				}
			}
		}
		
		return -1;
	}

	return -1;
}

int have_lexem(pref_tree *p_tree, char *token) {
	if (!p_tree) {
		return 0;
	}

	node_pref_tree *node = p_tree->root;
	char *ptr = token - 1;
	int index = 0;

	while (*++ptr) {
		index = binary_search(node->subtrees, node->curr_size, *ptr);
		if (index != -1) {
			node = node->subtrees[index];
		} else {
			return 0;
		}
	}

	return node->lexem ? 1 : 0;
}

void set_ptr_lexem(bin_tree *b_tree, pref_tree **p_tree, char *token) {
	node_pref_tree *node = (*p_tree)->root;
	char *ptr = token - 1;
	int index = 0;

	while (*++ptr) {
		index = binary_search(node->subtrees, node->curr_size, *ptr);
		node = node->subtrees[index];
	}
	node->lexem = find_node(b_tree->root, token);
}

int cmp(const void *item_1, const void *item_2) {
	return (*((node_pref_tree**)(item_1)))->lexeme_char > (*((node_pref_tree**)(item_2)))->lexeme_char;
}

int add_pref_lexem(node_bin_tree *add_node, pref_tree **p_tree, char *lexem) {
	int exit_code = 0;	

	if (!*p_tree) {
		*p_tree = (pref_tree*)malloc(sizeof(pref_tree));

		if (!*p_tree) {
			return NO_MEMORY;
		}

		exit_code = init_node_pref_tree(&((*p_tree)->root), 0);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}

	node_pref_tree *node = (*p_tree)->root;
	char *ptr = lexem - 1;
	int index = 0;

	while (*++ptr) {
		index = binary_search(node->subtrees, node->curr_size, *ptr);
		if (index != -1) {
			node = node->subtrees[index];
		} else {
			if (node->curr_size == node->size) {
				node->size = node->size ? node->size * 2 : 1;
				node_pref_tree **temp = (node_pref_tree**)realloc(node->subtrees, sizeof(node_pref_tree*) * node->size);
			
				if (!temp) {
					return NO_MEMORY;
				}

				node->subtrees = temp;
			}

			exit_code = init_node_pref_tree(&(node->subtrees[node->curr_size]), *ptr);

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			node_pref_tree **subtrees = node->subtrees;
			int count_subtrees = node->curr_size + 1;
			node = node->subtrees[node->curr_size++];
			qsort(subtrees, count_subtrees, sizeof(node_pref_tree*), cmp);
		}
	}

	node->lexem = add_node;

	return SUCCESS;
}

void free_pref_tree(node_pref_tree **root) {
	if (*root) {
		for (int i = 0; i < (*root)->curr_size; i++) {
			free_pref_tree(&((*root)->subtrees[i]));
		}
		free((*root)->subtrees);
		free(*root);
	}
}