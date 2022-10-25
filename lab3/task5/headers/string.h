#ifndef STRING_H
#define STRING_H

#include <stdio.h>

enum ERRORS {
	NO_MEMORY = -1,
	SUCCESS = -2,
	INVALID_STUDENT = -3,
	OPEN_FILE = -4,
	INVALID_ARGS = -5
};

int join_symb(char, char**, int*);
int get_token_str(char**, char**);
int split_space(char*, char***, int*);
int get_str(char**, FILE*);

#endif // STRING_H