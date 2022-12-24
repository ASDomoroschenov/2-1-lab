#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../lib/peasant.h"
#include "../lib/string.h"
#include "../lib/list.h"

long hash(long, int);
int create_table(hash_table*, int);
int add_user(long, hash_table*);
void set_min_max_size_list(hash_table*);
peasant *get_user(long, hash_table);
int gcd(int, int);
int new_size_table(hash_table);
int rebuild_table(hash_table*);
void free_table(hash_table*);

#endif // HASH_TABLE_H