#include <stdio.h>
#include <stdlib.h>
#include "../lib/execute.h"

void print_error(int, int);

int main(int argc, char *argv[]) {
	int line_error = 1;
	int exit_code = 0;
	exit_code = execute_instructions("instructions.txt", &line_error);

	if (exit_code != SUCCESS) {
		print_error(exit_code, line_error);

		return exit_code;
	}

	return 0;
}

void print_error(int exit_code, int line_error) {
	char *errors[] = {
		"NO_MEMORY - The system is out of memory",
		"EMPTY_STRING - Can't do calculations with empty string",
		"DIVISION_BY_ZERO - Division by zero in an expression",
		"INVALID_BRACKETS - Invalid brackets",
		"INVALID_EXPRESSION - Invalid expression",
		"OPEN_FILE - Can't open file",
		"HAVENT_VAR - There is a nonexistent variable in the expression",
		"EMPTY_ARRAY - Can't do calculations with empty array"
	};

	fprintf(stderr, "line %d: %s\n", line_error, errors[abs(exit_code) - 2]);
}