#ifndef COMMANDS_H
#define COMMANDS_H

#include "../types/types.h"

int delete_sep(char**);
int is_digital_value(char*);
int get_operator(char*, char**);
int get_left_operand(char*, char**);
int get_right_operand(char*, char**);
int get_name_base(char*, char*, char**);
int get_expression(char*, char**);
int count_expression(array_variables, char*, int*);
int set_value(array_variables*, char, int);
int set_var(array_variables*, char*);

#endif // COMMANDS_H