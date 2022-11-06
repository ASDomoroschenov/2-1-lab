#include "types.h"

int init_tree(Tree**, char);
int init_node_tree(node_tree**, char);
int add_node_subtree(node_tree**, char);
int make_tree(Tree**, char*);
void output_tree(node_tree*, FILE*);
void free_tree_nodes(node_tree**);
void free_tree(Tree**);