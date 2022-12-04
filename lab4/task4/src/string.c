#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include "../lib/string.h"

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

int get_lexem(char **token, FILE *read_file) {
	char ch = 0;
	char _c = 0;
	int size = 0;

	while (!feof(read_file) && !(isspace(ch) && !isspace(_c))) {
		_c = ch;
		ch = fgetc(read_file);
		
		if (ch != EOF && !isspace(ch)) {
			int exit_code = join_symb(ch, token, &size);

			if (exit_code != SUCCESS) {
				return exit_code;
			}
		}
	}

	if (*token) {
		char *temp = (char*)realloc(*token, sizeof(char) * (strlen(*token) + 1));

		if (!temp) {
			return NO_MEMORY;
		}

		*token = temp;
	}

	return SUCCESS;
}

int get_token(char **str, char **token) {
	if (!*str) {
		*token = NULL;

		return EMPTY_STRING;
	}

	int size = 0;
	char _c = **str;
	int exit_code = 0;
	char *temp = NULL;

	if (*token) {
		free(*token);
		*token = NULL;
	}

	while (**str && isspace(**str)) {
		(*str)++;
	}

	while (**str && !isspace(**str)) {
		exit_code = join_symb(**str, token, &size);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		(*str)++;
	}

	return SUCCESS;
}

int token_count(char *str) {
	if (str) {
		char _c = ' ';
		char *ptr = str - 1;
		int count = 0;

		while (*++ptr) {
			if (isspace(_c) && !isspace(*ptr)) {
				count++;
			}
			_c = *ptr;
		}

		return count;
	}

	return 0;
}

int strip(char **str) {
	if (!*str) {
		return EMPTY_STRING;
	}

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

int split_space(char *str, char ***array) {
	if (str) {
		*array = (char**)malloc(sizeof(char*) * token_count(str));

		if (!*array) {
			return NO_MEMORY;
		}

		char *token = NULL;
		int exit_code = 0;
		int index = 0;

		while (*str) {
			exit_code = get_token(&str, &token);

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			if (token) {
				(*array)[index] = (char*)malloc(sizeof(char) * (strlen(token) + 1)); 
			
				if (!(*array)[index]) {
					free(token);
					return NO_MEMORY;
				}

				strcpy((*array)[index++], token);
				free(token);
				token = NULL;
			}
		}

		return SUCCESS;
	}

	*array = NULL;

	return SUCCESS;
}

int char_to_str(int symb, char **str) {
	if (*str) {
		free(*str);
		*str = NULL;
	}

	*str = (char*)malloc(sizeof(char) * 2);
	
	if (!*str) {
		return NO_MEMORY;
	}

	(*str)[0] = (char)symb;
	(*str)[1] = 0;

	return SUCCESS;
}

int add_str(char **str, int count, ...) {
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		char *source = va_arg(args, char*);
		if (!*str) {
			*str = (char*)malloc(sizeof(char) * (strlen(source) + 1));

			if (!*str) {
				va_end(args);
				return NO_MEMORY;
			}

			strcpy(*str, source);
		} else {
			*str = (char*)realloc(*str, sizeof(char) * (strlen(*str) + strlen(source) + 1));
		
			if (!*str) {
				va_end(args);
				return NO_MEMORY;
			}

			strcpy(*str + strlen(*str), source);
		}
	}

	va_end(args);

	return SUCCESS;
}

int add_chr(char **str, int count, ...) {
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		char symb = va_arg(args, int);
		char *temp = NULL;
		int exit_code = char_to_str(symb, &temp);

		if (exit_code != SUCCESS) {
			va_end(args);
			return exit_code;
		}

		exit_code = add_str(str, 1, temp);

		if (exit_code != SUCCESS) {
			va_end(args);
			free(temp);
			return exit_code;
		}

		free(temp);
	}

	va_end(args);

	return SUCCESS;
}