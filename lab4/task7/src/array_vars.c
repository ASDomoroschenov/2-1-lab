#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/expression.h"
#include "../lib/array_vars.h"

int have_operator(char *str) {
	char *ptr = str - 1;

	while (*++ptr) {
		if (is_operator(*ptr)) {
			return 1;
		}
	}

	return 0;
}

int cmp(const void *item_1, const void *item_2) {
	return strcmp(((MemoryCell*)item_1)->name, ((MemoryCell*)item_2)->name);
}

int get_name_str(char *str, char **name_var) {
	char *ptr = str;
	int len_str = strlen(str);

	while (*ptr != '=') {
		ptr++;
	}

	*name_var = (char*)malloc(sizeof(char) * (ptr - str + 1));

	if (!*name_var) {
		return NO_MEMORY;
	}

	strncpy(*name_var, str, ptr - str);
	(*name_var)[ptr - str] = 0;

	return SUCCESS;
}

int get_value_str(char *str, char **value_var) {
	char *ptr = str;
	int len_str = strlen(str);

	while (*ptr != '=') {
		ptr++;
	}

	*value_var = (char*)malloc(sizeof(char) * (str + len_str - ptr));
	
	if (!*value_var) {
		return NO_MEMORY;
	}

	strncpy(*value_var, ptr + 1, str + len_str - ptr - 1);
	(*value_var)[str + len_str - ptr - 1] = 0;

	return SUCCESS;
}

int init_var(char *str, MemoryCell *var) {
	char *name_var = NULL;
	double d_value_var = 0.0;
	char *value_var = NULL;
	int exit_code = 0;

	exit_code = get_name_str(str, &name_var);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (have_operator(str)) {
		array_vars jammer = {NULL, 0, 0};
		exit_code = count_expression(str + strlen(name_var) + 1, jammer, &d_value_var);
		
		if (exit_code != SUCCESS) {
			return exit_code;
		}

		var->value = d_value_var;
	} else {
		exit_code = get_value_str(str, &value_var);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		var->value = atof(value_var);

		free(value_var);
	}

	var->name = name_var;

	return SUCCESS;
}

int add_var(array_vars *array, char *str) {
	MemoryCell var = {NULL, 0};
	int exit_code = init_var(str, &var);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (array->current_size == array->size) {
		array->size = array->size ? array->size * 2 : 1;

		MemoryCell *temp = (MemoryCell*)realloc(array->vars, sizeof(MemoryCell) * array->size);

		if (!temp) {
			return NO_MEMORY;
		}

		array->vars = temp;
	}

	array->vars[array->current_size++] = var;
	qsort(array->vars, array->current_size, sizeof(MemoryCell), cmp);

	return SUCCESS;
}

int get_var(array_vars array, char *name_var) {
	int left = 0;
	int right = array.current_size - 1;
	int middle = 0;
	int compare = 0;

	if (array.current_size == 0) {
		return -1;
	}

	while (left <= right) {
		compare = strcmp(name_var, array.vars[middle].name);
		middle = (left + right) / 2;

		if (compare == 0) {
			return middle;
		}

		if (compare < 0) {
			right = middle - 1;
		} else {
			left = middle + 1;
		}
	}

	return -1;
}

int get_var_value(array_vars array, char *name_var, double *value_var) {
	if (get_var(array, name_var) != -1) {
		*value_var = array.vars[get_var(array, name_var)].value;

		return SUCCESS;
	}

	return HAVENT_VAR;
}

int update_var(array_vars *array, char *str) {
	char *name_var = NULL;
	char *new_value = NULL;
	char *ptr = str;
	int exit_code = get_name_str(str, &name_var);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	while (*ptr != '=') {
		ptr++;
	}

	if (have_operator(str)) {
		exit_code = count_expression(ptr + 1, *array, &(array->vars[get_var(*array, name_var)].value));

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	} else {
		exit_code = get_value_str(str, &new_value);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
		
		array->vars[get_var(*array, name_var)].value = atof(new_value);

		free(new_value);
	}

	free(name_var);

	return SUCCESS;
}

int print(char *str, array_vars array) {
	char *ptr = str;
	char *name_var = NULL;
	int exit_code = 0;
	int len_str = strlen(str);

	if (array.current_size == 0) {
		return EMPTY_ARRAY;
	} 

	if (token_count(str) == 2) {
		ptr = str + strlen("print") + 1;
		name_var = (char*)malloc(sizeof(char) * (str + len_str - ptr + 1));
		strncpy(name_var, ptr, str + len_str - ptr);
		name_var[str + len_str - ptr] = 0;

		if (get_var(array, name_var) < 0) {
			return HAVENT_VAR;
		}

		double value_var = 0.0;
		exit_code = get_var_value(array, name_var, &value_var);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		printf("%s=%lf\n", name_var, value_var);
		free(name_var);
	} else {
		for (int i = 0; i < array.current_size; i++) {
			printf("%s=%lf\n", array.vars[i].name, array.vars[i].value);
		}
	}

	return SUCCESS;
}

void free_array_vars(array_vars *array) {
	for (int i = 0; i < array->current_size; i++) {
		free(array->vars[i].name);
	}

	free(array->vars);
	array->current_size = 0;
	array->size = 0;
}

void output_vars(array_vars array) {
	for (int i = 0; i < array.current_size; i++) {
		printf("%s=%lf\n", array.vars[i].name, array.vars[i].value);
	}
}