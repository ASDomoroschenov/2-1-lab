#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/message.h"
#include "../headers/string.h"

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