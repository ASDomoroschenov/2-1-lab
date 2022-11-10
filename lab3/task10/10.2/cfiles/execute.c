#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/list.h"
#include "../headers/execute.h"
#include "../headers/string.h"

int save_in_file(char *name_file, List **list) {
	FILE *write_file = NULL;

	if ((write_file = fopen(name_file, "a")) != NULL) {
		if (*list) {
			while ((*list)->root) {
				fprintf(write_file, "%s\n", (*list)->root->key);
				get_front(list);
			}
		}
		
		fclose(write_file);

		return SUCCESS;
	}

	return OPEN_FILE;
}

int delete_instructions(List **list, int count) {
	if (*list && (*list)->root) {
		for (int i = 0; (*list)->root && i < count; i++) {
			get_back(list);
		}
	}

	return SUCCESS;
}

int instruction_processing(char *name_file, int count) {
	List *list = NULL;
	char *str = NULL;
	int size_help = 0;
	int count_help = 0;
	int exit_code = 0;

	do {
		if (str) {
			free(str);
			str = NULL;
		}

		exit_code = get_str(&str, stdin);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		if (str) {
			exit_code = strip(&str);

			if (exit_code != SUCCESS) {
				free(str);

				return exit_code;
			}

			if (strcmp(str, "DELETE") == 0) {
				count_help--;
				delete_instructions(&list, count / 2);
			} else {
				if (strcmp(str, "STOP") != 0) {
					count_help++;
			
					exit_code = push_back(&list, str);

					if (exit_code != SUCCESS) {
						free(str);
						free_list(&list);

						return exit_code;
					}

					if (count_help == count) {
						exit_code = save_in_file(name_file, &list);
						
						if (exit_code != SUCCESS) {
							free(str);
							free_list(&list);

							return exit_code;
						}

						count_help = 0;
					}
				}
			}
		} else {
			fprintf(stderr, "EMPTY_STR: enter a non-empty string\n");
			join_symb('1', &str, &size_help);
			size_help = 0;
		}
	} while (strcmp(str, "STOP") != 0);

	free(str);
	free_list(&list);

	return SUCCESS;
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "NO_MEMORY: The system is out of memory\n");
	}
	if (exit_code == OPEN_FILE) {
		fprintf(stderr, "OPEN_FILE: Can't open file\n");
	}
	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "INVALID_ARGS: You put an invalid arguments\n");
	}
}