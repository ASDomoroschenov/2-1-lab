#ifndef BIN_HEAP
#define BIN_HEAP

#include "binom_tree.h"

typedef struct Tag_node_list {
	struct Tag_node_list *next;
	binom_tree *tree;
} Node_list;

typedef struct {
	Node_list *root;
	size_t length;
} binom_heap;

binom_heap *get_heap_from_arr(employee **, size_t);
binom_heap *init_heap(employee *item);
Node_list *init_node_heap(employee *item);
int stop_del(binom_heap *heap);
void output_heap(binom_heap *heap);
void delete_node_min(binom_heap*, Node_list*);
void delete_node(binom_heap*, Node_list*);
employee *del_min(binom_heap *heap);
void add_employe(binom_heap*, Node_list*, employee*);
int cmp(employee*, employee*);
void add_node(binom_heap*, Node_list*, Node_list*);

#endif // BIN_HEAP