#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "types.h"

unsigned long hash_func(int);
int create_hash_item(char, Array*, hash_item**);
int create_hash_table(hash_table**);
int add_to_hash_table(char, Array*, hash_table**);
hash_item *get_item(char, hash_table*);
void free_hash_item(hash_item**);
void free_hash_table(hash_table**);

#endif // HASH_TABLE_H