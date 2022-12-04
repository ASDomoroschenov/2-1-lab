#ifndef COMMANDS_H
#define COMMANDS_H

#include "types.h"
#include "hash_table.h"

int make_command(char*, char**);
int Load(char, char*, hash_table**);
int Save(char, char*, hash_table*);
int Rand(char, int, int, int, hash_table**);
int Concat(char, char, hash_table*);
int Free(char, hash_table*);
int Remove(char, int, int, hash_table*);
int Copy(char, int, int, char, hash_table**);
int ascending(const void*, const void*);
int descending(const void*, const void*);
int Sort_ascending(char, hash_table*);
int Sort_descending(char, hash_table*);
int Shuffle(char, hash_table*);
int Stats_max_min(Array*, int*, int*);
int Stats_often(Array*, int*);
int Stats_avarage(Array*, double*);
int Stats_max_deviation(Array*, double*);
int Stats(char, hash_table*);
int Print(char, int, int, hash_table*);
int Print_all(char, hash_table*);

#endif // COMMANDS_H