#ifndef STACK_H
#define STACK_H

#include "types.h"

int create_stack(stack_t**, enum TYPE);
int init_node_stack(node_stack**, void*, enum TYPE);
int push(stack_t**, void*);
int pop(stack_t**);
void *pick(stack_t*);
int is_empty(stack_t*);
int free_stack(stack_t**);

#endif // STACK_CHAR_H