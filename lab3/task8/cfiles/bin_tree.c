#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/bin_tree.h"
#include "../headers/string.h"

#define max_depth(a, b) (((a) > (b)) ? (a) : (b)) 

int init_node_bin_tree(node_bin_tree **node, char *lexem) {
	*node = (node_bin_tree*)malloc(sizeof(node_bin_tree));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->token = (char*)malloc(sizeof(char) * (strlen(lexem) + 1));
	
	if (!(*node)->token) {
		return NO_MEMORY;
	}

	strcpy((*node)->token, lexem);
	(*node)->parent = NULL;
	(*node)->left = NULL;
	(*node)->right = NULL;
	(*node)->num_matches = 1;

	return SUCCESS;
}

int add_bin_lexem(node_bin_tree **add_bin_node, bin_tree **b_tree, char *lexem) {
	int exit_code = 0;

	if (!*b_tree) {
		*b_tree = (bin_tree*)malloc(sizeof(bin_tree));
		exit_code = init_node_bin_tree(&((*b_tree)->root), lexem);
		
		if (exit_code != SUCCESS) {
			return exit_code;
		}
		
		*add_bin_node = (*b_tree)->root;

		return SUCCESS;
	}

	node_bin_tree *node = (*b_tree)->root;
	node_bin_tree *add_node = NULL;
	node_bin_tree *parent = node;
	exit_code = init_node_bin_tree(&add_node, lexem);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	while (node != NULL) {
		parent = node;
		node = node->left;
	}

	parent->left = add_node;
	add_node->parent = parent;
	*add_bin_node = add_node;

	return SUCCESS;
}

