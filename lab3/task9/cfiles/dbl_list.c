#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/dbl_list.h"
#include "../headers/string.h"

int get_node(list_node **node, char *str) {
	T *item = NULL;
	int exit_code = get_T(&item, str);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	*node = (list_node*)malloc(sizeof(list_node));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->key = item;
	(*node)->prev = NULL;
	(*node)->next = NULL;

	return SUCCESS;
}

int add_list(dbl_list **list, char *str) {
	list_node *add_node = NULL;
	int exit_code = 0;

	exit_code = get_node(&add_node, str);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*list) {
		*list = (dbl_list*)malloc(sizeof(dbl_list));

		if (!*list) {
			return NO_MEMORY;
		}

		(*list)->root = add_node;

		return SUCCESS;
	}

	list_node *node = (*list)->root;
	list_node *prev_node = NULL;

	while (node != NULL && node->key->prior <= add_node->key->prior) {
		prev_node = node;
		node = node->next;
	}

	if (node == (*list)->root) {
		if (node->key->prior <= add_node->key->prior) {
			node->next = add_node;
			add_node->prev = node;
		} else {
			add_node->next = (*list)->root;
			add_node->prev = NULL;
			(*list)->root->prev = add_node;
			(*list)->root = add_node;
		}
	} else {
		if (prev_node->next == NULL) {
			prev_node->next = add_node;
			add_node->prev = prev_node;
		} else {
			add_node->prev = prev_node;
			add_node->next = node;
			node->prev = add_node;
			prev_node->next = add_node;
		}
	}

	return SUCCESS;
}

T *get_min_list(dbl_list *list) {
	return list->root->key;
}

T *get_max_list(dbl_list *list) {
	list_node *node_max = list->root;

	while (node_max->next != NULL) {
		node_max = node_max->next;
	}

	return node_max->key;
}

int extract_min_list(dbl_list **list) {
	if (*list) {
		if ((*list)->root->next) {
			list_node *temp = (*list)->root;
	
			(*list)->root->next->prev = NULL;
			(*list)->root = (*list)->root->next;

			free_str(&(temp->key->text_field));
			free(temp->key);
			free(temp);
			temp = NULL;
		} else {
			free_str(&((*list)->root->key->text_field));
			free((*list)->root->key);
			free((*list)->root);
			(*list)->root = NULL;
		}

		return SUCCESS;
	}

	return EMPTY_LIST;
}

int extract_max_list(dbl_list **list) {
	if (*list) {
		list_node *node = (*list)->root;

		while (node->next != NULL) {
			node = node->next;
		}

		if (node == (*list)->root) {
			free_str(&((*list)->root->key->text_field));
			free((*list)->root->key);
			free((*list)->root);
			(*list)->root = NULL;
		} else {
			node->prev->next = NULL;
			free_str(&(node->key->text_field));
			free(node->key);
			free(node);
			node = NULL;
		}

		return SUCCESS;
	}

	return EMPTY_LIST;
}

int output_list(dbl_list **list, FILE *output) {
	if (*list) {
		while ((*list)->root) {
			fprintf(output, "%d ", (*list)->root->key->prior);
			print_str((*list)->root->key->text_field, output);
			extract_min_list(list);
		}
		free(*list);
		*list = NULL;

		return SUCCESS;
	}

	return EMPTY_LIST;
}