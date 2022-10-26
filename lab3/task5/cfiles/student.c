#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "../headers/student.h"
#include "../headers/string.h"

#define eps 0.0000000001

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
				return INVALID_STUDENT;
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

double get_avarage_grade(student *item) {
	int sum = 0;

	for (int i = 0; i < 5; i++) {
		sum += item->grades[i];
	}

	return sum / 5.0;
}

double get_avarage_grades_student(student **arr_students, int count_students) {
	int sum = 0;

	for (int i = 0; i < count_students; i++) {
		for (int j = 0; j < 5; j++) {
			sum += arr_students[i]->grades[j];
		}
	}

	return sum / (count_students * 5.0);
}

int push_back(list **list_students, student *item) {
	if (!*list_students) {
		*list_students = (list*)malloc(sizeof(list));
		if (!*list_students) {
			return NO_MEMORY;
		}
		(*list_students)->root = (list_node*)malloc(sizeof(list_node));
		if (!(*list_students)->root) {
			return NO_MEMORY;
		}
		(*list_students)->root->item = item;
		(*list_students)->root->next = NULL;
	} else {
		list_node *node = (*list_students)->root;

		while (node->next != NULL) {
			node = node->next;
		}

		node->next = (list_node*)malloc(sizeof(list_node));
		if (!(node->next)) {
			return NO_MEMORY;
		}
		node->next->item = item;
		node->next->next = NULL;
	}
}

int get_name_file(char **res, char *name_file, int course) {
	*res = (char*)malloc(sizeof(char) * (strlen("res_files/") + strlen(name_file) + 3));

	if (!*res) {
		return NO_MEMORY;
	}

	char *ptr_res = *res;
	char *ptr = name_file;
	char extension[4] = ".txt";
	char dir[10] = "res_files/";

	for (int i = 0; i < 10; i++) {
		*ptr_res++ = dir[i];
	}

	while (*ptr != '.') {
		*ptr_res++ = *ptr++;
	}

	while (*ptr != '.') {
		*ptr_res++ = *ptr++;
	}

	*ptr_res++ = '_';
	*ptr_res++ = (char)(course + '0');
	
	for (int i = 0; i < 4; i++) {
		*ptr_res++ = extension[i];
	}

	*ptr_res = 0;

	return SUCCESS;
}

int write_in_files(char *name, list **list_students, double avarage) {
	for (int i = 0; i < 4; i++) {
		char *name_file = NULL;
		if (get_name_file(&name_file, name, i + 1) != SUCCESS) {
			return NO_MEMORY;
		}

		FILE *file = NULL;

		if ((file = fopen(name_file, "w")) != NULL) {
			if (list_students[i]) {
				list_node *node = list_students[i]->root;

				while (node != NULL) {
					if (get_avarage_grade(node->item) - avarage > eps) {
						output_student(node->item, file);
					}
					node = node->next;
				}
			}

			fclose(file);
		} else {
			free(name_file);
			return OPEN_FILE;
		}

		free(name_file);
	}

	return SUCCESS;
}

list **grouping_by_course(student **arr_students, int count_students) {
	list **list_students = (list**)malloc(sizeof(list*) * 4);
	int exit_code = 0;

	for (int i = 0; i < 4; i++) {
		list_students[i] = NULL;
	}

	for (int i = 0; i < count_students; i++) {
		exit_code = push_back(&list_students[arr_students[i]->course - 1], arr_students[i]);
		
		if (exit_code == NO_MEMORY) {
			free_list_students(&list_students);
			return NULL;
		}
	}

	return list_students;
}

void output_student(student *item, FILE *out) {
	fprintf(out, "%-15s \t %-10s \t %d \t %-10s \t %lf\n", item->surname, item->name, item->course, item->group, get_avarage_grade(item));
}

void output_students(student **arr_students, int count_students) {
	for (int i = 0; i < count_students; i++) {
		output_student(arr_students[i], stdout);
	}
}

void output_list(list *list_students) {
	if (list_students) {
		list_node *node = list_students->root;

		while (node != NULL) {
			output_student(node->item, stdout);
			node = node->next;
		}

		printf("\n");
	}
}

void output_list_students(list **list_students) {
	for (int i = 0; i < 4; i++) {
		printf("%d COURSE:", i + 1);
		if (list_students[i]) {
			printf("\n");
			output_list(list_students[i]);
		} else {
			printf("There are no students\n\n");
		}
	}
}

void free_list(list **list_students) {
	if (*list_students) {
		list_node *node = (*list_students)->root;

		while (node != NULL) {
			list_node *temp = node;
			node = node->next;
			free(temp);
		}

		free(*list_students);
	}
}

void free_list_students(list ***list_students) {
	if (*list_students) {
		for (int i = 0; i < 4; i++) {
			if ((*list_students)[i]) {
				free_list(&(*list_students)[i]);
			}
		}

		free(*list_students);
	}
}

void free_arr_students(student ***arr_students, int count_students) {
	if (*arr_students) {
		for (int i = 0; i < count_students; i++) {
			free((*arr_students)[i]->name);
			free((*arr_students)[i]->surname);
			free((*arr_students)[i]->group);
			free((*arr_students)[i]);
		}

		free(*arr_students);
	}
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
	}
	if (exit_code == INVALID_STUDENT) {
		fprintf(stderr, "%s\n", "INVALID_STUDENT: You put an invalid student");
	}
	if (exit_code == OPEN_FILE) {
		fprintf(stderr, "%s\n", "OPEN_FILE: Can't open entered file");
	}
	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "%s\n", "INVALID_STUDENT: You put an invalid arguments");
	}
}