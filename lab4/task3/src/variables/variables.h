#ifndef VARIABLES_H
#define VARIABLES_H

#include "../types/types.h"

int get_next_var(char*, int*, char*);
int find_var(array_variables, char);
int get_value_var(array_variables, char, int*);
int add_var(array_variables*, char, int);
int update_var(array_variables*, char, int);
int compare(const void*, const void*);
void output_vars(array_variables);

#endif //VARIABLES_H