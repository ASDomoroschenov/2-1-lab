#ifndef BIN_HEAP
#define BIN_HEAP

#include "binom_tree.h"

typedef struct Tag_node_list {
	struct Tag_node_list *next;
	binom_tree *tree;
} Node_list;

typedef struct {
	Node_list *root;
	Node_list *extreme;
	size_t length;
} binom_heap;

binom_heap *init_heap_employee(employee*);
Node_list *make_node_employee(employee*);
void delete_node(binom_heap*, Node_list*, int (*cmp)(employee*, employee*));
void add_node(binom_heap*, Node_list*, Node_list*, int (*cmp)(employee*, employee*));
void set_extreme(binom_heap*, int (*cmp)(employee*, employee*));
binom_heap *get_heap(employee**, size_t, int (*cmp)(employee*, employee*));
employee *get_extreme_heap(binom_heap*, int (*cmp)(employee*, employee*));
int stop_delelete(binom_heap*);
void transform_to_bin_heap(binom_heap*, int (*cmp)(employee*, employee*));
void output_heap(binom_heap*);

#endif // BIN_HEAP