#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../headers/string.h"
#include "../headers/student.h"

int main(int argc, char *argv[]) {
	student **found_students = NULL;
	student **arr_students = NULL;
	int count_students = 0;
	int count_found_students = 0;

	get_arr_student("test.txt", &arr_students, &count_students);
	qsort(arr_students, count_students, sizeof(student*), cmp_id);
	output_students(arr_students, count_students);
	free_arr_students(&arr_students, count_students);
	free(found_students);

	return 0;
}