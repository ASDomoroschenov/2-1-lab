#ifndef STRING_H
#define STRING_H

#include "types.h"
#include <stdio.h>

int join_symb(char, char**, int*);
int is_bad(int);
int is_good(int);
int get_token(char**, FILE*, int (*is_bad)(int), int (*is_good)(int));
void print_error(int);

#endif // STRING_H