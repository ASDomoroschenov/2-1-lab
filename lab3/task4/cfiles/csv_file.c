#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/csv_file.h"
#include "../headers/message.h"
#include "../headers/string.h"

int generate_file_name(char **name) {
	size_t length = rand() % 10 + 1;
	length += 5;

	*name = (char*)malloc(sizeof(char) * length);

	if (!*name) {
		return NO_MEMORY;
	}

	for (int i = 0; i < length - 4; i++) {
		int is_letter = rand() % 2;

		if (is_letter == 1) {
			int upper_lower = rand() % 2;
			
			if (upper_lower == 1) {
				(*name)[i] = (char)(rand() % 26 + 'A');
			} else {
				(*name)[i] = (char)(rand() % 26 + 'a');
			}
		} else {
			(*name)[i] = (char)(rand() % 10 + '0');
		}
	}

	strcat(*name, ".csv");

	return SUCCESS;
}

int write_in_file(FILE *csv_file, message *item) {
	char *ptr = item->text_message;
	char *token = NULL;
	int count = 0;
	int count_tokens = token_count(item->text_message);

	fprintf(csv_file, "%d,", item->id);

	for (int i = 0; i < count_tokens; i++) {
		if (get_token_str(&ptr, &token) == SUCCESS) {
			if (i == count_tokens - 1) {
				fprintf(csv_file, "%s\n", token);
			} else {
				fprintf(csv_file, "%s,", token);
			}
			free(token);
		} else {
			return NO_MEMORY;
		}
	}
}

int fill_file(char *name_file, char *stop_word, size_t *count_messages) {
	FILE *csv_file = NULL;
	char *str = NULL;
	int code_get_str = 0;
	int code_get_message = 0;
	int code_get_valid_message = 0;
	int count = 0;

	if ((csv_file = fopen(name_file, "w")) != NULL) {
		do {
			if (str) {
				free(str);
			}

			code_get_str = get_str(&str, stdin);

			if (code_get_str != SUCCESS) {
				fclose(csv_file);
				return code_get_str;
			} else {
				if (strcmp(str, stop_word) != 0) {
					if (check_message(str) == INVALID_MESSAGE) {
						code_get_valid_message = try_get_valid_message(&str, stop_word);

						if (code_get_valid_message == NO_MEMORY) {
							fclose(csv_file);
							return NO_MEMORY;
						} else {
							if (code_get_valid_message == 3) {
								free(str);
								fclose(csv_file);
								return INVALID_MESSAGE;
							} else {
								if (strcmp(str, stop_word) == 0) {
									return SUCCESS;
								}
							}
						}
					}

					message *item = (message*)malloc(sizeof(message));

					if (!item) {
						free(str);
						fclose(csv_file);
						return NO_MEMORY;
					}

					code_get_message = get_message(str, &item);

					if (code_get_message != SUCCESS) {
						fclose(csv_file);
						return code_get_message;
					} else {
						write_in_file(csv_file, item);
						free(item);
						(*count_messages)++;
					}
				}
			}
		} while (strcmp(str, stop_word) != 0);

		fclose(csv_file);
		free(str);

		return SUCCESS;
	}

	return OPEN_FILE;
}