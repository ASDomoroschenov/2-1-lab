#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define SIZE_DIGIT 15

int check_args(int argc, char const *argv[]);
bool is_number(char *string);
long long notation_10(char *string, int base);
int get_min_base_notation(char *string);
void write_out_file(char const *argv[]);

int main(int argc, char const *argv[]) {
	int check = check_args(argc, argv);

	if (check == 0) {
		write_out_file(argv);
	} else {
		if (check == 1) {
			fprintf(stderr, "%s\n", "File didn't open");
		}
		if (check == 2) {
			fprintf(stderr, "%s\n", "Invalid file extension");
		}
		if (check == 3) {
			fprintf(stderr, "%s\n", "Invalid count of arguments");
		}
	}

	return 0;
}

int check_args(int argc, char const *argv[]) {
	FILE *file = NULL;

	if (argc == 2) {
		if (strlen(argv[1]) > 4 &&
			strcmp(argv[1] + strlen(argv[1]) - 4, ".txt") == 0) {
			if ((file = fopen(argv[1], "r")) != NULL) {
				fclose(file);
				return 0;
			}
			return 1;
		}
		return 2;
	}

	return 3;
}

bool is_number(char *string) {
	for (char *ptr = *string == '-' ? string + 1: string; *ptr; ptr++) {
		if (!(isdigit(*ptr) || isalpha(*ptr))) {
			return false;
		}
	}

	return true;
}

long long notation_10(char *string, int base) {
	int sign = 1;
	long long res = 0;
	char *ptr = NULL;

	if (*string == '-') {
		sign = -1;
		ptr = string;
	} else {
		ptr = string - 1;
	}

	while (*++ptr) {
		res = res * base + (isdigit(*ptr) ? (*ptr - '0') : (toupper(*ptr) - 'A' + 10));
	}

	return sign * res;
}

int get_min_base_notation(char *string) {
	int min_base = 0;
	char *ptr = NULL;

	if (*string == '-') {
		ptr = string;
	} else {
		ptr = string - 1;
	}

	while (*++ptr) {
		if (isdigit(*ptr) && (*ptr - '0' > min_base) ||
			isalpha(*ptr) && (toupper(*ptr) - 'A' + 10 > min_base)) {
			min_base = isalpha(*ptr) ? toupper(*ptr) - 'A' + 10 : *ptr - '0';
		}
	}

	return min_base + 1;
}

void write_out_file(char const *argv[]) {
	FILE *file = NULL;
	FILE *out = fopen("out_file.txt", "w");
	int min_base = 0;
	char digit[SIZE_DIGIT];

	if ((file = fopen(argv[1], "r")) != NULL) {
		while (!feof(file)) {
			if (fscanf(file, "%s", digit) != EOF && is_number(digit)) {
				if (strcmp(digit, "0") != 0) {
					min_base = get_min_base_notation(digit);
					if (min_base >= 2 && min_base <= 36) {
						fprintf(out, "%s - (%d) %ld\n", digit, min_base, notation_10(digit, min_base));
					} else {
						fprintf(out, "%s - Invalid number", digit);
					}
				} else {
					fprintf(out, "%s - Any notation", digit);
				}
			}
		}
		fclose(file);
		fclose(out);
	}
}