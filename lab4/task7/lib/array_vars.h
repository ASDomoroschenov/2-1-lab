#ifndef ARRAY_VARS_H
#define ARRAY_VARS_H

#include "types.h"
#include "expression.h"

int have_operator(char*);
int cmp(const void*, const void*);
int get_name_str(char*, char**);
int get_value_str(char*, char**);
int init_var(char*, MemoryCell*);
int add_var(array_vars*, char*);
int get_var(array_vars, char*);
int get_var_value(array_vars, char*, double*);
int update_var(array_vars*, char*);
int print(char*, array_vars);
void free_array_vars(array_vars*);
void output_vars(array_vars);

#endif // ARRAY_VARS_H