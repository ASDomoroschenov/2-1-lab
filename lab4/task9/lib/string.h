#ifndef	STRING_H
#define STRING_H

#include "types.h"
#include <stdio.h>

int join_symb(char, char**, int*);
int get_str(char**, FILE*);
int token_count(char*);
int strip(char**);
int split_space(char*, char***);
int char_to_str(int, char**);
int add_str(char**, int, ...);
int add_chr(char**, int, ...);

#endif // STRING_H