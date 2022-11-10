#ifndef STRING_H
#define STRING_H

#include "types.h"
#include <stdio.h>

int join_symb(char, char**, int*);
int get_str(char**, FILE*);
int strip(char**);

#endif // STRING_H