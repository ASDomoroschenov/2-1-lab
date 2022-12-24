#ifndef LIST_H
#define LIST_H

#include "types.h"

int create_list(list_t**);
int create_node_list(peasant*, node_list**);
int push_back(list_t**, node_list*);
void free_node(node_list**);
void free_list(list_t**);

#endif // LIST_H