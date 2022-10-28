#ifndef LIST_H
#define LIST_H

#include "types.h"

int init_node(list_node**, citizen*);
int cmp(citizen*, citizen*);
int add_node(list**, citizen*, int (*cmp)(citizen*, citizen*));
int delete_node(list**, list_node*);
void free_node(list_node**);
void free_list(list**);

#endif // LIST_H