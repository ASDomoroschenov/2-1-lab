#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/commands.h"
#include "../lib/check.h"

int check_Load(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);
		
		return exit_code;
	}

	if (size != 3 ||
		strcmp(split[0], "Load") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strcmp(split[2] + strlen(split[2]) - 5, ".txt;") != 0) {

		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Save(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);
		return exit_code;
	}

	if (size != 3 ||
		strcmp(split[0], "Save") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strcmp(split[2] + strlen(split[2]) - 5, ".txt;") != 0) {

		for (int i = 0; i < size; i++) {
			free(split[i]);
		}
		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}
	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Rand(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	if (size != 5) {
		free(true_command);

		return INVALID_COMMAND;
	}

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (strcmp(split[0], "Rand") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strlen(split[2]) < 2 ||
		split[2][strlen(split[2]) - 1] != ',' ||
		strlen(split[3]) < 2 ||
		split[3][strlen(split[3]) - 1] != ',' ||
		strlen(split[4]) < 2 ||
		split[4][strlen(split[4]) - 1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	char *values[3] = {NULL, NULL, NULL};

	for (int i = 0; i < 3; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(split[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(split[i]);
			}

			free(split);
			free(true_command);
			return NO_MEMORY;
		}

		strncpy(values[i], split[2 + i], strlen(split[2 + i]) - 1);
		values[i][strlen(split[2 + i]) - 1] = 0;
	}

	if (!isnumber(values[0]) ||
		atoi(values[0]) < 0 ||
		!isnumber(values[1]) ||
		atoi(values[1]) < 0 ||
		!isnumber(values[2]) ||
		atoi(values[2]) < 0) {
		for (int i = 0; i < 3; i++) {
			free(values[i]);
		}

		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < 3; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Concat(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (size != 3 ||
		strcmp(split[0], "Concat") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strlen(split[2]) != 2 ||
		!isalpha(split[2][0]) ||
		split[2][1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Free(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);

	if (size != 1 ||
		strlen(true_command) != 8) {
		free(true_command);

		return INVALID_COMMAND;
	}

	char *command_free = (char*)malloc(sizeof(char) * 5);

	if (!command_free) {
		free(true_command);

		return NO_MEMORY;
	}

	strncpy(command_free, true_command, 4);
	command_free[4] = 0;

	if (strcmp(command_free, "Free") != 0 ||
		strlen(true_command) != 8 ||
		!isalpha(true_command[5]) ||
		true_command[4] != '(' ||
		true_command[6] != ')' ||
		true_command[7] != ';') {
		free(command_free);
		free(true_command);

		return INVALID_COMMAND;
	}

	free(command_free);
	free(true_command);

	return SUCCESS;
}

int check_Remove(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	if (size != 4) {
		free(true_command);

		return INVALID_COMMAND;
	}

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (strcmp(split[0], "Remove") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strlen(split[2]) < 2 ||
		split[2][strlen(split[2]) - 1] != ',' ||
		strlen(split[3]) < 2 ||
		split[3][strlen(split[3]) - 1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	char *values[2] = {NULL, NULL};

	for (int i = 0; i < 2; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(split[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(split[i]);
			}

			free(split);
			free(true_command);

			return NO_MEMORY;
		}

		strncpy(values[i], split[2 + i], strlen(split[2 + i]) - 1);
		values[i][strlen(split[2 + i]) - 1] = 0;
	}

	if (!isnumber(values[0]) ||
		atoi(values[0]) < 0 ||
		!isnumber(values[1]) ||
		atoi(values[1]) < 0) {
		for (int i = 0; i < 2; i++) {
			free(values[i]);
		}

		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < 2; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Copy(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	if (size != 5) {
		free(true_command);

		return INVALID_COMMAND;
	}

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (strcmp(split[0], "Copy") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strlen(split[2]) < 2 ||
		split[2][strlen(split[2]) - 1] != ',' ||
		strlen(split[3]) < 2 ||
		split[3][strlen(split[3]) - 1] != ',' ||
		strlen(split[4]) != 2 ||
		!isalpha(split[4][0]) ||
		split[4][1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	char *values[2] = {NULL, NULL};

	for (int i = 0; i < 2; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(split[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(split[i]);
			}

			free(split);
			free(true_command);

			return NO_MEMORY;
		}

		strncpy(values[i], split[2 + i], strlen(split[2 + i]) - 1);
		values[i][strlen(split[2 + i]) - 1] = 0;
	}

	if (!isnumber(values[0]) ||
		atoi(values[0]) < 0 ||
		!isnumber(values[1]) ||
		atoi(values[1]) < 0) {
		for (int i = 0; i < 2; i++) {
			free(values[i]);
		}

		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < 2; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Sort(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (size != 2 ||
		strcmp(split[0], "Sort") != 0 ||
		strlen(split[1]) != 3 ||
		!isalpha(split[1][0]) ||
		(split[1][1] != '+' && split[1][1] != '-') ||
		split[1][2] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Shuffle(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (size != 2 ||
		strcmp(split[0], "Shuffle") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Stats(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (size != 2 ||
		strcmp(split[0], "Stats") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Print(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	if (size != 4) {
		free(true_command);

		return INVALID_COMMAND;
	}

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	char *values[2] = {NULL, NULL};

	if (strcmp(split[0], "Print") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strlen(split[2]) < 2 ||
		split[2][strlen(split[2]) - 1] != ',' ||
		strlen(split[3]) < 2 ||
		split[3][strlen(split[3]) - 1] != ';') {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < 2; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(split[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(split[i]);
			}

			free(split);
			free(true_command);

			return NO_MEMORY;
		}

		strncpy(values[i], split[2 + i], strlen(split[2 + i]) - 1);
		values[i][strlen(split[2 + i]) - 1] = 0;
	}

	if (!isnumber(values[0]) ||
		atoi(values[0]) < 0 ||
		!isnumber(values[1]) ||
		atoi(values[1]) < 0) {
		for (int i = 0; i < 2; i++) {
			free(values[i]);
		}

		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return 0;
	}

	for (int i = 0; i < 2; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int check_Print_all(char *command) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	char **split = NULL;

	exit_code = split_space(true_command, &split);

	if (exit_code != SUCCESS) {
		free(true_command);

		return exit_code;
	}

	if (size != 3 ||
		strcmp(split[0], "Print") != 0 ||
		strlen(split[1]) != 2 ||
		!isalpha(split[1][0]) ||
		split[1][1] != ',' ||
		strcmp(split[2], "all;") != 0) {
		for (int i = 0; i < size; i++) {
			free(split[i]);
		}

		free(split);
		free(true_command);

		return INVALID_COMMAND;
	}

	for (int i = 0; i < size; i++) {
		free(split[i]);
	}

	free(split);
	free(true_command);

	return SUCCESS;
}

int isnumber(char *str) {
	char *ptr = str - 1;

	if (*str == '-') {
		ptr++;
	}

	while (*++ptr) {
		if (!isdigit(*ptr)) {
			return 0;
		}
	}

	return 1;
}