#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

int delete_sep(char **str) {
	int len_str = strlen(*str);

	if (len_str > 1) {
		char *temp = (char*)malloc(sizeof(char) * len_str);

		if (!temp) {
			return NO_MEMORY;
		}

		strncpy(temp, *str, len_str - 1);
		temp[len_str - 1] = 0;

		free(*str);
		*str = temp;
	} else {
		free(*str);
		*str = NULL;
	}

	return SUCCESS;
}

int run_command(array_variables *array, char *str, int *line_error) {
	int exit_code = 0;
	char name_var = 0;
	char *base = NULL;
	char *expression = NULL;
	int value_expression = 0;

	if (check_read(str) || check_write(str)) {
		if ((exit_code = get_name_base(str, &name_var, &base)) != SUCCESS) {
			if (array->vars) {
				free(array->vars);
			}
			free(str);

			return exit_code;
		}

		if (check_read(str)) {
			if ((exit_code = read(array, name_var, atoi(base))) != SUCCESS) {
				if (array->vars) {
					free(array->vars);
				}
				free(base);
				free(str);

				return exit_code;
			}
		} else {
			if ((exit_code = write(*array, name_var, atoi(base))) != SUCCESS) {
				if (array->vars) {
					free(array->vars);
				}
				free(base);
				free(str);

				return exit_code;
			}
		}

		free(base);
	} else {
		if (check_binary(str) || check_unary(str)) {
			name_var = str[0];

			if ((exit_code = get_expression(str, &expression)) != SUCCESS) {
				if (array->vars) {
					free(array->vars);
				}
				free(str);

				return exit_code;
			}

			if ((exit_code = count_expression(*array, expression, &value_expression)) != SUCCESS) {
				if (array->vars) {
					free(array->vars);
				}
				free(expression);
				free(str);

				return exit_code;
			}

			if (find_var(*array, name_var) < 0) {
				if ((exit_code = add_var(array, name_var, value_expression)) != SUCCESS) {
					if (array->vars) {
						free(array->vars);
					}
					free(expression);
					free(str);

					return exit_code;
				}
			} else {
				if ((exit_code = update_var(array, name_var, value_expression)) != SUCCESS) {
					if (array->vars) {
						free(array->vars);
					}
					free(expression);
					free(str);

					return exit_code;
				}
			}

			free(expression);
		} else {
			if (array->vars) {
				free(array->vars);
			}
			free(str);

			return INVALID_EXPRESSION;
		}
	}

	return SUCCESS;
}

int interpreter(char *name_file, int *line_error) {
	array_variables array = {NULL, 0, 0};
	FILE *read_file = NULL;
	char *str = NULL;
	int exit_code = 0;

	if ((read_file = fopen(name_file, "r")) != NULL) {
		while (!feof(read_file)) {
			if ((exit_code = get_str_with_comments(&str, read_file, '%', '{', '}', line_error)) != SUCCESS) {
				if (array.vars) {
					free(array.vars);
				}
				fclose(read_file);
			}

			if (str && !is_empty_str(str)) {
				if ((exit_code = strip(&str)) != SUCCESS) {
					if (array.vars) {
						free(array.vars);
					}
					free(str);
					fclose(read_file);

					return exit_code;
				}

				if (str[strlen(str) - 1] != ';') {
					return INVALID_EXPRESSION;
				}

				if ((exit_code = delete_sep(&str)) != SUCCESS) {
					if (array.vars) {
						free(array.vars);
					}
					free(str);
					fclose(read_file);

					return exit_code;
				}

				if (str) {
					if ((exit_code = run_command(&array, str, line_error)) != SUCCESS) {
						if (array.vars) {
							free(array.vars);
						}
						fclose(read_file);

						return exit_code;
					}

					free(str);
					str = NULL;
				}
			}
		}

		if (array.vars) {
			free(array.vars);
		}
		fclose(read_file);
	} else {
		return OPEN_FILE;
	}

	return SUCCESS;
}