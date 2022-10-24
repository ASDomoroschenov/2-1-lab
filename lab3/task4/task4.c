#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum ERRORS {
	NO_MEMORY = -1,
	SUCCESS = -2,
	INVALID_MESSAGE = -3,
	OPEN_FILE = -4
};

typedef struct {
	size_t id;
	char *text_message;
	size_t length;
} message;

int join_symb(char, char**, size_t*);
int get_token_str(char**, char**);
int token_count(char*);
int get_str(char**, FILE*);
int check_id(char*);
int check_length(char*);
int check_message(char*);
int get_message(char*, message**);
int try_get_valid_message(char**);
int write_in_file(FILE*, message*);
int fill_file(char*, size_t*);
int get_messages_from_csv(FILE*, message**, size_t);

int main(int argc, char *argv[]) {
	if (argc == 2) {
		message **messages = NULL;
		size_t count_messages = 0;

		fill_file(argv[1], &count_messages);

		messages = (message**)malloc(sizeof(message*) * count_messages);
		for (int i = 0; i < count_messages; i++) {
			messages[i] = (message*)malloc(sizeof(message));
		}

		FILE *csv_file = fopen("test.csv", "r");
		get_messages_from_csv(csv_file, messages, count_messages);
		fclose(csv_file);
	} else {
		fprintf(stderr, "%s\n", "INVALID_ARGS: Invalid count of arguments");
	}

	return 0;
}

int join_symb(char symb, char **str, size_t *size) {
	if (*size == 0) {
		*size = 2;
		*str = (char*)malloc(sizeof(char) * *size);
		if (!*str) {
			return NO_MEMORY;
		}

		(*str)[0] = symb;
		(*str)[1] = 0;

		return SUCCESS;
	}

	if (strlen(*str) == *size - 1) {
		*size *= 2;
		*str = (char*)realloc(*str, sizeof(char) * *size);
		if (!*str) {
			return NO_MEMORY;
		}
	}

	(*str)[strlen(*str) + 1] = 0;
	(*str)[strlen(*str)] = symb;

	return SUCCESS;
}

int get_token_str(char **ptr, char **token) {
	size_t size = 0;

	while (isspace(**ptr)) {
		(*ptr)++;
	}

	while (!isspace(**ptr) && **ptr) {
		if (join_symb(**ptr, token, &size) == NO_MEMORY) {
			return NO_MEMORY;
		}
		(*ptr)++;
	}

	return SUCCESS;
}

int token_count(char *str) {
	char _c = ' ';
	char *ptr = str - 1;
	size_t count = 0;

	while (*++ptr) {
		if (isspace(_c) && !isspace(*ptr)) {
			count++;
		}
		_c = *ptr;
	}

	return count;
}

int get_str(char **str, FILE *input) {
	char symb = fgetc(input);
	int exit_code = 0;
	size_t size = 0;

	while (symb != '\n') {
		exit_code = join_symb(symb, str, &size);
		
		if (exit_code != SUCCESS) {
			return exit_code;
		}
		
		symb = fgetc(input);
	}

	return SUCCESS;
}

int check_id(char *id) {
	char *ptr = id - 1;

	while (*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return atoi(id) > 0;
}

int check_length(char *length) {
	char *ptr = length - 1;

	while (*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return atoi(length) > 0;
}

int check_message(char *str) {
	char *ptr = str;

	if (token_count(str) != 3) {
		return INVALID_MESSAGE;
	} else {
		char *id = NULL;
		char *text_msg = NULL;
		char *length = NULL;

		if (get_token_str(&ptr, &id) == SUCCESS &&
			get_token_str(&ptr, &text_msg) == SUCCESS &&
			get_token_str(&ptr, &length) == SUCCESS) {
			if (check_id(id) && check_length(length)) {
				free(id);
				free(text_msg);
				free(length);
				return SUCCESS;
			} else {
				free(id);
				free(text_msg);
				free(length);
				return INVALID_MESSAGE;
			}
		} else {
			return NO_MEMORY;
		}
	}
}

int get_message(char *str, message **msg) {
	char *ptr = str;
	char *id = NULL;
	char *text_msg = NULL;
	char *length = NULL;

	if (get_token_str(&ptr, &id) == SUCCESS &&
		get_token_str(&ptr, &text_msg) == SUCCESS &&
		get_token_str(&ptr, &length) == SUCCESS) {
		(*msg)->id = atoi(id);
		(*msg)->text_message = text_msg;
		(*msg)->length = atoi(length);
		free(id);
		free(length);
	} else {
		return NO_MEMORY;
	}

	return SUCCESS;
}

int try_get_valid_message(char **str) {
	int count = 0;

	fprintf(stderr, "%s\n", "INVALID_MESSAGE: Please enter a valid message");

	do {
		free(*str);

		if (get_str(str, stdin) == NO_MEMORY) {
			return NO_MEMORY;
		}

		count++;
	} while (count < 3 && check_message(*str) == INVALID_MESSAGE);

	return count;
}

int write_in_file(FILE *csv_file, message *item) {
	fprintf(csv_file, "%d,%s,%d\n", item->id, item->text_message, item->length);
}

int fill_file(char *stop_word, size_t *count_messages) {
	FILE *csv_file = NULL;
	char *str = NULL;
	int code_get_str = 0;
	int code_get_message = 0;
	int code_get_valid_message = 0;
	int count = 0;

	if ((csv_file = fopen("test.csv", "w")) != NULL) {
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
						code_get_valid_message = try_get_valid_message(&str);

						if (code_get_valid_message == NO_MEMORY) {
							fclose(csv_file);

							return NO_MEMORY;
						} else {
							if (code_get_valid_message == 3) {
								free(str);
								fclose(csv_file);

								return INVALID_MESSAGE;
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
						free(item->text_message);
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

int get_messages_from_csv(FILE *csv_file, message **messages, size_t count_messages) {
	for (int i = 0; i < count_messages; i++) {
		char *str = NULL;
		int code_get_str = get_str(&str, csv_file);
		
		if (code_get_str != SUCCESS) {
			return code_get_str;
		}

		char *item = strtok(str, ",");

		while (item != NULL) {
			printf("%s\n", item);
			item = strtok(NULL, ",");
		}
	}
}