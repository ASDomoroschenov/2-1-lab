#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../lib/hash_table.h"

unsigned long hash_func(int key) {
	return key % SIZE_TABLE;
}

int create_hash_item(char key, Array *array, hash_item **item) {
	*item = (hash_item*)malloc(sizeof(hash_item));

	if (!*item) {
		return NO_MEMORY;
	}

	(*item)->key = key;
	(*item)->value = array;

	return SUCCESS;
}

int create_hash_table(hash_table **table) {
	*table = (hash_table*)malloc(sizeof(hash_table));

	if (!*table) {
		return NO_MEMORY;
	}

	(*table)->current_size = 0;
	(*table)->size = SIZE_TABLE;
	(*table)->items = (hash_item**)malloc(sizeof(hash_item*) * (*table)->size);

	if (!(*table)->items) {
		free((*table)->items);
		(*table)->items = NULL;
		return NO_MEMORY;
	}

	for (int i = 0; i < (*table)->size; i++) {
		(*table)->items[i] = NULL;
	}

	return SUCCESS;
}

int add_to_hash_table(char key, Array *array, hash_table **table) {
	int exit_code = 0;

	if (!*table) {
		exit_code = create_hash_table(table);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	}

	int index = hash_func(key);
	hash_item *item = NULL;

	exit_code = create_hash_item(key, array, &item);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if ((*table)->items[index] == NULL) {
		if ((*table)->current_size == (*table)->size) {
			free(item);
			return FULL_TABLE;
		}

		(*table)->items[index] = item;
		(*table)->current_size++;
	} else {
		free_hash_item(&((*table)->items[index]));
		(*table)->items[index] = item;
	}

	return SUCCESS;
}

hash_item *get_item(char key, hash_table *table) {
	if (!table) {
		return NULL;
	}

	return table->items[hash_func(key)];
}

void free_hash_item(hash_item **item) {
	if (*item) {
		if ((*item)->value) {
			if ((*item)->value->data) {
				free((*item)->value->data);
				(*item)->value->data = NULL;
				(*item)->value->size = 0;
			}

			free((*item)->value);
			(*item)->value = NULL;
		}

		free(*item);
		*item = NULL;
	}
}

void free_hash_table(hash_table **table) {
	if (*table) {
		for (int i = 0; i < (*table)->size; i++) {
			free_hash_item(&((*table)->items[i]));
		}

		free((*table)->items);
		(*table)->items = NULL;
		(*table)->size = 0;
		(*table)->current_size = 0;
		free(*table);
		*table = NULL;
	}
}