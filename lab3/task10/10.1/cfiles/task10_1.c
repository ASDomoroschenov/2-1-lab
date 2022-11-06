#include <stdio.h>
#include "../headers/types.h"
#include "../headers/stack.h"
#include "../headers/string.h"
#include "../headers/tree.h"
#include "../headers/execute.h"

int main(int argc, char *argv[]) {
	int exit_code = 0;
	exit_code = file_processing(argc, argv);

	if (exit_code != SUCCESS) {
		print_error(exit_code);

		return exit_code;
	}

	return 0;
}