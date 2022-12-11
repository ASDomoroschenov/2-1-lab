#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/bin_tree.h"

int init_node_bin_tree(node_bin_tree **node, char *token) {
	*node = (node_bin_tree*)malloc(sizeof(node_bin_tree));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->data = (char*)malloc(sizeof(char) * (strlen(token) + 1));

	if (!(*node)->data) {
		free(*node);
		*node = NULL;

		return NO_MEMORY;
	}

	strcpy((*node)->data, token);

	(*node)->left = NULL;
	(*node)->right = NULL;

	return SUCCESS;
}

int make_bin_tree_exp(bin_tree **tree_exp, char *str) {
	stack_t *stack = NULL;
	char *postfix = NULL;
	int exit_code = to_postfix(str, &postfix);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int len = strlen(postfix);
	node_bin_tree *node = NULL;
	char *token = NULL;

	for (int i = 0; i < len; i++) {
		if (!isspace(postfix[i])) {
			if (postfix[i] == '-' ||
				postfix[i] == '<' ||
				postfix[i] == '+') {

				exit_code = add_chr(&token, 2, postfix[i], postfix[i + 1]);
				i++;
				
				if (exit_code != SUCCESS) {
					free_stack(&stack);

					return exit_code;
				}
			} else {
				exit_code = add_chr(&token, 1, postfix[i]);

				if (exit_code != SUCCESS) {
					free_stack(&stack);

					return exit_code;
				}
			}

			if (is_operand(token)) {
				exit_code = init_node_bin_tree(&node, token);

				if (exit_code != SUCCESS) {
					free_stack(&stack);
					free(postfix);

					return exit_code; 
				}

				exit_code = push(&stack, node, NODE_BIN_TREE);

				if (exit_code != SUCCESS) {
					free_stack(&stack);
					free(postfix);

					return exit_code;
				}
			} else {
				if (is_operator(token)) {
					exit_code = init_node_bin_tree(&node, token);

					if (exit_code != SUCCESS) {
						free_stack(&stack);
						free(postfix);

						return exit_code; 
					}

					if (is_unary(token, 0)) {
						node->left = (node_bin_tree*)pick(stack);
						node->right = NULL;
						pop(&stack);
					} else {
						node->right = (node_bin_tree*)pick(stack);
						pop(&stack);
						node->left = (node_bin_tree*)pick(stack);
						pop(&stack);
					}

					exit_code = push(&stack, node, NODE_BIN_TREE);
					
					if (exit_code != SUCCESS) {
						free_stack(&stack);
						free(postfix);

						return exit_code;
					}
				}
			}

			free(token);
			token = NULL;
		}
	}

	*tree_exp = (bin_tree*)malloc(sizeof(bin_tree));

	if (!*tree_exp) {
		return NO_MEMORY;
	}

	(*tree_exp)->root = (node_bin_tree*)pick(stack);

	free_stack(&stack);
	free(postfix);

	return SUCCESS;
}

void free_node(node_bin_tree **node) {
	if (*node) {
		if ((*node)->data) {
			free((*node)->data);
			(*node)->data = NULL;
		}
		free(*node);
		*node = NULL;
	}
}

void free_tree_nodes(node_bin_tree **root) {
	if (*root) {
		if ((*root)->left) {
			free_tree_nodes(&((*root)->left));
		}
		if ((*root)->right) {
			free_tree_nodes(&((*root)->right));
		}
		free_node(root);
	}
}

void free_bin_tree(bin_tree **tree) {
	free_tree_nodes(&((*tree)->root));
	free(*tree);
	*tree = NULL;
}