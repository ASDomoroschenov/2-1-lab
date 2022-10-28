#include <stdlib.h>
#include "../headers/fib_heap.h"

int init_heap_node(fib_heap **heap, node_list *node) {
	*heap = (fib_heap*)malloc(sizeof(fib_heap));

	if (*heap) {
		(*heap)->root = node;
		(*heap)->extreme = (*heap)->root;
		(*heap)->root->next = (*heap)->root;
		(*heap)->length = 1;
		return SUCCESS;
	}

	return NO_MEMORY;
}

int make_node_employee(node_list **node, employee *item) {
	*node = (node_list*)malloc(sizeof(node_list));

	if (*node) {
		if (init_root_tree(&((*node)->tree), item) == SUCCESS) {
			return SUCCESS;
		}
	}

	return NO_MEMORY;
}

void set_extreme(fib_heap **heap, int (*cmp)(employee*, employee*)) {
	(*heap)->extreme = (*heap)->root;
	node_list *node = (*heap)->root->next;

	while (node != (*heap)->root) {
		if (!cmp((*heap)->extreme->tree->root->key, node->tree->root->key)) {
			(*heap)->extreme = node;
		}
		node = node->next;
	}
}

int push_back(fib_heap **heap, node_list *add_node, int (*cmp)(employee*, employee*)) {
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

int get_heap(fib_heap **heap, employee **arr_employee, int count_employee, int (*cmp)(employee*, employee*)) {
	for (int i = 0; i < count_employee; i++) {
		node_list *add_node = NULL;
		if (make_node_employee(&add_node, arr_employee[i]) == SUCCESS) {
			push_back(heap, add_node, cmp);
		} else {
			return NO_MEMORY;
		}
	}

	return SUCCESS;
}

int delete_node(fib_heap **heap, node_list *node_del, int (*cmp)(employee*, employee*)) {
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

int stop_delete(fib_heap *heap) {
	node_list *node = heap->root;

	if (heap->root->next != heap->root) {
		for (size_t i = 0; i < heap->length; i++) {
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

int transform_to_bin_heap(fib_heap **heap, int (*cmp)(employee*, employee*)) {
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

int get_extreme_heap(employee **worker, fib_heap **heap, int (*cmp)(employee*, employee*)) {
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
				//почистить
				return NO_MEMORY;
			}
		} else {
			//почистить
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

int sort(fib_heap **heap, employee ***workers, int count_workers, int (*cmp)(employee*, employee*)) {
	int exit_code = 0;

	for (int i = 0; i < count_workers; i++) {
		exit_code = get_extreme_heap(&(*workers)[i], heap, cmp);
		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}

	return SUCCESS;
}