#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/string.h"
#include "../headers/tree.h"
#include "../headers/execute.h"

int check_args(int argc, char **argv) {
	if (argc == 2) {
		if (strcmp(argv[1] + strlen(argv[1]) - 4, ".txt") == 0) {
			return SUCCESS;
		} else {
			return NOT_TXT_FILE;
		}
	} else {
		return INVALID_ARGS;
	}
}

int file_processing(int argc, char **argv) {
	FILE *output = NULL;
	FILE *input = NULL;
	Tree *tree = NULL;
	char *str = NULL;
	int exit_code = check_args(argc, argv);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if ((input = fopen(argv[1], "r")) != NULL) {
		if ((output = fopen("output.txt", "w")) != NULL) {
			while (!feof(input)) {
				exit_code = get_str(&str, input);

				if (exit_code != SUCCESS) {
					return exit_code;
				}

				if (str) {
					exit_code = strip(&str);

					if (exit_code != SUCCESS) {
						free(str);
						return exit_code;
					}
					
					exit_code = correct_str(&str);
					
					if (exit_code != SUCCESS) {
						free(str);
						return exit_code;
					}

					exit_code = make_tree(&tree, str);

					if (exit_code != SUCCESS) {
						free(str);
						return exit_code;
					}

					output_tree(tree->root, output);
					fprintf(output, "\n");
					free_tree(&tree);
					free(str);
					str = NULL;
				}
			}

			fclose(input);
			fclose(output);

			return SUCCESS;
		} else {
			return OPEN_FILE;
		}
	} else {
		return OPEN_FILE;
	}
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "NO_MEMORY: The system is out of memory\n");
	}
	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "INVALID_ARGS: You put an invalid arguments\n");
	}
	if (exit_code == NOT_TXT_FILE) {
		fprintf(stderr, "NOT_TXT_FILE: You put the name not txt file\n");
	}
	if (exit_code == OPEN_FILE) {
		fprintf(stderr, "OPEN_FILE: Can't open file\n");
	}
}