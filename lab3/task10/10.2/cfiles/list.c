#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/list.h"

int init_node_list(Node **init_node, char *str) {
	*init_node = (Node*)malloc(sizeof(Node));

	if (!*init_node) {
		return NO_MEMORY;
	}

	(*init_node)->key = (char*)malloc(sizeof(char) * (strlen(str) + 1));

	if (!(*init_node)->key) {
		free(*init_node);
		*init_node = NULL;

		return NO_MEMORY;
	}

	(*init_node)->next = NULL;
	strcpy((*init_node)->key, str);

	return SUCCESS;
}

int push_back(List **list, char *str) {
	Node *node = NULL;
	int exit_code = init_node_list(&node, str);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*list) {
		*list = (List*)malloc(sizeof(List));

		if (!*list) {
			free(node->key);
			node->key = NULL;
			free(node);
			node = NULL;

			return NO_MEMORY;
		}

		(*list)->root = node;

		return SUCCESS;
	}

	if (!(*list)->root) {
		(*list)->root = node;

		return SUCCESS;
	}

	Node *prev_node = (*list)->root;

	while (prev_node->next != NULL) {
		prev_node = prev_node->next;
	}

	prev_node->next = node;

	return SUCCESS;
}

int get_back(List **list) {
	if (*list) {
		if ((*list)->root) {
			Node *node = (*list)->root;
			Node *prev_node = NULL;

			while (node->next != NULL) {
				prev_node = node;
				node = node->next;
			}

			if (node == (*list)->root) {
				free((*list)->root->key);
				(*list)->root->key = NULL;
				free((*list)->root);
				(*list)->root = NULL;

				return SUCCESS;
			}

			free(node->key);
			node->key = NULL;
			free(node);
			node = NULL;

			if (prev_node) {
				prev_node->next = NULL;
			}
		}
	}

	return SUCCESS;
}

int get_front(List **list) {
	if (*list && (*list)->root) {
		if ((*list)->root->next == NULL) {
			free((*list)->root->key);
			(*list)->root->key = NULL;
			free((*list)->root);
			(*list)->root = NULL;
		} else {
			Node *temp = (*list)->root;
			(*list)->root = (*list)->root->next;

			free(temp->key);
			temp->key = NULL;
			free(temp);
			temp = NULL;
		}
	}

	return SUCCESS;
}

void free_list(List **list) {
	if (*list) {
		if ((*list)->root) {
			Node *node = (*list)->root;

			while (node != NULL) {
				Node *temp = node;
				node = node->next;

				free(temp->key);
				temp->key = NULL;
				free(temp);
				temp = NULL;
			}
		}

		free(*list);
		*list = NULL;
	}
}