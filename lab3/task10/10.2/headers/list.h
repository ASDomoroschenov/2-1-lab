#ifndef LIST_H
#define LIST_H

#include "types.h"

int init_node_list(Node**, char*);
int push_back(List**, char*);
int get_back(List**);
int get_front(List**);
void free_list(List**);

#endif // LIST_H