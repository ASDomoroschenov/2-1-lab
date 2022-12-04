#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/interpreter.h"

int print_error(int exit_code, int num_line) {
	char *errors[9] = {
		"NO_MEMORY",
		"FULL_TABLE",
		"CANT_OPEN_FILE",
		"NO_SUCH_ITEM",
		"OUT_OF_RANGE",
		"EMPTY_ARRAY",
		"EMPTY_STRING",
		"INVALID_COMMAND",
		"THERE_IS_NO_VALIDATE_DATA_IN_INPUT_FILE"
	};

	fprintf(stderr, "Line %d: %s\n", num_line, errors[abs(exit_code) - 2]);
}

int interpreter(int *line_error) {
	char *str = NULL;
	int exit_code = 0;
	hash_table *table = NULL;
	FILE *file_instructions = NULL;
	*line_error = 1;

	if ((file_instructions = fopen("./instructions/instructions.txt", "r")) != NULL) {
		while (!feof(file_instructions)) {
			exit_code = get_str(&str, file_instructions);

			if (exit_code != SUCCESS) {
				fclose(file_instructions);
				free_hash_table(&table);

				return exit_code;
			}

			if (str) {
				exit_code = execute(str, &table);

				if (exit_code != SUCCESS) {
					free(str);
					fclose(file_instructions);
					free_hash_table(&table);

					return exit_code;
				}

				free(str);
				str = NULL;
				(*line_error)++;
			}
		}

		fclose(file_instructions);
	} else {

		return CANT_OPEN_FILE;
	}

	free_hash_table(&table);

	return SUCCESS;
}