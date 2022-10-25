#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum ERRORS {
	NO_MEMORY = -1,
	SUCCESS = -2,
	INVALID_STUDENT = -3,
	OPEN_FILE = -4,
	INVALID_ARGS = -5
};

typedef struct {
	int id;
	char *name;
	char *surname;
	int course;
	char *group;
	int grades[5];
} student;

int join_symb(char, char**, int*);
int get_token_str(char**, char**);
int split_space(char*, char***, int*);
int token_count(char*);
int get_str(char**, FILE*);
int check_number(char*);
int check_name(char*);
int check_surname(char*);
int check_course(char*);
int check_grade(char*);
int check_student(char*);
int split(char*, char***);
int get_student(char*, student**);
int get_arr_student(char*,student***,int*);
void free_arr_students(student***, int);

int main(int argc, char *argv[]) {
	student **arr_students = NULL;
	int count_students = 0;

	get_arr_student("test.txt", &arr_students, &count_students);

	for (int i = 0; i < count_students; i++) {
		printf("%d %s %s %d %s ", arr_students[i]->id, arr_students[i]->name, arr_students[i]->surname, arr_students[i]->course, arr_students[i]->group);
		for (int j = 0; j < 5; j++) {
			printf("%d ", arr_students[i]->grades[j]);
		}
		printf("\n");
	}

	free_arr_students(&arr_students, count_students);

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

	if (split_space(str, &student_tokens, &count_tokens) != SUCCESS) {
		return NO_MEMORY;
	}

	if (!check_number(student_tokens[0]) ||
		!check_name(student_tokens[1]) ||
		!check_surname(student_tokens[2]) ||
		!check_course(student_tokens[3]) ||
		!check_number(student_tokens[5]) ||
		!check_number(student_tokens[6]) ||
		!check_number(student_tokens[7]) ||
		!check_number(student_tokens[8]) ||
		!check_number(student_tokens[9])) {
		

		for (int i = 0; i < count_tokens; i++) {
			free(student_tokens[i]);
		}
		free(student_tokens);

		return 0;
	}

	for (int i = 0; i < count_tokens; i++) {
			free(student_tokens[i]);
	}
	free(student_tokens);

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
			char *str =  NULL;
			exit_code_get_str = get_str(&str, file_students);

			if (exit_code_get_str != SUCCESS) {
				return exit_code_get_str;
			}

			exit_code_check = check_student(str);

			if (exit_code_check != 1) {
				return exit_code_check;
			}

			if (index == size) {
				size = size ? size * 2 : 1;
				*students = (student**)realloc(*students, sizeof(student*) * size);
				
				if (!*students) {
					return NO_MEMORY;
				}
			}

			if (get_student(str, &(*students)[index]) != SUCCESS) {
				return NO_MEMORY;
			}

			index++;
			free(str);
		}
		fclose(file_students);
	} else {
		return OPEN_FILE;
	}

	*count_students = index;

	return SUCCESS;
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