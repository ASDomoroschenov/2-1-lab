#ifndef HASH_H
#define HASH_H

#include "types.h"

#define HASHSIZE 128

int is_valid_macro(char*);
long long hash(char*);
long long hash_func(char*, int);
int create_table(hash_table*, int);
int add_to_table(char*, char*, hash_table*);
void set_min_max_size_list(hash_table*);
char *get_value(char*, hash_table);
int gcd(int, int);
int new_size_table(hash_table);
int rebuild_table(hash_table*);
void free_table(hash_table*);

#endif // HASH_H