#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "task6.h"

int main() {
	int res = 0;
	int count = 2;
	int sizes[count];
	coord_tokens **coordinates = (coord_tokens**)malloc(sizeof(coord_tokens*) * count);
	char **filenames = (char**)malloc(sizeof(char*) * count);

	res = find_substr(filenames, coordinates, sizes, "123", count, "test1.txt", "test2.txt");

	if (res != SUCCESS) {
		if (res == NO_MEMORY) {
			fprintf(stderr, "%s\n", "No memory");
		}
		if (res == CANT_OPEN_FILE) {
			fprintf(stderr, "%s\n", "Can't open file");
		}
	} else {
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < sizes[i]; j++) {
				printf("%s %d:%d\n", filenames[i], coordinates[i][j].count_line, coordinates[i][j].count_symb);
			}
			if (i != count - 1) {
				printf("\n");
			}
		}
	}

	free_tokens(coordinates, count);
	free_filenames(filenames, count);

	return 0;
}

int find_substr(char **filenames, coord_tokens **tokens, int *sizes, char *str, int count, ...) {
	va_list args;
	va_start(args, count);
	char *filename = NULL;
	int res = 0;

	for (int i = 0; i < count; i++) {
		filename = va_arg(args, char*);
		filenames[i] = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
		strcpy(filenames[i], filename);
		res = find_substr_in_file(&tokens[i], str, filename);

		if (res != CANT_OPEN_FILE && res != NO_MEMORY) {
			sizes[i] = res;
		} else {
			return res;
		}
	}

	va_end(args);

	return SUCCESS;
}

int find_substr_in_file(coord_tokens **tokens, char *substr, char *filename) {
	FILE *file = NULL;
	char *token = NULL;
	bool is_new_line = false;
	int res_substr = 0;
	int count_symb = 0;
	int count_line = 1;
	int index = 0;
	int size = 0;

	if ((file = fopen(filename, "r")) != NULL) {
		while (!feof(file)) {
			if (get_token(&token, file, &is_new_line, &count_symb) == SUCCESS) {
				if (token) {
					res_substr = my_substr(token, substr);
					if (res_substr != NO_MEMORY) {
						if (res_substr >= 0) {
							if (index == size) {
								size = size == 0 ? 1 : size;
								*tokens = (coord_tokens*)realloc(*tokens, sizeof(coord_tokens) * size * 2);
								size *= 2;
							}
							(*tokens)[index].count_line = count_line;
							(*tokens)[index].count_symb = count_symb - strlen(token) + res_substr;
							index++;
						}
					} else {
						return NO_MEMORY;
					}
					free(token);
					token = NULL;
				}
				if (is_new_line) {
					count_line++;
					count_symb = 0;
				}
			} else {
				return NO_MEMORY;
			}
		}

		fclose(file);
	} else {
		return CANT_OPEN_FILE;
	}

	return index;
}

int my_substr(char *str, char *sub) {
	char *buff = (char*)malloc(sizeof(char) * (strlen(sub) + 1));

	if (buff) {
		if (strlen(str) >= strlen(sub)) {
			for (int i = 0; i <= strlen(str) - strlen(sub); i++) {
				for (int j = 0; j < strlen(sub); j++) {
					buff[j] = str[j + i];
				}
				buff[strlen(sub)] = 0;

				if (strcmp(buff, sub) == 0) {
					free(buff);
					return i;
				}
			}
		}
		free(buff);
	} else {
		return NO_MEMORY;
	}

	return -1;
}

int join_symb(char **str, int *size, char symb) {
	if (*str) {
		if (strlen(*str) == *size - 1) {
			*str = (char*)realloc(*str, sizeof(char) * *size * 2);
			*size *= 2;
		}
		if (*str) {
			(*str)[strlen(*str) + 1] = 0;
			(*str)[strlen(*str)] = symb;
		} else {
			return NO_MEMORY;
		}
	} else {
		*str = (char*)malloc(sizeof(char) * 2);
		if (*str) {
			(*str)[0] = symb;
			(*str)[1] = 0;
			*size = 2;
		} else {
			return NO_MEMORY;
		}
	}

	return SUCCESS;
}

int get_token(char **token ,FILE *file, bool *is_new_line, int *count_symb) {
	char symb = fgetc(file);
	int size = 0;
	(*count_symb)++;

	while (!isspace(symb) && symb != EOF) {
		if (join_symb(token, &size, symb) == NO_MEMORY) {
			return NO_MEMORY;
		}
		symb = fgetc(file);
		(*count_symb)++;
	}

	*is_new_line = symb == '\n';

	return SUCCESS;
}

void free_tokens(coord_tokens **tokens, int count) {
	for (int i = 0; i < count; i++) {
		free(tokens[i]);
	}

	free(tokens);
}

void free_filenames(char **filenames, int count) {
	for (int i = 0; i < count; i++) {
		free(filenames[i]);
	}

	free(filenames);
}