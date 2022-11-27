#ifndef STACK_CHAR_H
#define STACK_CHAR_H

#include "types.h"

int init_node_stack_char(node_stack_char**, char);
int push_char(stack_char**, char);
char pop_char(stack_char**);
int is_empty_char(stack_char*);
int free_stack_char(stack_char**);

#endif // STACK_CHAR_H