node_bin_tree *min(node_bin_tree *root) {
    node_bin_tree *node = root;

    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

node_bin_tree *max(node_bin_tree *root) {
    node_bin_tree *node = root;
    
    while (node->right != NULL){
        node = node->right;
    }

    return node;
}

node_bin_tree *follow_del(node_bin_tree *root) {
    node_bin_tree *node = root;
    node_bin_tree *find_node = NULL;

    if (node->right != NULL)
        return min(node->right);
    
    find_node = node->parent;
    
    while ((find_node != NULL) && (node == find_node -> right)) {
        node = find_node;
        find_node = find_node->parent;
    }
    
    return find_node;
}

node_bin_tree *find_node(node_bin_tree *root, char *lexem) {
	if (root) {
		node_bin_tree *node_left = NULL;
		node_bin_tree *node_right = NULL;
		
		if (strcmp(root->token, lexem) == 0) {
			return root;
		}

		node_left = find_node(root->left, lexem);
		node_right = find_node(root->right, lexem);

		if (!node_left) {
			return node_right;
		} else {
			return node_left;
		}
	}

	return NULL;
}

int add_node_tree(node_bin_tree *add_node, bin_tree **b_tree) {
	if (!*b_tree) {
		*b_tree = (bin_tree*)malloc(sizeof(bin_tree));

		if (!*b_tree) {
			return NO_MEMORY;
		}

		(*b_tree)->root = add_node;

		return SUCCESS;
	}

	node_bin_tree *past = (*b_tree)->root;
	node_bin_tree *past_parent = NULL;

	while (past != NULL) {
		past_parent = past;
		if (add_node->num_matches < past->num_matches) {
			past = past->left;
		} else {
			past = past->right;
		}
	}

	add_node->parent = past_parent;

	if (add_node->num_matches < past_parent->num_matches) {
		past_parent->left = add_node;
	} else {
		past_parent->right = add_node;
	}

	return SUCCESS;
}

void delete_node(node_bin_tree **del_node, bin_tree **b_tree) {
	if (*del_node == (*b_tree)->root) {
		if ((*b_tree)->root->left == NULL && (*b_tree)->root->right == NULL) {
			free(((*b_tree)->root->token));
			free((*b_tree)->root);
		}
		if ((*b_tree)->root->left != NULL && (*b_tree)->root->right == NULL) {
			node_bin_tree *temp = (*b_tree)->root->left;
			free((*b_tree)->root->token);
			free((*b_tree)->root);
			(*b_tree)->root = temp;
		}
		if ((*b_tree)->root->left == NULL && (*b_tree)->root->right != NULL) {
			node_bin_tree *temp = (*b_tree)->root->right;
			free((*b_tree)->root->token);
			free((*b_tree)->root);
			(*b_tree)->root = temp;
		}
		if ((*b_tree)->root->left != NULL && (*b_tree)->root->right != NULL) {
			node_bin_tree *temp = (*b_tree)->root;
			node_bin_tree *root_left = (*b_tree)->root->left;
			(*b_tree)->root = (*b_tree)->root->right;
			add_node_tree(root_left, b_tree);
			free(temp->token);
			free(temp);
		}
	} else {
		if (!(*del_node)->right && !(*del_node)->left) {
			if ((*del_node)->parent->right == (*del_node)) {
				(*del_node)->parent->right = NULL;
			} else {
				(*del_node)->parent->left = NULL;
			}
			free((*del_node)->token);
			free((*del_node));
		}
		if ((*del_node)->left != NULL && (*del_node)->right == NULL) {
			if ((*del_node)->parent->right == (*del_node)) {
				(*del_node)->parent->right = (*del_node)->left;
			} else {
				(*del_node)->parent->left = (*del_node)->left;
			}
			free((*del_node)->token);
			free((*del_node));
		}
		if ((*del_node)->left == NULL && (*del_node)->right != NULL) {
			if ((*del_node)->parent->right == (*del_node)) {
				(*del_node)->parent->right = (*del_node)->right;
			} else {
				(*del_node)->parent->left = (*del_node)->right;
			}
			free((*del_node)->token);
			free((*del_node));
		}
		if ((*del_node)->left != NULL && (*del_node)->right != NULL) {
			node_bin_tree *repl_node = follow_del((*del_node));
			free((*del_node)->token);
			(*del_node)->token = repl_node->token;
			if (repl_node->right == NULL) {
				repl_node->parent->left = NULL;
			} else {
				repl_node->parent->left = repl_node->right;
			}
			free(repl_node);
		}
	}
}

int rebuild_bin_tree(node_bin_tree **inc_node, bin_tree **b_tree) {
	if ((*b_tree)->root->left || (*b_tree)->root->right) {
		node_bin_tree *new_node = NULL;
		int exit_code = init_node_bin_tree(&new_node, (*inc_node)->token);
		new_node->num_matches = (*inc_node)->num_matches;

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		delete_node(inc_node, b_tree);
		exit_code = add_node_tree(new_node, b_tree);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}
}

int depth(node_bin_tree *root) {
	if (!root) {
		return 0;
	}
	return max_depth(depth(root->left), depth(root->right)) + 1;
}

int save_tree_in_file(bin_tree **b_tree, char *filename) {
	FILE *save_file = NULL;

	if ((save_file = fopen(filename, "w")) != NULL) {
		record((*b_tree)->root, save_file);
		fclose(save_file);
		free_bin_tree(&((*b_tree)->root));
		free(*b_tree);
		*b_tree = NULL;
		return SUCCESS;
	}

	return OPEN_FILE;
}

int record(node_bin_tree *root, FILE *save_file) {
	if (root) {
		fprintf(save_file, "%s %d ", root->token, root->num_matches);
		record(root->left, save_file);
		record(root->right, save_file);
	}
}

int unload_tree_from_file(bin_tree **b_tree, char *filename) {
	FILE *read_file = NULL;

	if (*b_tree) {
		free_bin_tree(&((*b_tree)->root));
		free(*b_tree);
	}

	if ((read_file = fopen(filename, "r")) != NULL) {
		if (!*b_tree) {
			fclose(read_file);
			return NO_MEMORY;
		}

		add_tree_from_file(b_tree, read_file);

		return SUCCESS;
	}

	return OPEN_FILE;
}

int add_tree_from_file(bin_tree **b_tree, FILE *read_file) {
	while (!feof(read_file)) {
		char *lexem = NULL;
		char *str_num_matches = NULL;
		int exit_code = get_token(&lexem, read_file, is_bad, is_good);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		exit_code = get_token(&str_num_matches, read_file, is_bad, is_good);
		
		node_bin_tree *add_node = NULL;

		exit_code = init_node_bin_tree(&add_node, lexem);
		add_node->num_matches = atoi(str_num_matches);

		exit_code = add_node_tree(add_node, b_tree);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}
}

void free_bin_tree(node_bin_tree **root) {
	if (*root) {
		if ((*root)->left) {
			free_bin_tree(&((*root)->left));
		}
		if ((*root)->right) {
			free_bin_tree(&((*root)->right));
		}
		free((*root)->token);
		free(*root);
	}
}