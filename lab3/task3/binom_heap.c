#include <stdlib.h>
#include "binom_heap.h"

binom_heap *init_heap(employee *item) {
	binom_heap *heap = (binom_heap*)malloc(sizeof(binom_heap));
	heap->root = init_node_heap(item);
	heap->root->next = heap->root;

	return heap;
}

Node_list *init_node_heap(employee *item) {
	Node_list *node = (Node_list*)malloc(sizeof(Node_list));
	node->tree = init_root_tree(item);
	node->next = NULL;

	return node;
}

Node_list *init_node_heap_by_root(Node_tree *node_tree) {
	Node_list *node = (Node_list*)malloc(sizeof(Node_list));
	node->tree = (binom_tree*)malloc(sizeof(binom_tree));
	node->tree->root = node_tree;
	node->next = NULL;

	return node;
}

int cmp(employee *item_1, employee *item_2) {
	return item_1->wage < item_2->wage;
}

binom_heap *get_heap_from_arr(employee **workers, size_t size) {
	binom_heap *heap = (binom_heap*)malloc(sizeof(binom_heap));
	heap = init_heap(workers[0]);
	Node_list *node = heap->root;

	for (size_t i = 1; i < size; i++) {
		add_employe(heap, node, workers[i]);
		node = node->next;
	}

	heap->length = size;

	return heap;
}

int stop_del(binom_heap *heap) {
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

void output_heap(binom_heap *heap) {
	employee *worker = NULL;
	Node_list *node = heap->root;

	for (size_t i = 0; i < heap->length; i++) {
		worker = node->tree->root->key;
		printf("%d %s %s %lf\n", worker->id, worker->name, worker->surname, worker->wage);
		node = node->next;
	}
}

employee *del_min(binom_heap *heap) {
	Node_list *node = heap->root->next;
	Node_list *min = heap->root;
	employee *min_item = min->tree->root->key;

	for (size_t i = 0; i < heap->length; i++) {
		if (!cmp(min_item, node->tree->root->key)) {
			min_item = node->tree->root->key;
			min = node;
		}

		node = node->next;
	}

	delete_node_min(heap, min);

	while (!stop_del(heap)) {
		node = heap->root;
		for (size_t i = 0; i < heap->length; i++) {
			Node_list *merge_node = node->next;
			for (size_t j = i + 1; j < heap->length; j++) {
				if (merge_node->tree->root->degree == node->tree->root->degree) {
					node->tree = merge(node->tree, merge_node->tree, cmp);
					Node_list *temp = merge_node;
					merge_node = merge_node->next;
					delete_node(heap, temp);
				} else {
					merge_node = merge_node->next;
				}
			}
			node = node->next;
		}
	}

	return min_item;
}

void delete_node_min(binom_heap *heap, Node_list *del_node) {
	for (int i = 0; i < del_node->tree->root->degree; i++) {
		Node_list *node = init_node_heap_by_root(del_node->tree->root->childs[i]);
		add_node(heap, del_node, node);
	}

	if (del_node->tree->root->childs)
		free(del_node->tree->root->childs);
	free(del_node->tree->root);
	free(del_node->tree);
	delete_node(heap, del_node);
}

void delete_node(binom_heap *heap, Node_list *del_node) {
	if (del_node != heap->root) {
		Node_list *node = heap->root;

		while (node->next != del_node) {
			node = node->next;
		}

		node->next = del_node->next;
		free(del_node);
	} else {
		Node_list *temp = heap->root;
		Node_list *node = heap->root;

		while (node->next != heap->root) {
			node = node->next;
		}

		node->next = heap->root->next;
		heap->root = heap->root->next;
		free(temp);
	}

	heap->length--;
}

void add_node(binom_heap *heap, Node_list *where, Node_list *node) {
	node->next = where->next;
	where->next = node;
	heap->length++;
}

void add_employe(binom_heap *heap, Node_list *where, employee *item) {
	Node_list *node = init_node_heap(item);
	add_node(heap, where, node);
}