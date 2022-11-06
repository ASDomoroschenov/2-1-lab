#ifndef BIN_HEAP
#define BIN_HEAP

#include "types.h"
#include "binom_tree.h"
#include "post.h"

int make_node_tree(node_list**, binom_tree*);
int init_heap_node(binom_heap**, node_list*);
int make_node_mail(node_list**, mail*);
void set_extreme(binom_heap**, int (*cmp)(mail*, mail*));
int push_back(binom_heap**, node_list*, int (*cmp)(mail*, mail*));
int delete_node(binom_heap**, node_list*, int (*cmp)(mail*, mail*));
int get_heap(binom_heap**, mail**, int, int (*cmp)(mail*, mail*));
int stop_delete(binom_heap*);
int transform_to_bin_heap(binom_heap**, int (*cmp)(mail*, mail*));
int get_extreme_heap(mail**, binom_heap**, int (*cmp)(mail*, mail*));
int heap_sort(mail***, int, int (*cmp)(mail*, mail*));
void free_bin_heap(binom_heap**);

#endif // BIN_HEAP