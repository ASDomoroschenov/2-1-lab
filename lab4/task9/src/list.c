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

int create_node_list(char *def_name, char *value, long long hash, node_list **node) {
	*node = (node_list*)malloc(sizeof(node_list));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->def_name = (char*)malloc(sizeof(char) * (strlen(def_name) + 1));

	if (!(*node)->def_name) {
		free(*node);
		*node = NULL;

		return NO_MEMORY;
	}

	(*node)->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));

	if (!(*node)->value) {
		free((*node)->def_name);
		free(*node);
		*node = NULL;

		return NO_MEMORY;
	}

	strcpy((*node)->def_name, def_name);
	strcpy((*node)->value, value);
	(*node)->hash = hash;
	(*node)->next = NULL;

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
		free((*node)->def_name);
		(*node)->def_name = NULL;
		free((*node)->value);
		(*node)->value = NULL;
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