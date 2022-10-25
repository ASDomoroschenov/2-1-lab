#include <stdlib.h>
#include "binom_heap.h"

binom_heap *init_heap_employee(employee *item) {
	binom_heap *heap = (binom_heap*)malloc(sizeof(binom_heap));
	heap->root = (Node_list*)malloc(sizeof(Node_list));
	heap->root->tree = init_root_tree(item);
	heap->extreme = heap->root;
	heap->root->next = heap->root;
	heap->length = 1;

	return heap;
}

Node_list *make_node_employee(employee *item) {
	Node_list *node = (Node_list*)malloc(sizeof(Node_list));
	node->tree = init_root_tree(item);

	return node;
}

void add_node(binom_heap *heap, Node_list *where, Node_list *node, int (*cmp)(employee*, employee*)) {
	node->next = where->next;
	where->next = node;
	heap->length++;

	if (!cmp(heap->extreme->tree->root->key, node->tree->root->key)) {
		heap->extreme = node;
	}
}

binom_heap *get_heap(employee **workers, size_t size, int (*cmp)(employee*, employee*)) {
	binom_heap *heap = init_heap_employee(workers[0]);
	Node_list *node = heap->root;

	for (int i = 1; i < size; i++) {
		add_node(heap, node, make_node_employee(workers[i]), cmp);
		node = node->next;
	}

	return heap;
}

void set_extreme(binom_heap *heap, int (*cmp)(employee*, employee*)) {
	heap->extreme = heap->root;
	Node_list *node = heap->root->next;

	for (int i = 1; i < heap->length; i++) {
		if (!cmp(heap->extreme->tree->root->key, node->tree->root->key)) {
			heap->extreme = node;
		}
		node = node->next;
	}
}

void delete_node(binom_heap *heap, Node_list *node_del, int (*cmp)(employee*, employee*)) {
	Node_list *node = heap->root;

	while (node->next != node_del) {
		node = node->next;
	}

	node->next = node_del->next;

	if (node_del == heap->root) {
		heap->root = heap->root->next;
	}

	if (node_del == heap->extreme) {
		free(node_del);
		set_extreme(heap, cmp);
	} else {
		free(node_del);
	}

	heap->length--;
}

int stop_delete(binom_heap *heap) {
	Node_list *node = heap->root;

	if (heap->root->next != heap->root) {
		for (size_t i = 0; i < heap->length; i++) {
			Node_list *check_node = node->next;
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

void transform_to_bin_heap(binom_heap *heap, int (*cmp)(employee*, employee*)) {
	Node_list *node = NULL;
	Node_list *node_merge = NULL;

	while (!stop_delete(heap)) {
		node = heap->root;
		for (int i = 0; i < heap->length; i++) {
			node_merge = node->next;
			for (int j = i + 1; j < heap->length; j++) {
				if (node->tree->root->degree == node_merge->tree->root->degree) {
					node->tree = merge(node->tree, node_merge->tree, cmp);
					Node_list *temp = node_merge;
					node_merge = node_merge->next;
					delete_node(heap, temp, cmp);
				} else {
					node_merge = node_merge->next;
				}
			}
			node = node->next;
		}
	}
}

employee *get_extreme_heap(binom_heap *heap, int (*cmp)(employee*, employee*)) {
	employee *extreme = heap->extreme->tree->root->key;

	for (size_t i = 0; i < heap->extreme->tree->root->degree; i++) {
		Node_list *node = (Node_list*)malloc(sizeof(node));
		node->tree = (binom_tree*)malloc(sizeof(binom_tree));
		node->tree->root = heap->extreme->tree->root->childs[i];
		node->next = NULL;
		add_node(heap, heap->extreme, node, cmp);
	}

	if (heap->extreme->tree->root->childs)
		free(heap->extreme->tree->root->childs);
	free(heap->extreme->tree->root);
	free(heap->extreme->tree);
	delete_node(heap, heap->extreme, cmp);
	transform_to_bin_heap(heap, cmp);

	return extreme;
}

void output_heap(binom_heap *heap) {
	Node_list *node = heap->root;

	for (int i = 0; i < heap->length; i++) {
		printf("%d %s %s %lf\n", node->tree->root->key->id, node->tree->root->key->name, node->tree->root->key->surname, node->tree->root->key->wage);
		node = node->next;
	}
}