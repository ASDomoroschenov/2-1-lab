#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../string/string.h"
#include "../list/list.h"

int hash(int, int);
int create_user(client**, int, int);
int create_table(hash_table*, int);
int add_user(int, int, hash_table*);
int delete_user(int, hash_table*);
void set_min_max_size_list(hash_table*);
client *get_user(int, hash_table);
int gcd(int, int);
int new_size_table(hash_table);
int rebuild_table(hash_table*);
void free_table(hash_table*);

#endif // HASH_TABLE_H