#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../commands/commands.h"
#include "../operations/operations.h"
#include "../variables/variables.h"
#include "../check/check.h"
#include "../string/string.h"

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

int is_digital_value(char *str) {
	char *ptr = str - 1;

	while(*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return 1;
}

int get_operator(char *str, char **res) {
	char *ptr = str;
	int exit_code = 0;

	while (*ptr && !is_operator(*ptr)) {
		ptr++;
	}

	if (!*ptr) {
		return 0;
	}

	if ((*ptr == '-' && *(ptr + 1) == '>') ||
		(*ptr == '+' && *(ptr + 1) == '>') ||
		(*ptr == '<' && *(ptr + 1) == '-') ||
		(*ptr == '<' && *(ptr + 1) == '>')) {
		exit_code = add_chr(res, 2, *ptr, *(ptr + 1));
	} else {
		exit_code = add_chr(res, 1, *ptr);
	}

	return exit_code;
}

int get_left_operand(char *expression, char **operand) {
	char *ptr = expression;
	int temp_size = 0;
	int exit_code = 0;
	char *temp = NULL;

	while (isspace(*ptr)) {
		ptr++;
	}

	while (*ptr && !isspace(*ptr) && !is_operator(*ptr)) {
		if ((exit_code = join_symb(*ptr, operand, &temp_size)) != SUCCESS) {
			return exit_code;
		}
		ptr++;
	}

	temp = (char*)realloc(*operand, sizeof(char*) * (strlen(*operand) + 1));

	if (!temp) {
		return NO_MEMORY;
	}

	*operand = temp;

	return SUCCESS;
}

int get_right_operand(char *expression, char **operand) {
	char *ptr = expression;
	char *temp = NULL;
	int temp_size = 0;
	int exit_code = 0;

	while (!is_operator(*ptr)) {
		ptr++;
	}

	if ((*ptr == '-' && *(ptr + 1) == '>') ||
		(*ptr == '+' && *(ptr + 1) == '>') ||
		(*ptr == '<' && *(ptr + 1) == '-') ||
		(*ptr == '<' && *(ptr + 1) == '>')) {
		ptr += 2;
	} else {
		ptr++;
	}

	while (isspace(*ptr)) {
		ptr++;
	}

	while (*ptr && !isspace(*ptr)) {
		if ((exit_code = join_symb(*ptr, operand, &temp_size)) != SUCCESS) {
			return exit_code;
		}
		ptr++;
	}

	temp = (char*)realloc(*operand, sizeof(char*) * (strlen(*operand) + 1));

	if (!temp) {
		return NO_MEMORY;
	}

	*operand = temp;

	return SUCCESS;
}

int get_name_base(char *str, char *name, char **base) {
	char *ptr = strchr(str, '(');
	char *base_begin = NULL;

	while (!isalpha(*ptr)) {
		ptr++;
	}

	*name = *ptr++;

	while (!isdigit(*ptr)) {
		ptr++;
	}

	base_begin = ptr;

	while(isdigit(*ptr)) {
		ptr++;
	}

	*base = (char*)malloc(sizeof(char) * (ptr - base_begin + 1));

	if (!*base) {
		return NO_MEMORY;
	}

	strncpy(*base, base_begin, ptr - base_begin);
	(*base)[ptr - base_begin] = 0;

	return SUCCESS;
}

int get_expression(char *str, char **expression) {
	char *ptr = strstr(str, ":=");
	ptr += 2;

	*expression = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));

	if (!*expression) {
		return NO_MEMORY;
	}

	strcpy(*expression, ptr);

	return strip(expression);
}

int count_expression(array_variables array, char *expression, int *res) {
	int exit_code = 0;
	int value_left_var = 0;
	int value_right_var = 0;
	char *operator = NULL;
	char *left_op = NULL;
	char *right_op = NULL;

	if (is_digital_value(expression)) {
		*res = atoi(expression);

		return SUCCESS;
	}

	if (strlen(expression) == 1 && isalpha(expression[0])) {
		if ((exit_code = get_value_var(array, expression[0], res)) != SUCCESS) {
			return exit_code;
		}

		return SUCCESS;
	}

	if ((exit_code = get_operator(expression, &operator)) != SUCCESS) {
		return exit_code;
	}

	if (is_binary_exp(expression)) {
		if ((exit_code = get_left_operand(expression, &left_op)) != SUCCESS) {
			free(operator);
			
			return exit_code;
		}

		if ((exit_code = get_right_operand(expression, &right_op)) != SUCCESS) {
			free(operator);
			free(left_op);

			return exit_code;
		}

		if (strlen(left_op) == 1 && isalpha(left_op[0])) {
			if ((exit_code = get_value_var(array, left_op[0], &value_left_var)) != SUCCESS) {
				free(operator);
				free(left_op);
				free(right_op);

				return exit_code;
			}
		} else {
			if (is_digital_value(left_op)) {
				value_left_var = atoi(left_op);
			} else {
				free(operator);
				free(left_op);
				free(right_op);

				return INVALID_EXPRESSION;
			}
		}

		if (strlen(right_op) == 1 && isalpha(right_op[0])) {
			if ((exit_code = get_value_var(array, right_op[0], &value_right_var)) != SUCCESS) {
				free(operator);
				free(left_op);
				free(right_op);

				return exit_code;
			}
		} else {
			if (is_digital_value(right_op)) {
				value_right_var = atoi(right_op);
			} else {
				free(operator);
				free(left_op);
				free(right_op);

				return INVALID_EXPRESSION;
			}
		}

		*res = execute_operation(operator, value_left_var, value_right_var);
	} else {
		if ((exit_code = get_right_operand(expression, &right_op)) != SUCCESS) {
			free(operator);

			return exit_code;
		}

		if (strlen(right_op) == 1 && isalpha(right_op[0])) {
			if ((exit_code = get_value_var(array, right_op[0], &value_right_var)) != SUCCESS) {
				free(operator);
				free(right_op);

				return exit_code;
			}
		} else {
			if (is_digital_value(right_op)) {
				value_right_var = atoi(right_op);
			} else {
				free(operator);
				free(right_op);

				return INVALID_EXPRESSION;
			}
		}

		*res = execute_operation(operator, 0, value_right_var);
	}

	if (left_op) {
		free(left_op);
	}
	free(right_op);
	free(operator);

	return SUCCESS;
}

int set_value(array_variables *array, char name_var, int value_var) {
	int exit_code = 0;
	
	if (find_var(*array, name_var) < 0) {
		exit_code = add_var(array, name_var, value_var);
	} else {
		exit_code = update_var(array, name_var, value_var);
	}

	return exit_code;
}

int set_var(array_variables *array, char *str) {
	char *expression = NULL;
	int value_expression = 0;
	int exit_code = 0;
	char name_var = 0;
	
	if ((exit_code = get_expression(str, &expression)) != SUCCESS) {
		return exit_code;
	}

	if (!is_empty_str(expression)) {
		name_var = str[0];
		if ((exit_code = count_expression(*array, expression, &value_expression)) != SUCCESS) {
			free(expression);
			return exit_code;
		}
		if ((exit_code = set_value(array, name_var, value_expression)) != SUCCESS) {
			free(expression);
			return exit_code;
		}
		free(expression);
	} else {
		free(expression);

		return INVALID_EXPRESSION;
	}

	return SUCCESS;
}