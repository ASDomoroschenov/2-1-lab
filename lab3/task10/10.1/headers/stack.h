#ifndef STACK_H
#define STACK_H

#include "types.h"

int init_root_stack(Stack**, node_stack*);
int init_node_stack(node_stack**, node_tree*);
int add_stack(Stack**, node_tree*);
int pop_stack(Stack**);

#endif // STACK_H