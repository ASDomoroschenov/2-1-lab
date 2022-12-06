#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/execute_command.h"

typedef int (*execute_func)(char*, hash_table**);
typedef int (*check_func)(char*);

int execute_Load(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	char *name_file = (char*)malloc(sizeof(char) * strlen(args[2]));
	
	if (!name_file) {
		return NO_MEMORY;
	}

	strncpy(name_file, args[2], strlen(args[2]) - 1);
	name_file[strlen(args[2]) - 1] = 0;

	exit_code = Load(args[1][0], name_file, table);

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(true_command);
	free(name_file);

	return exit_code;
}

int execute_Save(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		free(true_command);
		return exit_code;
	}

	char *name_file = (char*)malloc(sizeof(char) * strlen(args[2]));
	
	if (!name_file) {
		for (int i = 0; i < size; i++) {
			free(args[i]);
		}

		free(args);
		free(true_command);

		return NO_MEMORY;
	}

	strncpy(name_file, args[2], strlen(args[2]) - 1);
	name_file[strlen(args[2]) - 1] = 0;

	exit_code = Save(args[1][0], name_file, *table);

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(name_file);
	free(true_command);

	return exit_code;
}

int execute_Rand(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	char *values[3] = {NULL, NULL, NULL};

	for (int i = 0; i < 3; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(args[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(args[i]);
			}

			free(args);
			free(true_command);
			return NO_MEMORY;
		}

		strncpy(values[i], args[2 + i], strlen(args[2 + i]) - 1);
		values[i][strlen(args[2 + i]) - 1] = 0;
	}

	exit_code = Rand(args[1][0], atoi(values[0]), atoi(values[1]), atoi(values[2]), table);

	for (int i = 0; i < 3; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(true_command);

	return exit_code;
}

int execute_Concat(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = Concat(args[1][0], args[2][0], *table);

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(true_command);

	return exit_code;
}

int execute_Free(char *command, hash_table **table) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = Free(true_command[5], *table);

	free(true_command);

	return exit_code;
}

int execute_Remove(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	char *values[2] = {NULL, NULL};

	for (int i = 0; i < 2; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(args[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(args[i]);
			}

			free(args);
			free(true_command);
			return NO_MEMORY;
		}

		strncpy(values[i], args[2 + i], strlen(args[2 + i]) - 1);
		values[i][strlen(args[2 + i]) - 1] = 0;
	}

	exit_code = Remove(args[1][0], atoi(values[0]), atoi(values[1]), *table);

	for (int i = 0; i < 2; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(true_command);

	return exit_code;
}

int execute_Copy(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	char *values[2] = {NULL, NULL};

	for (int i = 0; i < 2; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(args[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(args[i]);
			}

			free(args);
			free(true_command);
			return NO_MEMORY;
		}

		strncpy(values[i], args[2 + i], strlen(args[2 + i]) - 1);
		values[i][strlen(args[2 + i]) - 1] = 0;
	}

	exit_code = Copy(args[1][0], atoi(values[0]), atoi(values[1]), args[4][0], table);

	for (int i = 0; i < 2; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(true_command);

	return exit_code;
}

int execute_Sort(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (true_command[6] == '+') {
		exit_code = Sort_ascending(true_command[5], *table);
	} else {
		exit_code = Sort_descending(true_command[5], *table);
	}

	free(true_command);

	return exit_code;
}

int execute_Shuffle(char *command, hash_table **table) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = Shuffle(true_command[8], *table);

	free(true_command);

	return exit_code;
}

int execute_Stats(char *command, hash_table **table) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = Stats(true_command[6], *table);

	free(true_command);

	return exit_code;
}

int execute_Print(char *command, hash_table **table) {
	char *true_command = NULL;
	char **args = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int size = token_count(true_command);
	exit_code = split_space(true_command, &args);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	char *values[2] = {NULL, NULL};

	for (int i = 0; i < 2; i++) {
		values[i] = (char*)malloc(sizeof(char) * strlen(args[2 + i]));

		if (!values[i]) {
			for (int i = 0; i < size; i++) {
				free(args[i]);
			}

			free(args);
			free(true_command);
			return NO_MEMORY;
		}

		strncpy(values[i], args[2 + i], strlen(args[2 + i]) - 1);
		values[i][strlen(args[2 + i]) - 1] = 0;
	}

	exit_code = Print(args[1][0], atoi(values[0]), atoi(values[1]), *table);

	for (int i = 0; i < 2; i++) {
		free(values[i]);
	}

	for (int i = 0; i < size; i++) {
		free(args[i]);
	}

	free(args);
	free(true_command);

	return exit_code;
}

int execute_Print_all(char *command, hash_table **table) {
	char *true_command = NULL;
	int exit_code = make_command(command, &true_command);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	exit_code = Print_all(true_command[6], *table);

	free(true_command);

	return exit_code;
}

int execute(char *command, hash_table **table) {
	int exit_code = 0;

	execute_func exec_funcs[12] = {
		execute_Load,
		execute_Save,
		execute_Rand,
		execute_Concat,
		execute_Free,
		execute_Remove,
		execute_Copy,
		execute_Sort,
		execute_Shuffle,
		execute_Stats,
		execute_Print,
		execute_Print_all
	};

	check_func check_funcs[12] = {
		check_Load,
		check_Save,
		check_Rand,
		check_Concat,
		check_Free,
		check_Remove,
		check_Copy,
		check_Sort,
		check_Shuffle,
		check_Stats,
		check_Print,
		check_Print_all
	};

	for (int i = 0; i < 12; i++) {
		exit_code = check_funcs[i](command);

		if (exit_code == SUCCESS) {
			return exec_funcs[i](command, table);
		} else {
			if (exit_code == EMPTY_STRING) {
				return exit_code;
			}
		}
	}

	return INVALID_COMMAND;
}