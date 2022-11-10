#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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

	int len = strlen(*str);

	if (len == *size - 1) {
		*size *= 2;
		char *temp = (char*)realloc(*str, sizeof(char) * *size);
		
		if (!temp) {
			return NO_MEMORY;
		}

		*str = temp;
	}

	(*str)[len + 1] = 0;
	(*str)[len] = symb;

	return SUCCESS;
}

int get_str(char **str, FILE *read_file) {
	int size = 0;
	char symb = fgetc(read_file);
	int exit_code = 0;

	if (*str) {
		free(*str);
		*str = NULL;
	}

	while (symb != EOF && symb != '\n') {
		exit_code = join_symb(symb, str, &size);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		symb = fgetc(read_file);
	}

	if (*str) {
		char *temp = (char*)realloc(*str, sizeof(char) * (strlen(*str) + 1));

		if (!temp) {
			return NO_MEMORY;
		}

		*str = temp;
	}

	return SUCCESS;
}

int strip(char **str) {
	char *res_str = NULL;
	char *ptr_left = *str;
	char *ptr_right = *str + strlen(*str) - 1;

	while (isspace(*ptr_left)) {
		ptr_left++;
	}

	while (isspace(*ptr_right)) {
		ptr_right--;
	}

	res_str = (char*)malloc(sizeof(char) * (ptr_right - ptr_left + 2));
	
	if (!res_str) {
		return NO_MEMORY;
	}
	
	strncpy(res_str, ptr_left, ptr_right - ptr_left + 1);
	res_str[ptr_right - ptr_left + 1] = 0;
	free(*str);
	*str = res_str;

	return SUCCESS;
}