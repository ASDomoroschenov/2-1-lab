#include <stdlib.h>
#include <string.h>
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

int get_str(char **str, FILE *input) {
	char symb = fgetc(input);
	int size = 0;

	while (symb != '\n' && symb != EOF) {
		if (join_symb(symb, str, &size) != SUCCESS) {
			return NO_MEMORY;
		}
		
		symb = fgetc(input);
	}

	if (*str) {
		*str = (char*)realloc(*str, sizeof(char) * (strlen(*str) + 1));
	}

	return SUCCESS;
}

int get_token_str(char **ptr, char **token) {
	int size = 0;

	while (isspace(**ptr)) {
		(*ptr)++;
	}

	while (!isspace(**ptr) && **ptr && **ptr != EOF) {
		if (join_symb(**ptr, token, &size) == NO_MEMORY) {
			return NO_MEMORY;
		}
		(*ptr)++;
	}

	if (*token) {
		*token = (char*)realloc(*token, sizeof(char) * (strlen(*token) + 1));
	}

	return SUCCESS;
}

int token_count(char *str) {
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

int strip(char *str, char **res_str) {
	char *ptr_left = str;
	char *ptr_right = str + strlen(str) - 1;

	while (*ptr_left == ' ') {
		ptr_left++;
	}

	while (*ptr_right == ' ') {
		ptr_right--;
	}

	*res_str = (char*)malloc(sizeof(char) * (ptr_right - ptr_left + 2));
	
	if (!*res_str) {
		return NO_MEMORY;
	}
	
	strncpy(*res_str, ptr_left, ptr_right - ptr_left + 1);
	(*res_str)[ptr_right - ptr_left + 1] = 0;

	return SUCCESS;
}

int reverse(char *str, char **reverse_res) {
	*reverse_res = (char*)malloc(sizeof(char) * (strlen(str) + 1));

	if (!*reverse_res) {
		return NO_MEMORY;
	}

	char *ptr_str = str + strlen(str) - 1;
	char *ptr_res = *reverse_res;

	while (ptr_str >= str) {
		*ptr_res++ = *ptr_str--;
	}

	*ptr_res = 0;

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
	if (exit_code == EMPTY_LIST) {
		fprintf(stderr, "%s\n", "EMPTY_LIST: List is empty");
	}
}