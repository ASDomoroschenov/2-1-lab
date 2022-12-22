#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/execute.h"

int execute_instructions(char *name_file, int *line_error) {
	FILE *read_file = NULL;
	char *str = NULL;
	char *name_var = NULL;
	array_vars array = {NULL, 0, 0};
	int exit_code = 0;

	if ((read_file = fopen(name_file, "r")) != NULL) {
		while (!feof(read_file)) {
			exit_code = get_str(&str, read_file);

			if (exit_code != SUCCESS) {
				fclose(read_file);
				free_array_vars(&array);

				return exit_code;
			}

			if (str && !is_empty_str(str)) {
				exit_code = strip(&str);

				if (exit_code != SUCCESS) {
					fclose(read_file);
					free_array_vars(&array);
					free(str);

					return exit_code;
				}

				int len_str = strlen(str);
				char *temp = (char*)malloc(sizeof(char) * len_str);

				if (!temp) {
					fclose(read_file);
					free_array_vars(&array);
					free(str);

					return NO_MEMORY;
				}

				strncpy(temp, str, len_str - 1);
				temp[len_str - 1] = 0;
				free(str);
				str = temp;

				if (strstr(str, "print")) {
					exit_code = print(str, array);

					if (exit_code != SUCCESS) {
						return exit_code;
					}
				} else {
					exit_code = get_name_str(str, &name_var);

					if (exit_code != SUCCESS) {
						fclose(read_file);
						free_array_vars(&array);
						free(str);

						return exit_code;
					}

					if (get_var(array, name_var) < 0) {
						exit_code = add_var(&array, str);

						if (exit_code != SUCCESS) {
							fclose(read_file);
							free_array_vars(&array);
							free(name_var);
							free(str);

							return exit_code;
						}
					} else {
						exit_code = update_var(&array, str);

						if (exit_code != SUCCESS) {
							fclose(read_file);
							free_array_vars(&array);
							free(name_var);
							free(str);

							return exit_code;
						}
					}

					free(name_var);
				}

				free(str);
				str = NULL;
			}
			(*line_error)++;
		}

		free_array_vars(&array);
		fclose(read_file);
	} else {
		return OPEN_FILE;
	}

	return SUCCESS;
}