#include <stdio.h>
#include <stdlib.h>
#include "../lib/types.h"
#include "../lib/string.h"
#include "../lib/hash.h"
#include "../lib/define.h"

void print_error(int);

int main(int argc, char *argv[]) {
	int exit_code = 0;
	
	if (argc == 2) {
		exit_code = make_substitution(argv[1]);

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

void print_error(int exit_code) {
	char *errors[3] = {
		"NO_MEMORY: The system is out of memory",
		"CANT_OPEN_FILE: Can't open file",
		"INVALID_ARGS: You put an invalid arguments"
	};

	fprintf(stderr, "%s\n", errors[abs(exit_code) - 2]);
}