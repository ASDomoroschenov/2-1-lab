#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string/string.h"
#include "types/types.h"
#include "check/check.h"
#include "operations/operations.h"
#include "variables/variables.h"
#include "commands/commands.h"
#include "interpreter/interpreter.h"

void print_error(int, int);

int main(int argc, char *argv[]) {
	int exit_code = 0;
	int line_error = 1;

	if ((exit_code = interpreter(argv[1], &line_error)) != SUCCESS) {
		print_error(exit_code, line_error);

		return exit_code;
	}

	return 0;
}

void print_error(int exit_code, int line_error) {
	char *errors[4] = {
		"NO_MEMORY - The system is out of memory\n",
		"OPEN_FILE - Can't open file\n",
		"INVALID_EXPRESSION - You put an invalid expression\n",
		"HAVENT_VAR - The expression contains an undeclared variable\n"
	};

	fprintf(stderr, "%d: %s", line_error, errors[abs(exit_code) - 2]);
}