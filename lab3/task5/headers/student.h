#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
	int id;
	char *name;
	char *surname;
	int course;
	char *group;
	int grades[5];
} student;

typedef struct tag_list_node {
	struct tag_list_node *next;
	student *item;
} list_node;

typedef struct {
	list_node *root;
} list;

int check_number(char*);
int check_name(char*);
int check_surname(char*);
int check_course(char*);
int check_grade(char*);
int check_student(char*);
int get_student(char*, student**);
int get_arr_student(char*,student***,int*);
int find_id_students(int, student**, int, student***,int*);
int find_surname_students(char*, student**, int, student***,int*);
int find_name_students(char*, student**, int, student***,int*);
int find_group_students(char*, student**, int, student***,int*);
int cmp_id(const void*, const void*);
int cmp_name(const void*, const void*);
int cmp_surname(const void*, const void*);
int cmp_group(const void*, const void*);
double get_avarage_grade(student*);
double get_avarage_grades_student(student**, int);
int push_back(list**, student*);
list **grouping_by_course(student**, int);
int get_name_file(char**, char*, int);
int write_in_files(char*, list**, double);
void output_list(list*);
void output_student(student*, FILE*);
void output_students(student**, int);
void output_list_students(list**);
void free_list(list**);
void free_list_students(list***);
void free_arr_students(student***, int);
void print_error(int);

#endif // STUDENT_H