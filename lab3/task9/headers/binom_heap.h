#ifndef BIN_HEAP
#define BIN_HEAP

#include "types.h"
#include "binom_tree.h"

int init_heap_node(binom_heap**, node_list*);
int make_node(node_list**, T*); 
int add_node(binom_heap**, char*, int (*cmp)(T*, T*));
int push_back(binom_heap**, node_list*, int (*cmp)(T*, T*));
int delete_node_list(binom_heap**, node_list*, int (*cmp)(T*, T*));
int delete_node_heap(binom_heap**, node_list*, int (*cmp)(T*, T*));
int extract_min_heap(binom_heap**, int (*cmp)(T*, T*));
int stop_delete(binom_heap*);
int transform(binom_heap**, int (*cmp)(T*, T*));
void set_min(binom_heap**, int (*cmp)(T*, T*));
T *get_min_heap(binom_heap*);
T *get_max_heap(binom_heap*);
int output_heap(binom_heap**, FILE*, int (*cmp)(T*, T*));

#endif // BIN_HEAP