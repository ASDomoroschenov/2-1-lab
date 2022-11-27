#ifndef STACK_DOUBLE_H
#define STACK_DOUBLE_H

#include "types.h"

int init_node_stack_double(node_stack_double**, double);
int push_double(stack_double**, double);
double pop_double(stack_double**);
int is_empty_double(stack_double*);
int free_stack_double(stack_double**);

#endif // STACK_DOUBLE_H