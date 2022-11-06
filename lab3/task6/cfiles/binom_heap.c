#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../headers/binom_heap.h"

int make_node_tree(node_list **node, binom_tree *tree) {
	*node = (node_list*)malloc(sizeof(node_list));

	if (!*node) {
		return NO_MEMORY;
	}

	(*node)->tree = tree;
	(*node)->next = NULL;

	return SUCCESS;
}

int init_heap_node(binom_heap **heap, node_list *node) {
	*heap = (binom_heap*)malloc(sizeof(binom_heap));

	if (*heap) {
		(*heap)->root = node;
		(*heap)->extreme = (*heap)->root;
		(*heap)->root->next = (*heap)->root;
		(*heap)->length = 1;
		return SUCCESS;
	}

	return NO_MEMORY;
}

int make_node_mail(node_list **node, mail *item) {
	*node = (node_list*)malloc(sizeof(node_list));

	if (*node) {
		if (init_root_tree(&((*node)->tree), item) == SUCCESS) {
			return SUCCESS;
		}
	}

	return NO_MEMORY;
}

void set_extreme(binom_heap **heap, int (*cmp)(mail*, mail*)) {
	(*heap)->extreme = (*heap)->root;
	node_list *node = (*heap)->root->next;

	while (node != (*heap)->root) {
		if (!cmp((*heap)->extreme->tree->root->key, node->tree->root->key)) {
			(*heap)->extreme = node;
		}
		node = node->next;
	}
}

int push_back(binom_heap **heap, node_list *add_node, int (*cmp)(mail*, mail*)) {
	if (*heap) {
		node_list *node = (*heap)->root;

		while (node->next != (*heap)->root) {
			node = node->next;
		}

		add_node->next = (*heap)->root;
		node->next = add_node;

		if (!cmp((*heap)->extreme->tree->root->key, add_node->tree->root->key)) {
			(*heap)->extreme = add_node;
		}

		((*heap)->length)++;
	} else {
		if (init_heap_node(heap, add_node) == NO_MEMORY) {
			return NO_MEMORY;
		}
	}

	return SUCCESS;
}

int get_heap(binom_heap **heap, mail **mails, int count_mails, int (*cmp)(mail*, mail*)) {
	for (int i = 0; i < count_mails; i++) {
		binom_tree *merge_tree = NULL;
		node_list *add_node = NULL;

		if (make_node_tree(&add_node, NULL) == NO_MEMORY) {
			return NO_MEMORY;
		}

		if (init_root_tree(&merge_tree, mails[i]) == NO_MEMORY ||
			merge(&(add_node->tree), add_node->tree, merge_tree, cmp) == NO_MEMORY) {
			return NO_MEMORY;
		}

		if (push_back(heap, add_node, cmp) == NO_MEMORY) {
			return NO_MEMORY;
		}

		transform_to_bin_heap(heap, cmp);
	}

	return SUCCESS;
}

int delete_node(binom_heap **heap, node_list *node_del, int (*cmp)(mail*, mail*)) {
	node_list *node = (*heap)->root;

	while (node->next != node_del) {
		node = node->next;
	}

	node->next = node_del->next;

	if (node_del == (*heap)->root) {
		(*heap)->root = (*heap)->root->next;
	}

	if (node_del == (*heap)->extreme && (*heap)->length > 1) {
		free(node_del);
		set_extreme(heap, cmp);
	} else {
		free(node_del);
	}

	(*heap)->length--;

	return SUCCESS;
}

int stop_delete(binom_heap *heap) {
	node_list *node = heap->root;

	if (heap->root->next != heap->root) {
		for (int i = 0; i < heap->length; i++) {
			node_list *check_node = node->next;
			for (int j = i + 1; j < heap->length; j++) {
				if (check_node->tree->root->degree == node->tree->root->degree) {
					return 0;
				}
				check_node = check_node->next;
			}
			node = node->next;
		}
	}

	return 1;
}

int transform_to_bin_heap(binom_heap **heap, int (*cmp)(mail*, mail*)) {
	node_list *node = NULL;
	node_list *node_merge = NULL;

	if ((*heap)->length != 0) {
		while (!stop_delete(*heap)) {
			node = (*heap)->root;
			for (int i = 0; i < (*heap)->length; i++) {
				node_merge = node->next;
				for (int j = i + 1; j < (*heap)->length; j++) {
					if (node->tree->root->degree == node_merge->tree->root->degree) {
						if (merge(&(node->tree), node->tree, node_merge->tree, cmp) != NO_MEMORY) {
							node_list *temp = node_merge;
							node_merge = node_merge->next;
							delete_node(heap, temp, cmp);
						} else {
							return NO_MEMORY;
						}
					} else {
						node_merge = node_merge->next;
					}
				}
				node = node->next;
			}
		}
	}

	return SUCCESS;
}

int get_extreme_heap(mail **worker, binom_heap **heap, int (*cmp)(mail*, mail*)) {
	*worker = (*heap)->extreme->tree->root->key;
	
	for (int i = 0; i < (*heap)->extreme->tree->root->degree; i++) {
		node_list *node = (node_list*)malloc(sizeof(node_list));
		if (node) {
			node->tree = (binom_tree*)malloc(sizeof(binom_tree));
			if (node->tree) {
				node->tree->root = (*heap)->extreme->tree->root->childs[i];
				node->next = NULL;
				push_back(heap, node, cmp);
			} else {
				free(node);
				return NO_MEMORY;
			}
		} else {
			return NO_MEMORY;
		}
	}
	if ((*heap)->extreme->tree->root->childs) {
		free((*heap)->extreme->tree->root->childs);
	}
	free((*heap)->extreme->tree->root);
	free((*heap)->extreme->tree);
	delete_node(heap, (*heap)->extreme, cmp);
	transform_to_bin_heap(heap, cmp);

	return SUCCESS;
}

int heap_sort(mail ***mails, int count_mails, int (*cmp)(mail*, mail*)) {
	binom_heap *heap = NULL;
	int exit_code = 0;

	exit_code = get_heap(&heap, *mails, count_mails, cmp);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	for (int i = 0; i < count_mails; i++) {
		mail *item_mail = NULL;
		exit_code = get_extreme_heap(&item_mail, &heap, cmp);
		if (exit_code != SUCCESS) {
			return exit_code;
		}
		(*mails)[i] = item_mail;
	}

	free(heap);

	return SUCCESS;
}

void free_bin_heap(binom_heap **heap) {
	if (*heap) {
		node_list *node = (*heap)->root;

		for (int i = 0; i < (*heap)->length; i++) {
			free_bin_tree(&(node->tree->root));
			free(node->tree);
			node_list *temp = node;
			node = node->next;
			free(temp);
		}

		free(*heap);
	}
}