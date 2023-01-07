#include <stdlib.h>
#include <string.h>
#include "list.h"

int create_list(list_t **list) {
	*list = (list_t*)malloc(sizeof(list_t));

	if (!*list) {
		return NO_MEMORY;
	}

	(*list)->root = NULL;
	(*list)->size = 0;

	return SUCCESS;
}

int create_node_list(client *user, node_list **node) {
	*node = (node_list*)malloc(sizeof(node_list));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->user = user;

	return SUCCESS;
}

int delete_node(node_list *del_node, list_t **list) {
	if (del_node != (*list)->root) {
		node_list *prev_node = (*list)->root;

		while (prev_node->next != del_node) {
			prev_node = prev_node->next;
		}

		node_list *temp = prev_node->next->next;
		free_node(&(prev_node->next));
		prev_node->next = temp;
	} else {
		free_node(&((*list)->root));
	}

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
		free((*node)->user->array.vars);
		(*node)->user->array.vars = NULL;
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