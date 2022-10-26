#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../headers/string.h"
#include "../headers/student.h"

int main(int argc, char *argv[]) {
	student **found_students = NULL;
	student **arr_students = NULL;
	list **list_students = NULL;
	int count_students = 0;
	int count_found_students = 0;
	double avarage_grade = 0;
	int exit_code = 0;

	exit_code = get_arr_student(argv[1], &arr_students, &count_students);

	if (exit_code == SUCCESS) {
		qsort(arr_students, count_students, sizeof(student*), cmp_id);
		avarage_grade = get_avarage_grades_student(arr_students, count_students);
		list_students = grouping_by_course(arr_students, count_students);
		
		if (list_students != NULL) {
			exit_code = write_in_files(argv[1], list_students, avarage_grade);
			
			if (exit_code == SUCCESS) {
				print_error(exit_code);
			}
			
			output_list_students(list_students);
			free_list_students(&list_students);
			free(found_students);
		} else {
			print_error(NO_MEMORY);
		}
	} else {
		print_error(exit_code);
	}

	free_arr_students(&arr_students, count_students);

	return 0;
}