#ifndef BIN_HEAP
#define BIN_HEAP

#include "binom_tree.h"

typedef struct Tag_node_list {
	struct Tag_node_list *next;
	binom_tree *tree;
} node_list;

typedef struct {
	node_list *root;
	node_list *extreme;
	int length;
} fib_heap;

int init_heap_node(fib_heap**, node_list*);
int make_node_employee(node_list**, employee*);
void set_extreme(fib_heap**, int (*cmp)(employee*, employee*));
int push_back(fib_heap**, node_list*, int (*cmp)(employee*, employee*));
int delete_node(fib_heap**, node_list*, int (*cmp)(employee*, employee*));
int get_heap(fib_heap**, employee **, int, int (*cmp)(employee*, employee*));
int stop_delete(fib_heap*);
int transform_to_bin_heap(fib_heap**, int (*cmp)(employee*, employee*));
int get_extreme_heap(employee**, fib_heap**, int (*cmp)(employee*, employee*));

#endif // BIN_HEAP