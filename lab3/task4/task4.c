#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

enum ERRORS {
	NO_MEMORY = -1,
	SUCCESS = -2,
	INVALID_MESSAGE = -3,
	OPEN_FILE = -4,
	INVALID_ARGS = -5
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
int check_message(char*);
int get_message(char*, message**);
int try_get_valid_message(char**, char*);
int generate_file_name(char**);
int write_in_file(FILE*, message*);
int fill_file(char*, char*, size_t*);
int get_messages_from_csv(char*, message***, size_t);
void output_messages(message**, size_t);
void free_messages(message***, size_t);
void print_error(int exit_code);

int main(int argc, char *argv[]) {
	srand(time(NULL));

	if (argc == 2) {
		message **messages = NULL;
		size_t count_messages = 0;
		int code_get_messages = 0;
		int code_fill_file = 0;
		char *name_file = NULL;

		if (generate_file_name(&name_file) == SUCCESS) {
			code_fill_file = fill_file(name_file, argv[1], &count_messages);

			if (code_fill_file == SUCCESS) {
				code_get_messages = get_messages_from_csv(name_file, &messages, count_messages);
			
				if (code_get_messages == SUCCESS) {
					output_messages(messages, count_messages);
				} else {
					print_error(code_get_messages);
				}
				
				free_messages(&messages, count_messages);
				free(name_file);
			} else {
				print_error(code_fill_file);
			}
		} else {
			print_error(NO_MEMORY);
		}
	} else {
		print_error(INVALID_ARGS);
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

	while (!isspace(**ptr) && **ptr && **ptr != EOF) {
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

int check_message(char *str) {
	char *ptr = str;

	if (token_count(str) < 2) {
		return INVALID_MESSAGE;
	} else {
		char *id = NULL;

		if (get_token_str(&ptr, &id) == SUCCESS) {
			if (check_id(id)) {
				free(id);
				return SUCCESS;
			} else {
				free(id);
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

	if (get_token_str(&ptr, &id) == SUCCESS) {
		(*msg)->id = atoi(id);
		(*msg)->text_message = ++ptr;
		(*msg)->length = strlen((*msg)->text_message);
		free(id);
	} else {
		return NO_MEMORY;
	}

	return SUCCESS;
}

int try_get_valid_message(char **str, char *stop_word) {
	int count = 0;

	fprintf(stderr, "%s\n", "INVALID_MESSAGE: Please enter a valid message");

	do {
		free(*str);

		if (get_str(str, stdin) == NO_MEMORY) {
			return NO_MEMORY;
		}
		
		count++;
	} while (count < 3 && check_message(*str) == INVALID_MESSAGE && strcmp(*str, stop_word) != 0);

	if (count < 3) {
		printf("SUCCESS\n");
	}

	return count;
}

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

int get_messages_from_csv(char *name_file, message ***messages, size_t count_messages) {
	FILE *csv_file = NULL;

	*messages = (message**)malloc(sizeof(message*) * count_messages);
	
	if (!*messages) {
		return NO_MEMORY;
	}

	for (int i = 0; i < count_messages; i++) {
		(*messages)[i] = (message*)malloc(sizeof(message));

		if (!(*messages)[i]) {
			return NO_MEMORY;
		}
	}

	if ((csv_file = fopen(name_file, "r")) != NULL) {
		for (int i = 0; i < count_messages; i++) {
			char *str = NULL;
			int code_get_str = get_str(&str, csv_file);
			
			if (code_get_str != SUCCESS) {
				return code_get_str;
			}

			char *id;
			char *ptr = str;
			size_t size = 0;

			while (*ptr != ',') {
				join_symb(*ptr, &id, &size);
				ptr++;
			}

			(*messages)[i]->id = atoi(id);
			free(id);
			ptr++;
			(*messages)[i]->text_message = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));
			(*messages)[i]->length = strlen(ptr);

			for (int j = 0; j < (*messages)[i]->length; j++) {
				if (*ptr == ',') {
					(*messages)[i]->text_message[j] = ' ';
				} else {
					(*messages)[i]->text_message[j] = *ptr;
				}
				ptr++;
			}
			
			free(str);
		}

		fclose(csv_file);
	} else {
		return OPEN_FILE;
	}

	return SUCCESS;
}

void output_messages(message **messages, size_t count_messages) {
	for (int i = 0; i < count_messages; i++) {
		printf("%d %s %d\n", messages[i]->id, messages[i]->text_message, messages[i]->length);
	}
}

void free_messages(message ***messages, size_t count_messages) {
	for (int i = 0; i < count_messages; i++) {
		if ((*messages)[i]->text_message) {
			free((*messages)[i]->text_message);
		}
		if ((*messages)[i]) {
			free((*messages)[i]);
		}
	}

	if (*messages) {
		free(*messages);
	}
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
	}

	if (exit_code == INVALID_MESSAGE) {
		fprintf(stderr, "%s\n", "INVALID_MESSAGE: You put an invalid message");
	}

	if (exit_code == OPEN_FILE) {
		fprintf(stderr, "%s\n", "ERROR_OPEN_FILE: Can't open file");
	}

	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "%s\n", "INVALID_ARGS: Invalid count of arguments");
	}
}