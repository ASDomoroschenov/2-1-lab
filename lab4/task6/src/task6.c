#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../lib/string.h"
#include "../lib/types.h"
#include "../lib/table.h"

void print_error(int);

int main(int argc, char *argv[]) {
	char *str = NULL;
	int exit_code = 0;
	FILE *input_file = NULL;
	array_var_names variables = {NULL, 0};
	table_t table = {NULL, 0, 0};

	if (argc > 1) {
		if ((input_file = fopen(argv[1], "r")) != NULL) {
			get_str(&str, input_file);
			exit_code = strip(&str);

			if (exit_code != SUCCESS) {
				print_error(exit_code);

				return exit_code;
			}

			exit_code = make_table(str, &table, &variables);

			if (exit_code != SUCCESS) {
				print_error(exit_code);

				return exit_code;
			}

			output_table(table, variables, str);

			free(str);
			free(variables.var_names);
			free_table(&table);
		} else {
			print_error(CANT_OPEN_FILE);
			return CANT_OPEN_FILE;
		}
	} else {
		print_error(INVALID_COUNT_ARGS);
		return INVALID_FILE_IN_ARGS;
	}

	return 0;
}

void print_error(int exit_code) {
	char *errors[7] = {
		"NO_MEMORY: The system is out of memory",
		"INVALID_EXPRESSION: You put an invalid expression",
		"INVALID_BRACKETS: You put an expression with invalid balancse of brackets",
		"EMPTY_STRING: Can't do calculations with empty string",
		"CANT_OPEN_FILE: Can't open file",
		"INVALID_COUNT_ARGS: Invalid count of arguments",
		"INVALID_FILE_IN_ARG: Invalid file in transferred arguments"
	};

	fprintf(stderr, "%s\n", errors[abs(exit_code) - 2]);
}