#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>

enum ERRORS {
	INVALID_NUMBER = -1,
	SUCCES = -2,
	NO_MEMORY = -3
};

#define max(a, b) ((a > b) ? (a) : (b))

bool is_valid_number(char *number, int notation);
int sum_notation(char **res, char *number_1, char *number_2, int notation);
int sum_numbers(char **res, int notation, int count, ...);
char *remove_leading_zeros(char *res);

int main() {
	char *number_1 = "111ABC";
	char *number_2 = "abe";
	char *number_3 = "10111";
	char *sum = NULL;
	int notation = 16;
	int exit_code = sum_numbers(&sum, 16, 3, number_1, number_2, number_3);

	if (exit_code == SUCCES) {
		sum = remove_leading_zeros(sum);
		printf("%s\n", sum);
		free(sum);
	} else {
		if (exit_code == INVALID_NUMBER) {
			fprintf(stderr, "%s\n", "Some number is written in the wrong number system");			
		}
		if (exit_code == NO_MEMORY) {
			fprintf(stderr, "%s\n", "No memory");
		}
	}

	return 0;
}

bool is_valid_number(char *number, int notation) {
	char *ptr = number;
	int value_digit = 0;

	while (*ptr) {
		value_digit = isalpha(*ptr) ? toupper(*ptr) - 'A' + 10 : *ptr - '0';
		if (value_digit >= notation) {
			return false;
		}
		ptr++;
	}

	return true;
}

int sum_notation(char **res, char *number_1, char *number_2, int notation) {
	if (!number_1) {
		*res = (char*)malloc(sizeof(char) * strlen(number_2));
		if (*res) {
			strcpy(*res, number_2);
		} else {
			return NO_MEMORY;
		}
	} else {
		if (!number_2) {
			*res = (char*)malloc(sizeof(char) * strlen(number_1));
			if (*res) {
				strcpy(*res, number_1);
			} else {
				return NO_MEMORY;
			}
		} else {
			free(*res);
			int size = max(strlen(number_1), strlen(number_2)) + 2;
			*res = (char*)malloc(sizeof(char*) * size);
			char *ptr = *res + size;
			char *ptr_1 = number_1 + strlen(number_1) - 1;
			char *ptr_2 = number_2 + strlen(number_2) - 1;
			int value_digit_1 = 0;
			int value_digit_2 = 0;
			int temp = 0;
			int remainder = 0;
			*ptr-- = 0;

			if (!*res) {
				return NO_MEMORY;
			}

			for (int i = 0; i < size; i++) {
				(*res)[i] = '0';
			}

			while (ptr_1 - number_1 >= 0 && ptr_2 - number_2 >= 0) {
				value_digit_1 = isalpha(*ptr_1) ? toupper(*ptr_1) - 'A' + 10 : *ptr_1 - '0';
				value_digit_2 = isalpha(*ptr_2) ? toupper(*ptr_2) - 'A' + 10 : *ptr_2 - '0';
				temp = (value_digit_1 + value_digit_2 + remainder) % notation;
				remainder = (value_digit_1 + value_digit_2) / notation;
				*ptr-- =  temp > 9 ? temp + 'A' - 10 : temp + '0';
				ptr_1--;
				ptr_2--;
			}

			if (ptr_1 - number_1 >= 0) {
				while (ptr_1 - number_1 >= 0) {
					value_digit_1 = isalpha(*ptr_1) ? toupper(*ptr_1) - 'A' + 10 : *ptr_1 - '0';
					temp = (value_digit_1 + remainder) % notation;
					remainder = (value_digit_1 + remainder) / notation;
					*ptr-- =  temp > 9 ? temp + 'A' - 10 : temp + '0';
					ptr_1--;
				}
			}

			if (ptr_2 - number_2 >= 0) {
				while (ptr_2 - number_2 >= 0) {
					value_digit_2 = isalpha(*ptr_2) ? toupper(*ptr_2) - 'A' + 10 : *ptr_2 - '0';
					temp = (value_digit_2 + remainder) % notation;
					remainder = (value_digit_2 + remainder) / notation;
					*ptr-- =  temp > 9 ? temp + 'A' - 10 : temp + '0';
					ptr_2--;
				}
			}

			if (remainder != 0) {
				*ptr-- = remainder + '0';
			}
		}
	}

	return SUCCES;
}

int sum_numbers(char **res, int notation, int count, ...) {
	char *number = NULL;
	char *temp = NULL;
	char *temp_res = NULL;
	int exit_code = 0;
	va_list arg;
	va_start(arg, count);

	for (int i = 0; i < count; i++) {
		number = va_arg(arg, char*);
		if (is_valid_number(number, notation)) {
			exit_code = sum_notation(res, temp, number, notation);
			if (exit_code == SUCCES) {
				if (temp) {
					free(temp);
				}
				temp = (char*)malloc(sizeof(char) * strlen(*res));
				if (!temp) {
					return NO_MEMORY;
				}
				strcpy(temp, *res);
			} else {
				return exit_code;
			}
		} else {
			return INVALID_NUMBER;
		}
	}

	va_end(arg);

	free(temp);

	return SUCCES;
}

char *remove_leading_zeros(char *res) {
	char *ptr = res;
	char *str = NULL;
	int index = 0;

	while (*ptr == '0') {
		ptr++;
		index++;
	}

	str = (char*)malloc(sizeof(char) * (strlen(res) - index));
	char *ptr_str = str;

	while (*ptr) {
		*ptr_str++ = *ptr++;
	}

	free(res);

	return str;
}