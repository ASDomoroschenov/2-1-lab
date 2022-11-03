#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/string.h"

int join_symb(char symb, char **str, int *size) {
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
		char *temp = (char*)realloc(*str, sizeof(char) * *size);
		
		if (!temp) {
			return NO_MEMORY;
		}

		*str = temp;
	}

	(*str)[strlen(*str) + 1] = 0;
	(*str)[strlen(*str)] = symb;

	return SUCCESS;
}

int is_bad(int c) {
	return !isalnum(c);
}

int is_good(int c) {
	return isalnum(c);
}

int get_token(char **token, FILE *read_file, int (*is_bad)(int), int (*is_good)(int)) {
	char ch = 0;
	char _c = 0;
	int size = 0;

	while (!feof(read_file) && !(is_bad(ch) && is_good(_c))) {
		_c = ch;
		ch = fgetc(read_file);
		if (is_good(ch)) {
			int exit_code = join_symb(ch, token, &size);
			if (exit_code != SUCCESS) {
				return exit_code;
			}
		}
		if (is_bad(ch) && is_good(_c)) {
			char *temp = (char*)realloc(*token, sizeof(char) * (strlen(*token) + 1));
			
			if (!temp) {
				return NO_MEMORY;
			}

			*token = temp;
		}
	}

	return SUCCESS;
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
	}
	if (exit_code == OPEN_FILE) {
		fprintf(stderr, "%s\n", "FILE_DIDNT_OPEN: Can't open file");
	}
	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "%s\n", "INVALID_ARGS: Run with invalid arguments");
	}
	if (exit_code == NOT_TXT_FILE) {
		fprintf(stderr, "%s\n", "NOT_TXT_FILE: Argument is not text file");
	}
}