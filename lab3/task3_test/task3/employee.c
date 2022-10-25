#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "employee.h"

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

int get_line(FILE *file_info, char **str) {
	char symb = fgetc(file_info);
	size_t size_str = 0;

	while (symb != '\n' && symb != EOF) {
		if (join_symb(symb, str, &size_str) == NO_MEMORY) {
			return NO_MEMORY;
		}
		symb = fgetc(file_info);
	}

	return SUCCESS;
}

int word_count(char *str) {
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

int get_token_str(char **ptr, char **token) {
	size_t size = 0;

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

int check_id(char *id) {
	char *ptr = id - 1;

	while (*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return 1;
}

int check_name_surname(char *name) {
	char *ptr = name - 1;

	while (*++ptr) {
		if (!isalpha(*ptr)) {
			return 0;
		}
	}

	return 1;
}

int check_wage(char *wage) {
	char *ptr = wage - 1;
	int count_dots = 0;

	while (*++ptr) {
		if (*ptr == '.') {
			count_dots++;
		} else {
			if (!isdigit(*ptr)) {
				return 0;
			}
		}

		if (count_dots > 1) {
			return 0;
		}
	}

	return 1;
}

int get_employee_info(char *str, employee **item) {
	char *ptr = str;
	*item = (employee*)malloc(sizeof(employee));

	if (word_count(str) != 4) {
		free(*item);
		return INVALID_STR;
	}

	char *id = NULL;
	char *name = NULL;
	char *surname = NULL;
	char *wage = NULL;

	int exit_id = get_token_str(&ptr, &id);
	int exit_name = get_token_str(&ptr, &name);
	int exit_surname = get_token_str(&ptr, &surname);
	int exit_wage = get_token_str(&ptr, &wage);

	if (exit_id == SUCCESS && check_id(id)) {
		(*item)->id = atoi(id);
		free(id);
	} else {
		free(id);
		free(name);
		free(surname);
		free(wage);
		free_worker(item);
		return exit_id == SUCCESS ? INVALID_STR : exit_id;
	}

	if (exit_name == SUCCESS && check_name_surname(name)) {
		(*item)->name = name;
	} else {
		free(id);
		free(name);
		free(surname);
		free(wage);
		free_worker(item);
		return exit_name == SUCCESS ? INVALID_STR : exit_name;
	}

	if (exit_surname == SUCCESS && check_name_surname(surname)) {
		(*item)->surname = surname;
	} else {
		free(id);
		free(name);
		free(surname);
		free(wage);
		free_worker(item);
		return exit_surname == SUCCESS ? INVALID_STR : exit_surname;
	}

	if (exit_wage == SUCCESS && check_wage(wage)) {
		(*item)->wage = atof(wage);
		free(wage);
	} else {
		free(id);
		free(name);
		free(surname);
		free(wage);
		free(*item);
		return exit_wage == SUCCESS ? INVALID_STR : exit_wage;
	}

	return SUCCESS;
}

int get_workers(char *file_name, employee ***workers, size_t *size) {
	FILE *file = NULL;

	if ((file = fopen(file_name, "r")) != NULL) {
		char *str = NULL;
		int exit_code = 0;
		int index = 0;
		size_t length = 1;
		*workers = (employee**)malloc(sizeof(employee*));

		while (!feof(file)) {
			if (get_line(file, &str) == NO_MEMORY) {
				return NO_MEMORY;
			}

			if (str) { // если строка не пустая
				exit_code = get_employee_info(str, &(*workers)[index]);

				if (exit_code == SUCCESS) {
					if (index == length - 1) {
						length *= 2;
						*workers = (employee**)realloc(*workers, sizeof(employee*) * length);
						if (!workers) {
							return NO_MEMORY;
						}
					}

					index++;
				}

				free(str);
			}
		}

		*size = index;
		fclose(file);

		return SUCCESS;
	}

	return FILE_DIDNT_OPEN;
}

void free_worker(employee **item) {
	if (*item) {
		if ((*item)->name) {
			free((*item)->name);
		}
		if ((*item)->surname) {
			free((*item)->surname);
		}
		free(*item);
	}
}

void free_arr_workers(employee ***workers, size_t size) {
	if (*workers) {
		for (size_t i = 0; i < size; i++) {
			if ((*workers)[i]) {
				free_worker(&(*workers)[i]);
			}
		}

		free(*workers);
	}
}

void output_workers(employee **workers, size_t size) {
	for (int i = 0; i < size; i++) {
		printf("%d %s %s %lf\n", workers[i]->id, workers[i]->name, workers[i]->surname, workers[i]->wage);
	}
	printf("\n");
}