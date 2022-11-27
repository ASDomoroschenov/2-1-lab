#include <stdio.h>
#include <stdlib.h>
#include "../lib/types.h"
#include "../lib/execute.h"

int main(int argc, char *argv[]) {
	int exit_code = files_processing(argc, argv);

	if (exit_code != SUCCESS) {
		print_error(exit_code, stderr);
		return exit_code;
	}

	return 0;
}