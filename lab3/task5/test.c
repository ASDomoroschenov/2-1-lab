#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[]) {
	char *str = "123 123 123";
	char **arr_token = NULL;
	int length = 0;

	if (split(str, &arr_token, &length) == SUCCESS) {
		for (int i = 0; i < length; i++) {
			printf("%s\n", arr_token[i]);
		}
	}

	return 0;
}

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
	int size = 0;

	while (isspace(**ptr)) {
		(*ptr)++;
	}

	while (!isspace(**ptr) && **ptr) {
		if (join_symb(**ptr, token, &size) == NO_MEMORY) {
			return NO_MEMORY;
		}
		(*ptr)++;
	}

	return SUCCESS;
}

int split_space(char *str, char ***arr_token, int *length) {
	int size = 0;
	int index = 0;
	char *ptr = str;
	char *token = NULL;

	while (*ptr) {
		if (get_token_str(&ptr, &token) != SUCCESS) {
			return NO_MEMORY;
		}

		if (index == size) {
			size = size ? size * 2 : 1;
			*arr_token = (char**)realloc(*arr_token, sizeof(char*) * size);

			if (!*arr_token) {
				return NO_MEMORY;
			}
		}

		(*arr_token)[index++] = token;
	}

	*length = index;

	return SUCCESS;
}