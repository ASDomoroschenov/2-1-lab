#include "../headers/student.h"
#include "../headers/string.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int check_number(char *number) {
	char *ptr = number - 1;

	while (*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return atoi(number) > 0;
}

int check_name(char *name) {
	char *ptr = name - 1;

	while (*++ptr) {
		if (!isalpha(*ptr)) {
			return 0;
		}
	}

	return 1;
}

int check_surname(char *surname) {
	char *ptr = surname - 1;

	while (*++ptr) {
		if (!isalpha(*ptr)) {
			return 0;
		}
	}

	return 1;
}

int check_course(char *course) {
	return check_number(course) && (1 <= atoi(course) && atoi(course) <= 4);
}

int check_grade(char *grade) {
	return check_number(grade) && (2 <= atoi(grade) && atoi(grade) <= 5);
}

int check_student(char *str) {
	char **student_tokens = NULL;
	int count_tokens = 0;

	if (str && split_space(str, &student_tokens, &count_tokens) != SUCCESS) {
		return NO_MEMORY;
	}

	if (count_tokens != 10 ||
		!check_number(student_tokens[0]) ||
		!check_name(student_tokens[1]) ||
		!check_surname(student_tokens[2]) ||
		!check_course(student_tokens[3]) ||
		!check_number(student_tokens[5]) ||
		!check_number(student_tokens[6]) ||
		!check_number(student_tokens[7]) ||
		!check_number(student_tokens[8]) ||
		!check_number(student_tokens[9])) {

		if (student_tokens) {
			for (int i = 0; i < count_tokens; i++) {
				free(student_tokens[i]);
			}
			free(student_tokens);
		}

		return 0;
	}

	if (student_tokens) {
		for (int i = 0; i < count_tokens; i++) {
			free(student_tokens[i]);
		}
		free(student_tokens);
	}

	return 1;
}

int get_student(char *str, student **item) {
	char **student_tokens = NULL;
	int count_tokens = 0;

	*item = (student*)malloc(sizeof(student));

	if (!*item) {
		return NO_MEMORY;
	}

	if (split_space(str, &student_tokens, &count_tokens) != SUCCESS) {
		return NO_MEMORY;
	}

	(*item)->id = atoi(student_tokens[0]);
	free(student_tokens[0]);
	(*item)->name = student_tokens[1];
	(*item)->surname = student_tokens[2];
	(*item)->course = atoi(student_tokens[3]);
	free(student_tokens[3]);
	(*item)->group = student_tokens[4];
	for (int i = 0; i < 5; i++) {
		(*item)->grades[i] = atoi(student_tokens[5 + i]);
		free(student_tokens[5 + i]);
	}
	free(student_tokens);

	return SUCCESS;
}

int get_arr_student(char *name_file, student ***students, int *count_students) {
	FILE *file_students = NULL;
	int size = 0;
	int index = 0;
	int exit_code_check = 0;
	int exit_code_get_str = 0;

	if ((file_students = fopen(name_file, "r")) != NULL) {
		while (!feof(file_students)) {
			char *str = NULL;
			exit_code_get_str = get_str(&str, file_students);

			if (exit_code_get_str != SUCCESS) {
				free(str);
				fclose(file_students);
				return exit_code_get_str;
			}

			exit_code_check = check_student(str);

			if (exit_code_check != 1) {
				free(str);
				fclose(file_students);
				return exit_code_check;
			}

			if (index == size) {
				size = size ? size * 2 : 1;
				*students = (student**)realloc(*students, sizeof(student*) * size);
				
				if (!*students) {
					free(str);
					fclose(file_students);
					return NO_MEMORY;
				}
			}

			if (get_student(str, &(*students)[index]) != SUCCESS) {
				free(str);
				fclose(file_students);
				return NO_MEMORY;
			}

			index++;
			(*count_students)++;
			free(str);
		}
		fclose(file_students);
	} else {
		return OPEN_FILE;
	}

	return SUCCESS;
}

int find_id_students(int id, student **students, int count_students, student ***found_students, int *count_found_students) {
	int size = 0;

	for (int i = 0; i < count_students; i++) {
		if (students[i]->id == id) {
			if (*count_found_students == size) {
				size = size ? size * 2 : 1;
				*found_students = (student**)realloc(*found_students, sizeof(student*) * size);
				if (!*found_students) {
					return NO_MEMORY;
				}
			}
			(*found_students)[(*count_found_students)++] = students[i];
		}
	}

	return SUCCESS;
}

int find_surname_students(char *surname, student **students, int count_students, student ***found_students, int *count_found_students) {
	int size = 0;

	for (int i = 0; i < count_students; i++) {
		if (strcmp(students[i]->surname, surname) == 0) {
			if (*count_found_students == size) {
				size = size ? size * 2 : 1;
				*found_students = (student**)realloc(*found_students, sizeof(student*) * size);
				if (!*found_students) {
					return NO_MEMORY;
				}
			}
			(*found_students)[(*count_found_students)++] = students[i];
		}
	}

	return SUCCESS;
}

int find_name_students(char *name, student **students, int count_students, student ***found_students, int *count_found_students) {
	int size = 0;

	for (int i = 0; i < count_students; i++) {
		if (strcmp(students[i]->name, name) == 0) {
			if (*count_found_students == size) {
				size = size ? size * 2 : 1;
				*found_students = (student**)realloc(*found_students, sizeof(student*) * size);
				if (!*found_students) {
					return NO_MEMORY;
				}
			}
			(*found_students)[(*count_found_students)++] = students[i];
		}
	}

	return SUCCESS;
}

int find_group_students(char *group, student **students, int count_students, student ***found_students, int *count_found_students) {
	int size = 0;

	for (int i = 0; i < count_students; i++) {
		if (strcmp(students[i]->group, group) == 0) {
			if (*count_found_students == size) {
				size = size ? size * 2 : 1;
				*found_students = (student**)realloc(*found_students, sizeof(student*) * size);
				if (!*found_students) {
					return NO_MEMORY;
				}
			}
			(*found_students)[(*count_found_students)++] = students[i];
		}
	}

	return SUCCESS;
}

int cmp_id(const void *item_1, const void *item_2) {
	return (*((student**)(item_1)))->id > (*((student**)(item_2)))->id;
}

int cmp_name(const void *item_1, const void *item_2) {
	return strcmp((*((student**)(item_1)))->name, (*((student**)(item_2)))->name);
}

int cmp_surname(const void *item_1, const void *item_2) {
	return strcmp((*((student**)(item_1)))->surname, (*((student**)(item_2)))->surname);
}

int cmp_group(const void *item_1, const void *item_2) {
	return strcmp((*((student**)(item_1)))->group, (*((student**)(item_2)))->group);
}

void output_students(student **arr_students, int count_students) {
	for (int i = 0; i < count_students; i++) {
		printf("%d %s %s %d %s ", arr_students[i]->id, arr_students[i]->name, arr_students[i]->surname, arr_students[i]->course, arr_students[i]->group);
		for (int j = 0; j < 5; j++) {
			printf("%d ", arr_students[i]->grades[j]);
		}
		printf("\n");
	}
}

void free_arr_students(student ***arr_students, int count_students) {
	for (int i = 0; i < count_students; i++) {
		free((*arr_students)[i]->name);
		free((*arr_students)[i]->surname);
		free((*arr_students)[i]->group);
		free((*arr_students)[i]);
	}

	free(*arr_students);
}