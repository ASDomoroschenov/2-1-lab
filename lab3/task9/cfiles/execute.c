#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/execute.h"
#include "../headers/dbl_list.h"
#include "../headers/binom_heap.h"
#include "../headers/string.h"
#include "../headers/check.h"

int cmp(T *item_1, T *item_2) {
	return item_1->prior <= item_2->prior;
}

int check_args(int argc, char **argv) {
	FILE *file = NULL;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i] + strlen(argv[i]) - 4, ".txt") != 0) {
				return NOT_TXT_FILE;
			}
			if ((file = fopen(argv[i], "r")) == NULL) {
				return OPEN_FILE;
			}
			fclose(file);
		}

		return SUCCESS;
	}

	return INVALID_ARGS;
}

int is_all_file_closed(int *closed, int size) {
	for (int i = 0; i < size; i++) {
		if (!closed[i]) {
			return 0;
		}
	}

	return 1;
}

int get_list(dbl_list **list, int argc, char **argv) {
	int count_files = argc - 1;
	FILE *opened_files[count_files];
	int closed[count_files];
	char *str = NULL;
	int exit_code = 0;

	for (int i = 0; i < count_files; i++) {
		closed[i] = 0;
		if ((opened_files[i] = fopen(argv[i + 1], "r")) == NULL) {
			return OPEN_FILE;
		}
	}

	while (!is_all_file_closed(closed, count_files)) {
		for (int i = 0; i < count_files; i++) {
			if (!closed[i]) {
				exit_code = get_str(&str, opened_files[i]);

				if (exit_code != SUCCESS) {
					return exit_code;
				}

				if (str) {
					exit_code = strip(&str);

					if (exit_code != SUCCESS) {
						return exit_code;
					}
					
					if (check_str(str)) {
						add_list(list, str);
					}
					free(str);
					str = NULL;
				}

				if (feof(opened_files[i])) {
					closed[i] = 1;
				}
			}
		}
	}

	for (int i = 0; i < count_files; i++) {
		fclose(opened_files[i]);
	}

	return SUCCESS;
}

int get_heap(binom_heap **heap, int argc, char **argv) {
	int count_files = argc - 1;
	FILE *opened_files[count_files];
	int closed[count_files];
	char *str = NULL;
	int exit_code = 0;

	for (int i = 0; i < count_files; i++) {
		closed[i] = 0;
		if ((opened_files[i] = fopen(argv[i + 1], "r")) == NULL) {
			return OPEN_FILE;
		}
	}

	while (!is_all_file_closed(closed, count_files)) {
		for (int i = 0; i < count_files; i++) {
			if (!closed[i]) {
				exit_code = get_str(&str, opened_files[i]);

				if (exit_code != SUCCESS) {
					return exit_code;
				}

				if (str) {
					exit_code = strip(&str);

					if (exit_code != SUCCESS) {
						return exit_code;
					}

					if (check_str(str)) {
						add_node(heap, str, cmp);
					}
					free(str);
					str = NULL;
				}

				if (feof(opened_files[i])) {
					closed[i] = 1;
				}
			}
		}
	}

	for (int i = 0; i < count_files; i++) {
		fclose(opened_files[i]);
	}

	return SUCCESS;
}

int output_list_file(dbl_list **list, char *name_file) {
	FILE *output = NULL;
	int exit_code = 0;

	if ((output = fopen(name_file, "w")) != NULL) {
		exit_code = output_list(list, output);
		fclose(output);

		if (exit_code != SUCCESS) {
			return exit_code;
		}
		
		return SUCCESS;
	}

	return OPEN_FILE;
}

int output_heap_file(binom_heap **heap, char *name_file) {
	FILE *output = NULL;
	int exit_code = 0;

	if ((output = fopen(name_file, "w")) != NULL) {
		exit_code = output_heap(heap, output, cmp);
		fclose(output);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		return SUCCESS;
	}

	return OPEN_FILE;
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "NO_MEMORY: The system is out of memory\n");
	}
	if (exit_code == EMPTY_LIST) {
		fprintf(stderr, "EMPTY_LIST: You can interact with empty list\n");
	}
	if (exit_code == EMPTY_HEAP) {
		fprintf(stderr, "EMPTY_HEAP: You can interact with empty heap\n");
	}
	if (exit_code == OPEN_FILE) {
		fprintf(stderr, "OPEN_FILE: Can't open file\n");
	}
	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "INVALID_ARGS: You put an invalid arguments\n");
	}
	if (exit_code == NOT_TXT_FILE) {
		fprintf(stderr, "NOT_TXT_FILE: You put the name not txt file\n");
	}
}