#ifndef	STRING_H
#define STRING_H

#include "types.h"
#include <stdio.h>

int join_symb(char, char**, int*);
int get_str(char**, FILE*);
int token_count(char*);
int strip(char**);
int reverse(char**);
int split_space(char*, char***);
int char_to_str(int, char**);
int add_str(char**, int, ...);
int add_chr(char**, int, ...);
long to_str(long, char**);
int to_int(char*);
int is_empty_str(char*);

#endif // STRING_H