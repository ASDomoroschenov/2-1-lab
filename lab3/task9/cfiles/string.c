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

int get_token(char **token, FILE *read_file) {
	char ch = 0;
	char _c = 0;
	int size = 0;

	while (!feof(read_file) && !(!isalnum(ch) && isalnum(_c))) {
		_c = ch;
		ch = fgetc(read_file);
		if (isalnum(ch)) {
			int exit_code = join_symb(ch, token, &size);
			if (exit_code != SUCCESS) {
				return exit_code;
			}
		}
		if (!isalnum(ch) && isalnum(_c)) {
			char *temp = (char*)realloc(*token, sizeof(char) * (strlen(*token) + 1));
			
			if (!temp) {
				return NO_MEMORY;
			}

			*token = temp;
		}
	}

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

int create_str(string *destination, char *source) {
	if (source) {
		if (destination->text) {
			free(destination->text);
			destination->length = 0;
		}

		destination->length = strlen(source);
		destination->text = (char*)malloc(sizeof(char) * destination->length);

		if (!destination->text) {
			return NO_MEMORY;
		}

		strncpy(destination->text, source, destination->length);

		return SUCCESS;
	}

	return SUCCESS;
}

int get_T(T **item, char *str) {
	char *ptr = str;
	char *prior = NULL;
	char *text = NULL;
	int exit_code = 0;
	int size = 0;

	while (*ptr != '=') {
		ptr++;
	}

	ptr++;

	while (!isspace(*ptr)) {
		exit_code = join_symb(*ptr, &prior, &size);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
		ptr++;
	}

	char *temp = (char*)realloc(prior, sizeof(char) * (strlen(prior) + 1));

	if (!temp) {
		return NO_MEMORY;
	}

	prior = temp;
	size = 0;

	while (isspace(*ptr)) {
		ptr++;
	}

	while (*ptr != '=') {
		ptr++;
	}

	ptr++;

	while (*ptr) {
		exit_code = join_symb(*ptr, &text, &size);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
		ptr++;
	}

	temp = (char*)realloc(text, sizeof(char) * (strlen(text) + 1));

	if (!temp) {
		free(prior);
		return NO_MEMORY;
	}

	text = temp;

	*item = (T*)malloc(sizeof(T));
	(*item)->prior = 0;
	(*item)->text_field.text = NULL;

	if (!*item) {
		free(prior);
		free(text);
		return NO_MEMORY;
	}

	(*item)->prior = atoi(prior);

	exit_code = create_str(&((*item)->text_field), text);

	if (exit_code != SUCCESS) {
		free(prior);
		free(text);
		return NO_MEMORY;
	}

	free(prior);
	free(text);

	return SUCCESS;
}

void print_str(string str, FILE *output) {
	for (int i = 0; i < str.length; i++) {
		fprintf(output, "%c", str.text[i]);
	}
	fprintf(output, "\n");
}

void free_str(string *str) {
	if (str) {
		if (str->text) {
			free(str->text);
			str->text = NULL;
		}
		str->length = 0;
	}
}