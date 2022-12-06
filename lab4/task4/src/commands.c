#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/check.h"
#include "../lib/string.h"
#include "../lib/commands.h"

#define eps 0.00000001

int make_command(char *command, char **true_command) {
	if (*true_command) {
		free(*true_command);
		*true_command = NULL;
	}

	int size = token_count(command);

	if (size == 0) {
		return EMPTY_STRING;
	}

	char **split = NULL;
	int exit_code = split_space(command, &split);

	for (int i = 0; i < size; i++) {
		if (i + 1 < size &&
			(strcmp(split[i + 1], ",") == 0 ||
			strcmp(split[i + 1], ";") == 0)) {
			exit_code = add_str(true_command, 1, split[i]);
		} else {
			exit_code = add_str(true_command, 2, split[i], " ");
		}

		if (exit_code != SUCCESS) {
			for (int j = 0; j < size; j++) {
				free(split[j]);
			}
			
			free(split);

			if (*true_command) {
				free(*true_command);
				*true_command = NULL;
			}

			return NO_MEMORY;
		}
	}

	exit_code = strip(true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	for (int j = 0; j < size; j++) {
		free(split[j]);
	}

	free(split);

	return SUCCESS;
}

int Load(char key, char *name_file, hash_table **table) {
	FILE *read_file = NULL;
	int exit_code = 0;

	if ((read_file = fopen(name_file, "r")) != NULL) {
		Array *array = (Array*)malloc(sizeof(Array));
		int tmp_size_array = 0;
		int *tmp = NULL;
		char *token = NULL;

		if (!array) {
			return NO_MEMORY;
		}

		array->size = 0;
		array->data = NULL;

		while (!feof(read_file)) {
			exit_code = get_lexem(&token, read_file);

			if (exit_code != SUCCESS) {
				free(array);
				return exit_code;
			}

			if (token) {
				if (isnumber(token)) {
					if (array->size == tmp_size_array) {
						tmp_size_array = tmp_size_array ? tmp_size_array * 2 : 1;
						tmp = (int*)realloc(array->data, sizeof(int) * tmp_size_array);

						if (!tmp) {
							return NO_MEMORY;
						}

						array->data = tmp;
					}

					array->data[array->size++] = atoi(token);
				}

				free(token);
				token = NULL;
			}
		}

		fclose(read_file);

		if (array->data) {
			int *temp = (int*)realloc(array->data, sizeof(int) * array->size);

			if (!temp) {
				return NO_MEMORY;
			}

			array->data = temp;

			exit_code = add_to_hash_table(key, array, table);

			if (exit_code != SUCCESS) {
				free(array->data);
				free(array);
				
				return exit_code;
			}
		} else {
			free(array);

			return HAVENT_VALIDATE_DATA;
		}
	} else {
		return CANT_OPEN_FILE;
	}

	return SUCCESS;
}

int Save(char key, char *name_file, hash_table *table) {
	FILE *output_file = NULL;

	if ((output_file = fopen(name_file, "w")) != NULL) {
		hash_item *item = get_item(key, table);

		if (!item) {
			fclose(output_file);

			return NO_SUCH_ITEM;
		}

		for (int i = 0; i < item->value->size; i++) {
			fprintf(output_file, "%d ", item->value->data[i]);
		}

		fclose(output_file);
	}

	return SUCCESS;
}

int Rand(char key, int count, int left_board, int right_board, hash_table **table) {
	Array *array = (Array*)malloc(sizeof(Array));
	int exit_code = 0;

	if (!array) {
		return NO_MEMORY;
	}

	array->data = (int*)malloc(sizeof(int) * count);
	array->size = count;

	if (!array->data) {
		free(array);
		return NO_MEMORY;
	}

	for (int i = 0; i < count; i++) {
		array->data[i] = left_board + rand() % (right_board - left_board + 1);
	}

	exit_code = add_to_hash_table(key, array, table);

	return SUCCESS;
}

int Concat(char key_1, char key_2, hash_table *table) {
	hash_item *h_item_1 = get_item(key_1, table);

	if (!h_item_1) {
		return NO_SUCH_ITEM;
	}

	hash_item *h_item_2 = get_item(key_2, table);

	if (!h_item_2) {
		return NO_SUCH_ITEM;
	}

	Array *item_1 = h_item_1->value;
	Array *item_2 = h_item_2->value;

	int *temp = (int*)realloc(item_1->data, sizeof(int) * (item_1->size + item_2->size));
	
	if (!temp) {
		return NO_MEMORY;
	}

	item_1->data = temp;

	memcpy(item_1->data + item_1->size, item_2->data, sizeof(int) * item_2->size);
	
	item_1->size += item_2->size;

	return SUCCESS;
}

int Free(char key, hash_table *table) {
	hash_item *item = get_item(key, table);

	if (!item) {
		return NO_SUCH_ITEM;
	}

	free(item->value->data);
	item->value->data = (int*)malloc(sizeof(int) * 0);
	item->value->size = 0;

	return SUCCESS;
}

int Remove(char key, int begin, int count, hash_table *table) {
	hash_item *item = get_item(key, table);

	if (!item) {
		return NO_SUCH_ITEM;
	}

	if (begin > item->value->size ||
		begin + count > item->value->size ||
		count > item->value->size ||
		item->value->size == 0) {
		return OUT_OF_RANGE;
	}

	int *temp = (int*)malloc(sizeof(int) * (item->value->size - count));
	int index_temp = 0;

	if (!temp) {
		return NO_MEMORY;
	}

	for (int i = 0; i < item->value->size; i++) {
		if ((i < begin) || (i >= begin + count)) {
			temp[index_temp++] = item->value->data[i];
		}
	}

	free(item->value->data);
	item->value->size -= count;

	item->value->data = temp;

	return SUCCESS;
}

int Copy(char key_1, int left_board, int right_board, char key_2, hash_table **table) {
	hash_item *h_item_1 = get_item(key_1, *table);

	if (!h_item_1) {
		return NO_SUCH_ITEM;
	}

	if (left_board > h_item_1->value->size ||
		right_board > h_item_1->value->size ||
		left_board > right_board) {
		return OUT_OF_RANGE;
	}

	int new_size = right_board - left_board + 1;
	int *temp = (int*)malloc(sizeof(int) * new_size);

	if (!temp) {
		return NO_MEMORY;
	}

	memcpy(temp, h_item_1->value->data + left_board, sizeof(int) * new_size);

	hash_item *h_item_2 = get_item(key_2, *table);

	if (!h_item_2) {
		Array *add_arr = (Array*)malloc(sizeof(Array));
		add_arr->data = temp;
		add_arr->size = new_size;

		if (!add_arr) {
			free(temp);
			return NO_MEMORY;
		}

		int exit_code = add_to_hash_table(key_2, add_arr, table);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
	} else {
		free(h_item_2->value->data);
		h_item_2->value->data = temp;
		h_item_2->value->size = new_size;
	}

	return SUCCESS;
}

int ascending(const void *item_1, const void *item_2) {
	return *((int*)item_1) > *((int*)item_2);
}

int descending(const void *item_1, const void *item_2) {
	return *((int*)item_1) < *((int*)item_2);
}

int Sort_ascending(char key, hash_table *table) {
	hash_item *item = get_item(key, table);

	if (!item) {
		return NO_SUCH_ITEM;
	}

	qsort(item->value->data, item->value->size, sizeof(int), ascending);

	return SUCCESS;
}

int Sort_descending(char key, hash_table *table) {
	hash_item *item = get_item(key, table);

	if (!item) {
		return NO_SUCH_ITEM;
	}

	qsort(item->value->data, item->value->size, sizeof(int), descending);

	return SUCCESS;
}

int Shuffle(char key, hash_table *table) {
	hash_item *item = get_item(key, table);

	if (!item) {
		return NO_SUCH_ITEM;
	}

	for (int i = 0; i < item->value->size; i++) {
		int rand_index = rand() % (i + 1);

		int temp = item->value->data[rand_index];
		item->value->data[rand_index] = item->value->data[i];
		item->value->data[i] = temp;
	}

	return SUCCESS;
}

int Stats_max_min(Array *array, int *min_index, int *max_index) {
	if (!array || array->size == 0) {
		return EMPTY_ARRAY;
	}

	int min = 0;
	int max = 0;

	for (int i = 0; i < array->size; i++) {
		if (array->data[i] < array->data[min]) {
			min = i;
		}
		if (array->data[i] > array->data[max]) {
			max = i;
		}
	}

	*min_index = min;
	*max_index = max;

	return SUCCESS;
}

int Stats_often(Array *array, int *index_often_item) {
	if (!array || array->size == 0) {
		return EMPTY_ARRAY;
	}

	int max_matches = 0;
	int count_matches = 0;
	int index_res = 0;

	for (int i = 0; i < array->size; i++) {
		for (int j = i + 1; j < array->size; j++) {
			if (array->data[i] == array->data[j]) {
				count_matches++;
			}
		}
		if (count_matches > max_matches) {
			max_matches = count_matches;
			index_res = i;
		} else {
			if (count_matches == max_matches) {
				index_res = array->data[index_res] > array->data[i] ? index_res : i;
			}
		}
		count_matches = 0;
	}

	*index_often_item = index_res;

	return SUCCESS;
}

int Stats_avarage(Array *array, double *avarage) {
	if (!array || array->size == 0) {
		return EMPTY_ARRAY;
	}

	int sum = 0;

	for (int i = 0; i < array->size; i++) {
		sum += array->data[i];
	}

	*avarage = (double)sum / array->size;

	return SUCCESS;
}

int Stats_max_deviation(Array *array, double *max_deviation) {
	if (!array || array->size == 0) {
		return EMPTY_ARRAY;
	}

	double avarage = 0.0;
	int exit_code = Stats_avarage(array, &avarage);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	double deviation = 0.0;

	for (int i = 0; i < array->size; i++) {
		if (deviation - fabs(avarage - array->data[i]) < eps) {
			deviation = fabs(avarage - array->data[i]);
		}
	}

	*max_deviation = deviation;

	return SUCCESS;
}

int Stats(char key, hash_table *table) {
	hash_item *h_item = get_item(key, table);

	if (!h_item) {
		return NO_SUCH_ITEM;
	}

	int index_min = 0;
	int index_max = 0;
	int index_often = 0;
	double avarage = 0.0;
	double max_deviation = 0.0;

	if (Stats_max_min(h_item->value, &index_min, &index_max) == SUCCESS &&
		Stats_often(h_item->value, &index_often) == SUCCESS &&
		Stats_avarage(h_item->value, &avarage) == SUCCESS &&
		Stats_max_deviation(h_item->value, &max_deviation) == SUCCESS) {
		printf("Maximum: %d\n", h_item->value->data[index_max]);
		printf("Minimum: %d\n", h_item->value->data[index_min]);
		printf("Most often item: %d\n", h_item->value->data[index_often]);
		printf("Avarage: %lf\n", avarage);
		printf("Max deviation: %lf\n", max_deviation);
	} else {
		return EMPTY_ARRAY;
	}

	return SUCCESS;
}

int Print(char key, int left_board, int right_board, hash_table *table) {
	hash_item *h_item = get_item(key, table);

	if (!h_item) {
		return NO_SUCH_ITEM;
	}

	if (left_board > h_item->value->size ||
		right_board > h_item->value->size ||
		left_board > right_board) {
		return OUT_OF_RANGE;
	}

	for (int i = left_board; i <= right_board; i++) {
		printf("%d ", h_item->value->data[i]);
	}

	printf("\n");

	return SUCCESS;
}

int Print_all(char key, hash_table *table) {
	hash_item *h_item = get_item(key, table);

	if (!h_item) {
		return NO_SUCH_ITEM;
	}

	for (int i = 0; i < h_item->value->size; i++) {
		printf("%d ", h_item->value->data[i]);
	}

	printf("\n");

	return SUCCESS;
}