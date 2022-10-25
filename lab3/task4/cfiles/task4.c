#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "../headers/csv_file.h"
#include "../headers/string.h"
#include "../headers/message.h"

void print_error(int);

int main(int argc, char *argv[]) {
	srand(time(NULL));

	if (argc == 2) {
		message **messages = NULL;
		size_t count_messages = 0;
		int code_get_messages = 0;
		int code_fill_file = 0;
		char *name_file = NULL;

		if (generate_file_name(&name_file) == SUCCESS) {
			char fullname[512];
			strcat(fullname, "csv_file/");
			strcat(fullname, name_file);
			code_fill_file = fill_file(fullname, argv[1], &count_messages);

			if (code_fill_file == SUCCESS) {
				code_get_messages = get_messages_from_csv(fullname, &messages, count_messages);
			
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