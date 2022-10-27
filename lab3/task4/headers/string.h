#ifndef STRING_H
#define STRING_H

#include <stdio.h>

enum ERRORS {
	NO_MEMORY = -1,
	SUCCESS = -2,
	INVALID_MESSAGE = -3,
	OPEN_FILE = -4,
	INVALID_ARGS = -5
};

int join_symb(char, char**, size_t*);
int get_token_str(char**, char**);
int token_count(char*);
int get_str(char**, FILE*);
int strip(char*, char**);

#endif // STRING_H