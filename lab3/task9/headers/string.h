#ifndef STRING_H
#define STRING_H

#include "types.h"
#include <stdio.h>

int join_symb(char, char**, int*);
int get_token(char**, FILE*);
int get_str(char**, FILE*);
int token_count(char*);
int strip(char**);
int create_str(string*, char*);
int get_T(T**, char*);
void print_str(string str, FILE *output);
void free_str(string*);

#endif // STRING_H