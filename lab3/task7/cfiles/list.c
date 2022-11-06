#include <stdlib.h>
#include <string.h>
#include "../headers/string.h"
#include "../headers/list.h"

int init_node(list_node **node, citizen *item) {
	*node = (list_node*)malloc(sizeof(list_node));
	
	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->human = item;
	(*node)->next = NULL;
		
	return SUCCESS;
}

int cmp(citizen *item_1, citizen *item_2) {
	char *reverse_date_1 = NULL;
	char *reverse_date_2 = NULL;
	int res = 0;

	if (reverse(item_1->date_birth, &reverse_date_1) == NO_MEMORY ||
		reverse(item_2->date_birth, &reverse_date_2) == NO_MEMORY) {
		return NO_MEMORY;
	}

	res = strcmp(reverse_date_1, reverse_date_2);

	free(reverse_date_1);
	free(reverse_date_2);

	return res;
}

int add_node(list **list_citizen, citizen *item, int (*cmp)(citizen*, citizen*)) {
	if (!*list_citizen) {
		*list_citizen = (list*)malloc(sizeof(list));

		if (!*list_citizen) {
			return NO_MEMORY;
		}

		if (init_node(&((*list_citizen)->root), item) == NO_MEMORY) {
			return NO_MEMORY;
		}

		return SUCCESS;
	}

	list_node *node = (*list_citizen)->root;
	list_node *add_node = NULL;

	if (init_node(&add_node, item) == NO_MEMORY) {
		return NO_MEMORY;
	}

	int exit_code_cmp = cmp(add_node->human, node->human);

	if (exit_code_cmp == NO_MEMORY) {
		free(add_node);
		return NO_MEMORY;
	}

	while (exit_code_cmp >= 0 && node->next != NULL) {
		node = node->next;
	}

	if (node == (*list_citizen)->root && exit_code_cmp < 0) {
		add_node->next = (*list_citizen)->root;
		(*list_citizen)->root = add_node;
	} else {
		add_node->next = node->next;
		node->next = add_node;
	}

	return SUCCESS;
}

int delete_node(list **list_citizen, list_node *del_node) {
	if (del_node != (*list_citizen)->root) {
		list_node *node = (*list_citizen)->root;

		while (node->next != del_node) {
			node = node->next;
		}

		node->next = del_node->next;
		free_node(&del_node);
	} else {
		list_node *temp = (*list_citizen)->root;
		(*list_citizen)->root = (*list_citizen)->root->next;
		free_node(&temp);
		if (!(*list_citizen)->root) {
			free(*list_citizen);
			*list_citizen = NULL;
		}
	}
}

void free_node(list_node **node) {
	free((*node)->human->surname);
	free((*node)->human->name);
	free((*node)->human->patronymic);
	free((*node)->human->date_birth);
	free((*node)->human);
	free((*node));
}

void free_list(list **list_citizen) {
	if (*list_citizen) {
		list_node *node = (*list_citizen)->root;

		while (node != NULL) {
			list_node *temp = node;
			node = node->next;
			free_node(&temp);
		}

		free(*list_citizen);
	}
}