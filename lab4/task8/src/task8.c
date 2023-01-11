#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types/types.h"
#include "string/string.h"
#include "check/check.h"
#include "gets/gets.h"
#include "configuration/configuration.h"
#include "interpreter/interpreter.h"

void print_error(int, int, char*);

int main(int argc, char *argv[]) {
	int exit_code = 0;
	int line_error = 0;
	char *file_error = NULL;

	if ((exit_code = interpreter(argv[1], argv[2], &line_error, &file_error)) != SUCCESS) {
		print_error(exit_code, line_error, file_error);

		return exit_code;
	}

	return 0;
}

void print_error(int exit_code, int line_error, char *file_error) {
	char *errors[7] = {
		"NO_MEMORY - The system is out of memory\n",
		"INVALID_MULTILINE_COMMENT - You must close multiline comment\n",
		"CANT_OPEN_FILE - Can't open file\n",
		"INVALID_INSTRUCTION - You put an invalid instruction\n",
		"HAVENT_VAR - There is an undeclared variable\n",
		"HAVENT_OPERATOR - There is an invalid operator\n",
		"DIVISION_BY_ZERO - Division by zero\n"
	};

	fprintf(stderr, "(%s) %d: %s", file_error, line_error, errors[abs(exit_code) - 2]);
}