#ifndef STACK_CHAR_H
#define STACK_CHAR_H

#include "types.h"

int init_node_stack(node_stack**, void*, int);
int push(stack_t**, void*, int);
int pop(stack_t**);
void *pick(stack_t*);
int is_empty(stack_t*);
int free_stack(stack_t**);

#endif // STACK_CHAR_H