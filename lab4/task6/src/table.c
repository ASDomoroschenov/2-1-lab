#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/table.h"

int binary_search(array_var_names variables, char symb) {
	int left = 0;
	int right = variables.size;
	int find_index = -1;

	if (variables.size > 0) {
		while (left <= right) {
			int middle = (right + left) / 2;

			if (variables.var_names[middle].name == symb) {
				return middle;
			}

			if (symb < variables.var_names[middle].name) {
				right = middle - 1;
			} else {
				left = middle + 1;
			}
		}
	}

	return find_index;
}

int compare(const void *item_1, const void *item_2) {
	return ((variable*)item_1)->name > ((variable*)item_2)->name;
}

int get_variables(array_var_names *variables, char *str) {
	int size = 0;
	variable *temp = NULL;
	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		if (isalpha(str[i])) {
			if (binary_search(*variables, str[i]) < 0) {
				if (variables->size == size) {
					size *= 2;
					temp = (variable*)realloc(variables->var_names, sizeof(variable) * size);

					if (!temp) {
						return NO_MEMORY;
					}

					variables->var_names = temp;
				}

				variables->var_names[variables->size++].name = str[i];
				qsort(variables->var_names, variables->size, sizeof(variable), compare);
			}
		}
	}

	temp = (variable*)realloc(variables->var_names, sizeof(variable) * variables->size);

	if (!temp) {
		return NO_MEMORY;
	}

	variables->var_names = temp;

	return SUCCESS;
}

int count_bin_tree(node_bin_tree *node, array_var_names variables) {
	if (!node) {
		return 0;
	}
	if (isalpha(node->data[0])) {
		return variables.var_names[binary_search(variables, node->data[0])].value;
	}
	if (isdigit(node->data[0])) {
		return node->data[0] - '0';
	}
	if (is_operator(node->data)) {
		return execute(node->data, count_bin_tree(node->left, variables), count_bin_tree(node->right, variables));
	}
}

int make_table(char *str, table_t *table, array_var_names *variables) {
	if (!str) {
		return EMPTY_STRING;
	}

	bin_tree *tree_exp = NULL;
	int exit_code = get_variables(variables, str);

	if (exit_code != SUCCESS) {
		return NO_MEMORY;
	}

	exit_code = make_bin_tree_exp(&tree_exp, str);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	table->row = variables->size ? 2 << (variables->size - 1) : 1;
	table->col = variables->size + 1;
	table->arr = (int**)malloc(sizeof(int*) * table->row);

	if (!table->arr) {
		free(variables->var_names);
		free_bin_tree(&tree_exp);

		return NO_MEMORY;
	}

	for (int i = 0; i < table->row; i++) {
		table->arr[i] = (int*)malloc(sizeof(int) * table->col);

		if (!table->arr[i]) {
			for (int j = 0; j < i; j++) {
				free(table->arr[j]);
				table->arr[j] = NULL;
			}

			free_bin_tree(&tree_exp);
			free(table->arr);
			free(variables->var_names);
			table->arr = NULL;
			table->row = 0;
			table->col = 0;

			return NO_MEMORY;
		}
	}

	for (int i = 0; i < table->row; i++) {
		for (int j = 0; j < variables->size; j++) {
			variables->var_names[variables->size - j - 1].value = (i >> j) & 1;
			table->arr[i][j] = (i >> j) & 1;
		}
		table->arr[i][variables->size] = count_bin_tree(tree_exp->root, *variables);
	}

	free_bin_tree(&tree_exp);

	return SUCCESS;
}

void output_table(table_t table, array_var_names variables, char *expression) {
	for (int i = 0; i < variables.size * 4 + strlen(expression) + 4; i++) {
		printf("-");
	}
	printf("\n|");

	for (int i = 0; i < variables.size; i++) {
		printf(" %c |", variables.var_names[i].name);
	}
	printf(" %s |\n", expression);

	for (int i = 0; i < table.row; i++) {
		for (int i = 0; i < variables.size * 4 + strlen(expression) + 4; i++) {
			printf("-");
		}
		printf("\n|");
		for (int j = 0; j < table.col; j++) {
			if (j != table.col - 1) {
				printf(" %d |", table.arr[i][j]);
			} else {
				for (int index = 0; index < (strlen(expression) + 2) / 2; index++) {
					printf(" ");
				}
				printf("%d", table.arr[i][j]);
				for (int index = 0; index < (strlen(expression) + 1) / 2; index++) {
					printf(" ");
				}
				printf("|");
			}
		}
		printf("\n");
	}

	for (int i = 0; i < variables.size * 4 + strlen(expression) + 4; i++) {
		printf("-");
	}
	printf("\n");
}

void free_table(table_t *table) {
	for (int i = 0; i < table->row; i++) {
		free(table->arr[i]);
		table->arr[i] = NULL;
	}

	free(table->arr);
	table->arr = NULL;
	table->row = 0;
	table->col = 0;
}