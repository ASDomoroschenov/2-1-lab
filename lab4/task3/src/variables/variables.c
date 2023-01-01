#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../operations/operations.h"
#include "../variables/variables.h"
#include "../check/check.h"
#include "../commands/commands.h"
#include "../string/string.h"

int get_next_var(char *str, int *index, char *res) {
	int len_str = strlen(str);

	for (;*index < len_str; (*index)++) {
		if (isalpha(str[*index])) {
			*res = str[*index];

			return 1;
		}
	}

	return 0;
}

int find_var(array_variables array, char name_var) {
	int left = 0;
	int right = array.current_size - 1;
	int find_index = -1;

	if (array.current_size > 0) {
		while (left <= right) {
			int middle = (right + left) / 2;

			if (array.vars[middle].name == name_var) {
				return middle;
			}

			if (name_var < array.vars[middle].name) {
				right = middle - 1;
			} else {
				left = middle + 1;
			}
		}
	}

	return find_index;
}

int get_value_var(array_variables array, char name_var, int *res) {
	int find_index = find_var(array, name_var);

	if (find_index < 0) {
		*res = 0;

		return HAVENT_VAR;
	}

	*res = array.vars[find_index].value;

	return SUCCESS;
}

int add_var(array_variables *array, char name_var, int value_var) {
	variable *temp = NULL;

	if (array->current_size == array->size) {
		array->size = array->size ? array->size * 2 : 1;
		temp = (variable*)realloc(array->vars, sizeof(variable) * array->size);
		
		if (!temp) {
			return NO_MEMORY;
		}

		array->vars = temp;
	}

	array->vars[array->current_size].name = name_var;
	array->vars[array->current_size].value = value_var;

	qsort(array->vars, array->current_size, sizeof(variable), compare);
	array->current_size++;

	return SUCCESS;
}

int update_var(array_variables *array, char name_var, int value_var) {
	int find_index = find_var(*array, name_var);

	if (find_index < 0) {
		return HAVENT_VAR;
	}

	array->vars[find_index].value = value_var;

	return SUCCESS;
}

int compare(const void *item_1, const void *item_2) {
	return ((variable*)item_1)->name > ((variable*)item_2)->name;
}

void output_vars(array_variables array) {
	for (int i = 0; i < array.current_size; i++) {
		printf("%c := %d\n", array.vars[i].name, array.vars[i].value);
	}
}