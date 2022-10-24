#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>

enum FLAGS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	INVALID_STR = -3,
	FILE_DIDNT_OPEN = -4
};

typedef struct {
	size_t id;
	char *name;
	char *surname;
	double wage;
} employee;

int join_symb(char, char**, size_t*);
int get_line(FILE*, char**);
int word_count(char*);
int get_token_str(char **str, char **token);
int check_id(char*);
int check_name(char*);
int check_surname(char*);
int check_wage(char*);
int get_employee_info(char*, employee**);
int get_workers(char*, employee***, size_t*);
void free_worker(employee**);
void free_arr_workers(employee***, size_t);

#endif // EMPLOYEE_H