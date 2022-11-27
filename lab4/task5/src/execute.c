#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/string.h"
#include "../lib/execute.h"
#include "../lib/expression.h"

int check_args(int argc, char **argv) {
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i] + strlen(argv[i]) - 4, ".txt") != 0) {
				return INVALID_FILE_IN_ARGS;
			}
		}

		return SUCCESS;
	}

	return INVALID_COUNT_ARGS;
}

void print_error(int exit_code, FILE *out) {
	if (exit_code == NO_MEMORY) {
		fprintf(out, "NO_MEMORY: The system is out of memory\n");
	}
	if (exit_code == INVALID_EXPRESSION) {
		fprintf(out, "INVALID_EXPRESSION: The expression contains literal characters\n");
	}
	if (exit_code == INVALID_BRACKETS) {
		fprintf(out, "INVALID_BRACKETS: The order of brackets is not respected in the expression\n");
	}
	if (exit_code == EMPTY_STRING) {
		fprintf(out, "EMPTY_STRING: Cannot operate on an empty string\n");
	}
	if (exit_code == DIVISION_BY_ZERO) {
		fprintf(out, "DIVISION_BY_ZERO: division by zero is not allowed\n");
	}
	if (exit_code == CANT_OPEN_FILE) {
		fprintf(out, "CANT_OPEN_FILE: Can't open file or file doesn't exist\n");
	}
	if (exit_code == INVALID_COUNT_ARGS) {
		fprintf(out, "INVALID_COUNT_ARGS: Invalid number of arguments passed\n");
	}
	if (exit_code == INVALID_FILE_IN_ARGS) {
		fprintf(out, "INVALID_FILE_IN_ARGS: The passed arguments contain a non-text file\n");
	}
}

int files_processing(int argc, char **argv) {
	char *name_log_file = NULL;
	char *str = NULL;
	char *postfix = NULL;
	double result = 0.0;
	int exit_code = 0;
	int serial_number = 0;
	int division_by_zero = 0;

	exit_code = check_args(argc, argv);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	for (int i = 1; i < argc; i++) {
		FILE *read_file = NULL;
		FILE *log_file = NULL;
		exit_code = add_str(&name_log_file, 2, "./log/log_", argv[i] + 7);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		if ((read_file = fopen(argv[i], "r")) != NULL &&
			(log_file = fopen(name_log_file, "a")) != NULL) {
			while (!feof(read_file)) {
				exit_code = get_str(&str, read_file);

				if (exit_code != SUCCESS) {
					return exit_code;
				}

				exit_code = to_postfix(str, &postfix);

				if (exit_code != SUCCESS) {
					fprintf(log_file, "%s : %d - ", str, serial_number);
					print_error(exit_code, log_file);
				} else {
					exit_code = count_expression(str, &result);

					if (exit_code != SUCCESS) {
						fprintf(log_file, "%s : %d - ", str, serial_number);
						print_error(exit_code, log_file);
					} else {
						printf("%s(expression %d):\n", argv[i], serial_number);
						printf("expression: %s\npostfix expression: %s\ncount expression: %lf\n\n", str, postfix, result);
					}
				}

				serial_number++;
				free(str);
				free(postfix);
				str = NULL;
				postfix = NULL;
				result = 0.0;
			}
			fclose(read_file);
			fclose(log_file);
			serial_number = 0;
		} else {
			return CANT_OPEN_FILE;
		}

		free(name_log_file);
		name_log_file = NULL;
	}

	return SUCCESS;
}