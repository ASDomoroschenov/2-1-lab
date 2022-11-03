#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "types.h"

int init_node_bin_tree(node_bin_tree**, char*);
int add_bin_lexem(node_bin_tree**,bin_tree**, char*);
node_bin_tree *min(node_bin_tree*);
node_bin_tree *max(node_bin_tree*);
node_bin_tree *follow_del(node_bin_tree*);
node_bin_tree *find_node(node_bin_tree*, char*);
int add_node_tree(node_bin_tree*, bin_tree**);
void delete_node(node_bin_tree**, bin_tree**);
int rebuild_bin_tree(node_bin_tree**, bin_tree**);
int depth(node_bin_tree*);
int save_tree_in_file(bin_tree**, char*);
int record(node_bin_tree*, FILE*);
int unload_tree_from_file(bin_tree**, char*);
int add_tree_from_file(bin_tree**, FILE*);
void free_bin_tree(node_bin_tree**);

#endif // BIN_TREE_H