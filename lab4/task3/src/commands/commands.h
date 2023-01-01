#ifndef COMMANDS_H
#define COMMANDS_H

#include "../types/types.h"

int get_name_base(char*, char*, char**);
int get_expression(char*, char**);
int count_expression(array_variables, char*, int*);
int read(array_variables*, char, int);
int write(array_variables, char, int);

#endif // COMMANDS_H