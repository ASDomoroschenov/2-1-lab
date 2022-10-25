#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/string.h"

int join_symb(char symb, char **str, size_t *size) {
	if (*size == 0) {
		*size = 2;
		*str = (char*)malloc(sizeof(char) * *size);
		if (!*str) {
			return NO_MEMORY;
		}

		(*str)[0] = symb;
		(*str)[1] = 0;

		return SUCCESS;
	}

	if (strlen(*str) == *size - 1) {
		*size *= 2;
		*str = (char*)realloc(*str, sizeof(char) * *size);
		if (!*str) {
			return NO_MEMORY;
		}
	}

	(*str)[strlen(*str) + 1] = 0;
	(*str)[strlen(*str)] = symb;

	return SUCCESS;
}

int get_token_str(char **ptr, char **token) {
	size_t size = 0;

	while (isspace(**ptr)) {
		(*ptr)++;
	}

	while (!isspace(**ptr) && **ptr && **ptr != EOF) {
		if (join_symb(**ptr, token, &size) == NO_MEMORY) {
			return NO_MEMORY;
		}
		(*ptr)++;
	}

	return SUCCESS;
}

int token_count(char *str) {
	char _c = ' ';
	char *ptr = str - 1;
	size_t count = 0;

	while (*++ptr) {
		if (isspace(_c) && !isspace(*ptr)) {
			count++;
		}
		_c = *ptr;
	}

	return count;
}

int get_str(char **str, FILE *input) {
	char symb = fgetc(input);
	int exit_code = 0;
	size_t size = 0;

	while (symb != '\n') {
		exit_code = join_symb(symb, str, &size);
		
		if (exit_code != SUCCESS) {
			return exit_code;
		}
		
		symb = fgetc(input);
	}

	return SUCCESS;
}

int check_id(char *id) {
	char *ptr = id - 1;

	while (*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return atoi(id) > 0;
}