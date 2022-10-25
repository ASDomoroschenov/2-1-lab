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
int cmp_id(const void *item_1, const void *item_2);
int cmp_name(const void *item_1, const void *item_2);
int cmp_surname(const void *item_1, const void *item_2);
int cmp_group(const void *item_1, const void *item_2);
void output_students(student**, int);
void free_arr_students(student***, int);

#endif // STIDENT_H