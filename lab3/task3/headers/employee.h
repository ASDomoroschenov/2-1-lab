#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <stdio.h>

enum FLAGS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	INVALID_STR = -3,
	FILE_DIDNT_OPEN = -4,
	INVALID_ARGS = -5
};

typedef struct {
	int id;
	char *name;
	char *surname;
	double wage;
} employee;

int join_symb(char, char**, int*);
int get_line(FILE*, char**);
int word_count(char*);
int get_token_str(char **str, char **token);
int check_id(char*);
int check_name(char*);
int check_surname(char*);
int check_wage(char*);
int get_employee_info(char*, employee**);
int get_workers(char*, employee***, int*);
int write_in_file(char*, employee**, int);
void free_worker(employee**);
void free_arr_workers(employee***, int);
void output_workers(employee**, int);
void print_error(int);

#endif // EMPLOYEE_H