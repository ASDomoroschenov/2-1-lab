#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "../lib/string.h"
#include "../lib/list.h"
#include "../lib/hash.h"

int is_valid_macro(char *str) {
	char *ptr = str - 1;

	while (*++ptr) {
		if (!isalnum(*ptr)) {
			return 0;
		}
	}

	return 1;
}

long long hash(char *str_62) {
	char *ptr = str_62 - 1;
	long long num_10 = 0;
	int base = 16;

	while (*++ptr) {
		num_10 = num_10 * base + ((isdigit(*ptr)) ? (*ptr - '0') : ((isupper(*ptr)) ? (*ptr - 'A' + 10) : (*ptr - 'a' + 36)));
 	}

	return num_10;
}

long long hash_value(char *def_name, int table_size) {
	return hash(def_name) % table_size;
}

int create_table(hash_table *table, int hash_size) {
	int exit_code = 0;

	table->size = hash_size;
	table->array_list = (list_t**)malloc(sizeof(list_t*) * table->size);

	if (!table->array_list) {
		table->size = 0;

		return NO_MEMORY;
	}

	for (int i = 0; i < table->size; i++) {
		exit_code = create_list(&(table->array_list[i]));

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}

	table->min_size_list = table->array_list[0]->size;
	table->max_size_list = table->array_list[0]->size;

	return SUCCESS;
}

int add_to_table(char *def_name, char *value, hash_table *table) {
	int exit_code = 0;

	if (!table->array_list || table->size == 0) {
		exit_code = create_table(table, HASHSIZE);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}
		
	long long cache = hash(def_name);
	long long hash_value = cache % table->size;
	list_t *list = table->array_list[hash_value];

	if (!(list->root)) {
		exit_code = create_node_list(def_name, value, cache, &(list->root));

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		list->size++;
		set_min_max_size_list(table);

		return SUCCESS;
	}

	node_list *node = list->root;

	while (node->next) {
		node = node->next;
	}

	exit_code = create_node_list(def_name, value, cache, &(list->root));

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	list->size++;
	set_min_max_size_list(table);

	return SUCCESS;
}

void set_min_max_size_list(hash_table *table) {
	table->min_size_list = table->array_list[0]->size;
	table->max_size_list = table->array_list[0]->size;

	for (int i = 0; i < table->size; i++) {
		if (table->min_size_list > table->array_list[i]->size) {
			table->min_size_list = table->array_list[i]->size;
		} else {
			if (!table->min_size_list) {
				table->min_size_list = table->array_list[i]->size;
			}
		}
		if (table->max_size_list < table->array_list[i]->size) {
			table->max_size_list = table->array_list[i]->size;
		}
	}
}

char *get_value(char *def_name, hash_table table) {
	if (table.size != 0) {
		long long hash_value_def = hash_value(def_name, table.size);

		if (is_valid_macro(def_name)) {
			node_list *node = table.array_list[hash_value_def]->root;

			while (node) {
				if (strcmp(def_name, node->def_name) == 0) {
					return node->value;
				}
				node = node->next;
			}
		}
	}

	return NULL;
}

int gcd(int num_1, int num_2) {
	return num_2 ? gcd(num_2, num_1 % num_2) : num_1;
}

int new_size_table(hash_table table) {
	int remainders[table.size];
	int coprime = table.size;
	int max_count_remaind = 0;
	int min_count_remaind = 0;
	node_list *node = NULL;

	while (min_count_remaind && max_count_remaind / min_count_remaind <= 2) {
		max_count_remaind = 0;
		min_count_remaind = 0;

		for (int i = 0; i < table.size; i++) {
			remainders[i] = 0;
		}

		while (gcd(table.size, coprime) != 1) {
			coprime++;
		}

		for (int i = 0; i < table.size; i++) {
			node = table.array_list[i]->root;

			while (node) {
				remainders[node->hash % coprime]++;
				node = node->next;
			}
		}

		for (int i = 0; i < table.size; i++) {
			if (max_count_remaind < remainders[i]) {
				max_count_remaind = remainders[i];
			}
			if (min_count_remaind > remainders[i]) {
				min_count_remaind = remainders[i];
			} else {
				if (!min_count_remaind) {
					min_count_remaind = remainders[i];
				}
			}
		}
	}

	return coprime;
}

int rebuild_table(hash_table *table) {
	hash_table new_table = {NULL, 0, 0, 0};
	node_list *node = NULL;
	int exit_code = create_table(&new_table, new_size_table(*table));

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	for (int i = 0; i < table->size; i++) {
		node = table->array_list[i]->root;

		while (node) {
			push_back(&(new_table.array_list[node->hash % new_table.size]), node);
			node = node->next;
		}

		free(table->array_list[i]);
	}

	free(table->array_list);
	table->array_list = new_table.array_list;
	table->size = new_table.size;
	set_min_max_size_list(table);

	return SUCCESS;
}

void free_table(hash_table *table) {
	for (int i = 0; i < table->size; i++) {
		free_list(&(table->array_list[i]));
	}
	if (table->array_list) {
		free(table->array_list);
	}
	table->array_list = NULL;
	table->size = 0;
}