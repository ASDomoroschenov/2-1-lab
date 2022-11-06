#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/types.h"
#include "../headers/check.h"
#include "../headers/post.h"
#include "../headers/string.h"
#include "../headers/binom_heap.h"
#include "../headers/dialog.h"

//Moscow Chertanovskaya 24 226 117552 Moscow Bolotnikovckaya 10 121 117556 1.5 01234567891234 30.10.2022 17:19:54 03.11.2022 18:00:00
//SBP Pushkina 34 134 117456 Moscow Chertanovskaya 24 226 117552 2 01234567891112 31.10.2022 12:18:00 03.11.2022 20:00:00

int main(int argc, char *argv[]) {
	if (argc == 3) {
		char datetime[BUFSIZ];
		strcat(datetime, argv[1]);
		strcat(datetime, " ");
		strcat(datetime, argv[2]);
		
		int exit_code = dialog(datetime);

		if (exit_code != SUCCESS) {
			print_error(exit_code);
		}
	} else {
		print_error(INVALID_ARGS);
	}

	return 0;
}