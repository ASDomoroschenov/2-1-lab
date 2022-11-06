#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "../headers/binom_heap.h"
#include "../headers/string.h"

int init_heap_node(binom_heap **heap, node_list *node) {
	*heap = (binom_heap*)malloc(sizeof(binom_heap));

	if (*heap) {
		(*heap)->root = node;
		(*heap)->min = (*heap)->root;
		(*heap)->root->next = (*heap)->root;
		(*heap)->length = 1;
		return SUCCESS;
	}

	return NO_MEMORY;
}

int make_node(node_list **node, T *item) {
	*node = (node_list*)malloc(sizeof(node_list));
	int exit_code = 0;

	if (!*node) {
		return NO_MEMORY;
	}

	exit_code = init_root_tree(&((*node)->tree), item);
	
	if (exit_code != SUCCESS) {
		return exit_code;
	}

	(*node)->next = NULL;

	return SUCCESS;
}

int add_node(binom_heap **heap, char *str, int (*cmp)(T*, T*)) {
	node_list *node = NULL;
	T *item = NULL;
	int exit_code = get_T(&item, str);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = make_node(&node, item);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = push_back(heap, node, cmp);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = transform(heap, cmp);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	return SUCCESS;
}

int push_back(binom_heap **heap, node_list *add_node, int (*cmp)(T*, T*)) {
	int exit_code = 0;

	if (*heap) {
		node_list *node = (*heap)->root;

		while (node->next != (*heap)->root) {
			node = node->next;
		}

		add_node->next = (*heap)->root;
		node->next = add_node;

		if (!cmp((*heap)->min->tree->root->key, add_node->tree->root->key)) {
			(*heap)->min = add_node;
		}

		((*heap)->length)++;
	} else {
		exit_code = init_heap_node(heap, add_node);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}

	return SUCCESS;
}

void set_min(binom_heap **heap, int (*cmp)(T*, T*)) {
	(*heap)->min = (*heap)->root;
	node_list *node = (*heap)->root->next;

	while (node != (*heap)->root) {
		if (!cmp((*heap)->min->tree->root->key, node->tree->root->key)) {
			(*heap)->min = node;
		}
		node = node->next;
	}
}

T *get_min_heap(binom_heap *heap) {
	return heap->root->tree->root->key;
}

T *get_max_heap(binom_heap *heap) {
	T *max = get_max_tree(heap->root->tree->root);
	node_list *node = heap->root->next;

	while (node != heap->root) {
		T *max_tree = get_max_tree(node->tree->root);

		if (max->prior < max_tree->prior) {
			max = max_tree;
		}
		
		node = node->next;
	}

	return max;
}

int delete_node_list(binom_heap **heap, node_list *node_del, int (*cmp)(T*, T*)) {
	node_list *prev_node = (*heap)->root;

	while (prev_node->next != node_del) {
		prev_node = prev_node->next;
	}

	prev_node->next = node_del->next;

	if (node_del == (*heap)->root) {
		if ((*heap)->root == (*heap)->root->next) {
			free((*heap)->root);
			(*heap)->root = NULL;
		} else {
			(*heap)->root = (*heap)->root->next;

			if (node_del == (*heap)->min && (*heap)->length > 1) {
				set_min(heap, cmp);
			}

			free(node_del);
			node_del = NULL;
		}

		(*heap)->length--;

		return SUCCESS;
	}

	if (node_del == (*heap)->min && (*heap)->length > 1) {
		set_min(heap, cmp);
	}

	free(node_del);
	node_del = NULL;

	(*heap)->length--;

	return SUCCESS;
}

int delete_node_heap(binom_heap **heap, node_list *node_del, int (*cmp)(T*, T*)) {
	int exit_code = 0;

	for (int i = 0; i < node_del->tree->root->degree; i++) {
		node_list *node = (node_list*)malloc(sizeof(node_list));
		if (node) {
			node->tree = (binom_tree*)malloc(sizeof(binom_tree));
			if (node->tree) {
				node->tree->root = node_del->tree->root->childs[i];
				node->next = NULL;
				exit_code = push_back(heap, node, cmp);

				if (exit_code != SUCCESS) {
					free(node);
					free(node->tree);
					return exit_code;
				}
			} else {
				free(node);
				return NO_MEMORY;
			}
		} else {
			return NO_MEMORY;
		}
	}

	if (node_del->tree->root->childs) {
		free(node_del->tree->root->childs);
		node_del->tree->root->childs = NULL;
	}
	free_str(&(node_del->tree->root->key->text_field));
	free(node_del->tree->root->key);
	node_del->tree->root->key = NULL;
	free(node_del->tree->root);
	node_del->tree->root = NULL;
	free(node_del->tree);
	node_del->tree = NULL;
	delete_node_list(heap, node_del, cmp);
	transform(heap, cmp);

	return SUCCESS;
}

int extract_min_heap(binom_heap **heap, int (*cmp)(T*, T*)) {
	return delete_node_heap(heap, (*heap)->min, cmp);
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

int transform(binom_heap **heap, int (*cmp)(T*, T*)) {
	node_list *node = NULL;
	node_list *node_merge = NULL;
	int exit_code = 0;

	if ((*heap)->length != 0) {
		while (!stop_delete(*heap)) {
			node = (*heap)->root;
			for (int i = 0; i < (*heap)->length; i++) {
				node_merge = node->next;
				for (int j = i + 1; j < (*heap)->length; j++) {
					if (node->tree->root->degree == node_merge->tree->root->degree) {
						exit_code = merge(&(node->tree), node->tree, node_merge->tree, cmp);

						if (exit_code != SUCCESS) {
							return exit_code;
						}

						node_list *temp = node_merge;
						node_merge = node_merge->next;

						delete_node_list(heap, temp, cmp);
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

int output_heap(binom_heap **heap, FILE *output, int (*cmp)(T*, T*)) {
	if (*heap) {
		while ((*heap)->root) {
			fprintf(output, "%d ", (*heap)->min->tree->root->key->prior);
			print_str((*heap)->min->tree->root->key->text_field, output);
			extract_min_heap(heap, cmp);
		}
		free(*heap);

		return SUCCESS;
	}

	return EMPTY_HEAP;
}