#include "../headers/string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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
	char *ptr = str;
	char *token = NULL;
	
	while (*ptr) {
		if (get_token_str(&ptr, &token) != SUCCESS) {
			return NO_MEMORY;
		}

		if (token) {
			if (*length == size) {
				size = size ? size * 2 : 1;
				*arr_token = (char**)realloc(*arr_token, sizeof(char*) * size);

				if (!*arr_token) {
					return NO_MEMORY;
				}
			}
			(*arr_token)[*length] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy((*arr_token)[*length], token);
			free(token);
			token = NULL;
			(*length)++;
		}
	}

	return SUCCESS;
}

int get_str(char **str, FILE *input) {
	char symb = fgetc(input);
	int exit_code = 0;
	int size = 0;

	while (symb != '\n' && symb != EOF) {
		exit_code = join_symb(symb, str, &size);
		
		if (exit_code != SUCCESS) {
			return exit_code;
		}
		
		symb = fgetc(input);
	}

	return SUCCESS;
}