#include <stdlib.h>
#include <ctype.h>
#include "../headers/string.h"
#include "../headers/check.h"

int check_str(char *str) {
	char *ptr = str;
	char prior[6] = "prior=";
	char task[5] = "task=";

	if (token_count(str) < 2) {
		return 0;
	}

	for (int i = 0; i < 6; i++) {
		if (*ptr++ != prior[i]) {
			return 0;
		}
	}

	ptr++;

	if (*ptr == '-') {
		ptr++;
	}

	while (!isspace(*ptr)) {
		if (!isdigit(*ptr)) {
			return 0;
		}
		ptr++;
	}

	while (isspace(*ptr)) {
		ptr++;
	}

	for (int i = 0; i < 5; i++) {
		if (*ptr++ != task[i]) {
			return 0;
		}
	}

	return *ptr ? 1 : 0;
}