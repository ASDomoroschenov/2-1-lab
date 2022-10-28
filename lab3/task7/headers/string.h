#ifndef STRING_H
#define STRING_H

#include "types.h"
#include <stdio.h>

int join_symb(char, char**, int*);
int get_str(char**, FILE*);
int get_token_str(char**, char**);
int token_count(char*);
int strip(char*, char**);
int reverse(char*, char**);
void print_error(int);

#endif // STRING_H