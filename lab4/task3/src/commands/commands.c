#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../commands/commands.h"
#include "../operations/operations.h"
#include "../variables/variables.h"
#include "../check/check.h"
#include "../string/string.h"

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

//read(a, 20);

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

int count_expression(array_variables array, char *str, int *res) {
	char var_name_1 = 0;
	char var_name_2 = 0;
	int index = 0;
	int exit_code = 0;
	int value_var_1 = 0;
	int value_var_2 = 0;
	char *operator = NULL;

	if ((exit_code = get_operator(str, &operator)) != SUCCESS) {
		return exit_code;
	}

	if (is_binary_exp(str)) {
		get_next_var(str, &index, &var_name_1);
		index++;
		get_next_var(str, &index, &var_name_2);

		if ((exit_code = get_value_var(array, var_name_1, &value_var_1)) != SUCCESS) {
			return exit_code;
		}

		if ((exit_code = get_value_var(array, var_name_2, &value_var_2)) != SUCCESS) {
			free(operator);

			return exit_code;
		}

		*res = execute_operation(operator, value_var_1, value_var_2);
	} else {
		get_next_var(str, &index, &var_name_1);

		if ((exit_code = get_value_var(array, var_name_1, &value_var_1)) != SUCCESS) {
			free(operator);
			
			return exit_code;
		}

		*res = execute_operation(operator, value_var_1, 0);
	}

	free(operator);

	return SUCCESS;
}

int read(array_variables *array, char name_var, int base) {
	char *str = NULL;
	int exit_code = 0;
	int value_var = 0;

	if ((exit_code = get_str(&str, stdin)) != SUCCESS) {
		return exit_code;
	}

	if ((exit_code = to_10(str, base, &value_var)) != SUCCESS) {
		return exit_code;
	}

	if ((exit_code = add_var(array, name_var, value_var)) != SUCCESS) {
		return exit_code;
	}

	free(str);

	return SUCCESS;
}

int write(array_variables array, char name_var, int base) {
	int find_index = find_var(array, name_var);
	char *value = NULL;
	int exit_code = 0;

	if (find_index < 0) {
		return HAVENT_VAR;	
	}

	if ((exit_code = to_base(array.vars[find_index].value, base, &value)) != SUCCESS) {
		return exit_code;
	}

	if (is_empty_str(value)) {
		printf("%c := 0\n", array.vars[find_index].name);
	} else {
		printf("%c := %s\n", array.vars[find_index].name, value);
	}

	free(value);

	return SUCCESS;
}
