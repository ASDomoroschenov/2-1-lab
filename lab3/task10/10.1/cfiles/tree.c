#include <stdlib.h>
#include <stdio.h>
#include "../headers/tree.h"
#include "../headers/stack.h"

int init_tree(Tree **tree, char c) {
	if (!*tree) {
		*tree = (Tree*)malloc(sizeof(Tree));

		if (!*tree) {
			return NO_MEMORY;
		}

		(*tree)->root = NULL;
	}

	int exit_code = init_node_tree(&((*tree)->root), c);

	if (exit_code != SUCCESS) {
		free(*tree);
		*tree = NULL;

		return exit_code;
	}

	return SUCCESS;
}

int init_node_tree(node_tree **init_node, char c) {
	if (*init_node) {
		free(*init_node);
		*init_node = NULL;
	}

	*init_node = (node_tree*)malloc(sizeof(node_tree));

	if (!*init_node) {
		return NO_MEMORY;
	}

	(*init_node)->subtrees = NULL;
	(*init_node)->key_tree = c;
	(*init_node)->curr_size = 0;
	(*init_node)->size = 0;

	return SUCCESS;
}

int add_node_subtree(node_tree **add_node, char c) {
	if (*add_node) {
		if ((*add_node)->size == (*add_node)->curr_size) {
			(*add_node)->size = (*add_node)->size ? (*add_node)->size * 2 : 1;

			node_tree **temp = (node_tree**)realloc((*add_node)->subtrees, sizeof(node_tree*) * (*add_node)->size);

			if (!temp) {
				return NO_MEMORY;
			}

			(*add_node)->subtrees = temp;
		}

		node_tree *node = NULL;
		int exit_code = init_node_tree(&node, c);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		(*add_node)->subtrees[(*add_node)->curr_size++] = node;
	}

	return SUCCESS;
}

int make_tree(Tree **tree, char *str) {
	Stack *stack = NULL;
	char *ptr = str;
	int exit_code = init_tree(tree, *ptr);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	node_tree *node = (*tree)->root;

	while (*++ptr) {
		if (*ptr == ',') {
			ptr++;
		}
		if (*ptr == '(') {
			exit_code = add_stack(&stack, node);
			
			if (exit_code != SUCCESS) {
				free_tree(tree);
				free_stack(&stack);

				return exit_code;
			}
		} else {
			if (*ptr == ')') {
				pop_stack(&stack);
				
				if (stack) {
					node = stack->top->key_stack;
				}
			} else {
				if (stack) {
					exit_code = add_node_subtree(&(stack->top->key_stack), *ptr);
					
					if (exit_code != SUCCESS) {
						free_tree(tree);
						free_stack(&stack);
						
						return exit_code;
					}
				}
			}
		}
	}

	return SUCCESS;
}

void output_tree(node_tree *root, FILE *output) {
	if (root) {
		fprintf(output, "%c ", root->key_tree);
		if (root->subtrees) {
			for (int i = 0; i < root->curr_size; i++) {
				output_tree(root->subtrees[i], output);
			}
		}
	}
}

void free_tree_nodes(node_tree **root) {
	if (*root) {
		if ((*root)->subtrees) {
			for (int i = 0; i < (*root)->curr_size; i++) {
				free_tree_nodes(&((*root)->subtrees[i]));
			}
			free((*root)->subtrees);
			(*root)->subtrees = NULL;
		}
		free(*root);
		*root = NULL;
	}
}

void free_tree(Tree **tree) {
	if (*tree) {
		free_tree_nodes(&((*tree)->root));
		free(*tree);
		*tree = NULL;
	}
}