#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "../lib/interpreter.h"

int main(int argc, char *argv[]) {
	srand(time(NULL));
	
	int line_error = 0;
	int exit_code = interpreter(&line_error);

	if (exit_code != SUCCESS) {
		print_error(exit_code, line_error);
	}

	return 0;
}