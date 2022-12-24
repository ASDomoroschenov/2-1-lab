#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/string.h"
#include "../lib/execute.h"

int execute_command(char *str, peasant **user) {
	if (!str || is_empty_str(str)) {
		return EMPTY_STRING;
	}

	int count_tokens = token_count(str);
	char **tokens = NULL;
	int exit_code = 0;
	int len_str = strlen(str);

	if (str[len_str - 1] != ';') {
		return WRONG_COMMAND;
	}

	char *temp = (char*)malloc(sizeof(char) * len_str);

	if (!temp) {
		return NO_MEMORY;
	}

	strncpy(temp, str, len_str - 1);
	temp[len_str - 1] = 0;

	if (count_tokens < 3) {
		if ((exit_code = split_space(temp, &tokens)) != SUCCESS) {
			return exit_code;
		}
		if (count_tokens == 2 && 
			strcmp(tokens[0], "take") == 0) {
			exit_code = take(user, tokens[1]);
			for (int i = 0; i < count_tokens; i++) {
				free(tokens[i]);
				tokens[i] = NULL;
			}

			free(tokens);
			free(temp);
			tokens = NULL;
			temp = NULL;

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			return check_coasts(user);
		}
		if (count_tokens == 1 &&
			strcmp(tokens[0], "put") == 0) {
			exit_code = put(user);

			for (int i = 0; i < count_tokens; i++) {
				free(tokens[i]);
				tokens[i] = NULL;
			}

			free(tokens);
			free(temp);
			tokens = NULL;
			temp = NULL;

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			return check_coasts(user);
		}
		if (count_tokens == 1 && 
			strcmp(tokens[0], "move") == 0) {
			exit_code = move(user);

			for (int i = 0; i < count_tokens; i++) {
				free(tokens[i]);
				tokens[i] = NULL;
			}

			free(tokens);
			free(temp);
			tokens = NULL;
			temp = NULL;

			if (exit_code != SUCCESS) {
				return exit_code;
			}

			return check_coasts(user);
		}
	}

	free(temp);
	
	return WRONG_COMMAND;
}