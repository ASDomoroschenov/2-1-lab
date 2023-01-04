#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "hash_table.h"

int hash(int id, int table_size) {
	return id % table_size;
}

int create_user(client **user, int id, int trace) {
	(*user) = (client*)malloc(sizeof(client));

	if (!(*user)) {
		return NO_MEMORY;
	}

	(*user)->line_error = 0;
	(*user)->id = id;
	(*user)->trace = trace;
	(*user)->array.vars = NULL;
	(*user)->array.current_size = 0;
	(*user)->array.size = 0;

	return SUCCESS;
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
	table->count_users = 0;

	return SUCCESS;
}

int add_user(int id, int trace, hash_table *table) {
	int exit_code = 0;
	client *user = NULL;

	if ((exit_code = create_user(&user, id, trace)) != SUCCESS) {
		return exit_code;
	}

	if (!table->array_list || table->size == 0) {
		exit_code = create_table(table, HASHSIZE);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}

	long hash_value = hash(user->id, table->size);
	list_t *list = table->array_list[hash_value];
	node_list *add_node = NULL;

	exit_code = create_node_list(user, &add_node);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	push_back(&list, add_node);

	list->size++;
	set_min_max_size_list(table);
	table->count_users++;

	return SUCCESS;
}

int delete_user(int id, hash_table *table) {
	list_t *list = table->array_list[hash(id, table->size)];
	node_list *node = list->root;

	while (node) {
		if (node->user->id == id) {
			table->count_users--;
			return delete_node(node, &list);
		}
	}

	return HAVENT_USER;
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

client *get_user(int id, hash_table table) {
	if (table.size != 0) {
		int hash_value = hash(id, table.size);

		node_list *node = table.array_list[hash_value]->root;

		while (node) {
			if (node->user->id == id) {
				return node->user;
			}

			node = node->next;
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
				remainders[node->user->id % coprime]++;
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
	
	if ((table->min_size_list != 0) &&
		((table->max_size_list / table->min_size_list) > 2)) {
		int exit_code = create_table(&new_table, new_size_table(*table));

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		for (int i = 0; i < table->size; i++) {
			node = table->array_list[i]->root;

			while (node) {
				push_back(&(new_table.array_list[node->user->id % new_table.size]), node);
				node = node->next;
			}

			free(table->array_list[i]);
		}

		free(table->array_list);
		table->array_list = new_table.array_list;
		table->size = new_table.size;
		set_min_max_size_list(table);
	}

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