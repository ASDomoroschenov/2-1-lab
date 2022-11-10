#include <stdio.h>
#include <stdlib.h>
#include "../headers/types.h"
#include "../headers/execute.h"

int main(int argc, char *argv[]) {
	int exit_code = 0;

	if (argc == 2) {
		exit_code = instruction_processing("output.txt", atoi(argv[1]));

		if (exit_code != SUCCESS) {
			print_error(exit_code);
			return exit_code;
		}
	} else {
		print_error(INVALID_ARGS);
		return INVALID_ARGS;
	}

	return 0;
}