#ifndef TABLE_H
#define TABLE_H

#include "types.h"
#include "check.h"
#include "bin_tree.h"
#include "operations.h"

int binary_search(array_var_names, char);
int get_variables(array_var_names*, char*);
int count_expression(node_bin_tree*, array_var_names);
int make_table(char*, table_t*, array_var_names*);
void output_table(table_t, array_var_names, char*);
void free_table(table_t*);

#endif // TABLE_H