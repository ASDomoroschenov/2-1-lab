#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../lib/define.h"

char *get_subtraction(char *str) {
	char *ptr = str + strlen("#define");

	while (isspace(*ptr)) {
		ptr++;
	}

	while (isalnum(*ptr)) {
		ptr++;
	}

	while (isspace(*ptr)) {
		ptr++;
	}

	return ptr;
}

int check_def_name(char *def_name) {
	int len = strlen(def_name);

	for (int i = 0; i < len; i++) {
		if (!isdigit(def_name[i]) && !isalpha(def_name[i])) {
			return 0;
		}
	}

	return 1;
}

int make_substitution(char *name_file) {
	hash_table table = {NULL, 0, 0, 0};
	FILE *read_file = NULL;
	FILE *output_file = NULL;
	int count_tokens = 0;
	char **tokens = NULL;
	int exit_code = 0;
	char *str = NULL;

	if ((read_file = fopen(name_file, "r")) != NULL &&
		(output_file = fopen("temp.txt", "w")) != NULL) {
		while (!feof(read_file)) {
			exit_code = get_str(&str, read_file);

			if (exit_code != SUCCESS) {
				fclose(read_file);
				fclose(output_file);
				free_table(&table);

				return exit_code;
			}

			if (str) {
				exit_code = strip(&str);

				if (exit_code != SUCCESS) {
					free(str);
					fclose(read_file);
					fclose(output_file);
					free_table(&table);

					return exit_code;
				}

				count_tokens = token_count(str);

				exit_code = split_space(str, &tokens);

				if (exit_code != SUCCESS) {
					free(str);
					fclose(read_file);
					fclose(output_file);
					free_table(&table);

					return exit_code;
				}

				if (count_tokens > 2 &&
					strcmp(tokens[0], "#define") == 0 &&
					check_def_name(tokens[1])) {
						add_to_table(tokens[1], get_subtraction(str), &table);
						fprintf(output_file, "%s\n", str);
				} else {
					if (strcmp(tokens[0], "#define") != 0) {
						int index = 0;
						int len = strlen(str);
						char *subtraction = NULL;

						for (int i = 0; i < len; i++) {
							if (!isspace(str[i])) {
								subtraction = get_value(tokens[index], table);

								if (subtraction) {
									fprintf(output_file, "%s", subtraction);
								} else {
									fprintf(output_file, "%s", tokens[index]);
								}

								i += strlen(tokens[index++]) - 1;
							} else {
								fprintf(output_file, "%c", str[i]);
							}
						}

						fprintf(output_file, "\n");
					} else {
						fprintf(output_file, "%s\n", str);
					}
				}
				for (int i = 0; i < count_tokens; i++) {
					free(tokens[i]);
					tokens[i] = NULL;
				}

				free(tokens);
				tokens = NULL;
				free(str);
				str = NULL;
			} else {
				fprintf(output_file, "\n");
			}

			if (table.min_size_list && table.max_size_list / table.min_size_list >= 2) {
				exit_code = rebuild_table(&table);

				if (exit_code != SUCCESS) {
					fclose(read_file);
					fclose(output_file);
					free_table(&table);

					return exit_code;
				}
			}
		}

		fclose(read_file);
		fclose(output_file);
		free_table(&table);
		remove(name_file);
		rename("temp.txt", name_file);
	} else {
		return CANT_OPEN_FILE;
	}

	return SUCCESS;
}