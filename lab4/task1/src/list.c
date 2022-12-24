#include <stdlib.h>
#include <string.h>
#include "../lib/list.h"

int create_list(list_t **list) {
	*list = (list_t*)malloc(sizeof(list_t));

	if (!*list) {
		return NO_MEMORY;
	}

	(*list)->root = NULL;
	(*list)->size = 0;

	return SUCCESS;
}

int create_node_list(peasant *user, node_list **node) {
	*node = (node_list*)malloc(sizeof(node_list));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->user = user;

	return SUCCESS;
}

int push_back(list_t **list, node_list *add_node) {
	node_list *node = (*list)->root;
	add_node->next = NULL;

	if (!(*list)->root) {
		(*list)->root = add_node;

		return SUCCESS;
	}

	while (node->next) {
		node = node->next;
	}

	node->next = add_node;

	return SUCCESS;
}

void free_node(node_list **node) {
	if (*node) {
		for (int i = 0; i < 3; i++) {
			if ((*node)->user->right_coast[i]) {
				free((*node)->user->right_coast[i]);
				(*node)->user->right_coast[i] = NULL;
			}
		}

		for (int i = 0; i < 3; i++) {
			if ((*node)->user->left_coast[i]) {
				free((*node)->user->left_coast[i]);
				(*node)->user->left_coast[i] = NULL;
			}
		}

		free((*node)->user->on_boat);
		(*node)->user->on_boat = NULL;

		free((*node)->user);
		(*node)->user = NULL;

		free(*node);
		*node = NULL;
	}
}

void free_list(list_t **list) {
	if (*list) {
		if ((*list)->root) {
			node_list *node = (*list)->root;
			node_list *temp = NULL;

			while (node) {
				temp = node;
				node = node->next;
				free_node(&temp);
			}
		}

		free(*list);
		*list = NULL;
	}
}