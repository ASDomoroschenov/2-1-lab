#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "task1.h"

int main() {
	char *res = NULL;
	int degree = 0;
	int number = 0;

	printf("Enter the number: ");
	if (scanf_int(&number) == SUCCESS) {
		printf("Enter the degree of 2: ");
		if (scanf_int(&degree) == SUCCESS) {
			int exit_code = notation(&res, degree, number);

			if (exit_code == SUCCESS) {
				printf("%d(%d) = %s\n", number, 1 << degree, res);
			} else {
				if (exit_code == INVALID_NOTATION) {
					fprintf(stderr, "%s\n", "INVALID_NOTATION: Wrong number system entered");
				}
				if (exit_code == NO_MEMORY) {
					fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
				}
			}

			if (res) {
				free(res);
			}
		} else {
			fprintf(stderr, "%s\n", "NOT_INTEGER: The degree is not integer");
		}
	} else {
		fprintf(stderr, "%s\n", "NOT_INTEGER: The number is not integer");
	}

	return 0;
}

int scanf_int(int *number) {
	char symb = 0;

	if (scanf("%d%c", number, &symb) == 2 && isspace(symb)) {
		return SUCCESS;
	}

	return NOT_INTEGER;
}

int notation(char **res, int degree, int number) {
	if (degree >= 1 && degree <= 5) {
		int remainder = 0;
		int length = 1;
		int exit_code = 0;
		char *reverse_str = (char*)malloc(sizeof(char));
		char *ptr = reverse_str;

		if (!reverse_str) {
			return NO_MEMORY;
		}

		while (number) {
			remainder = get_remainder(number, 1 << degree);

			if (ptr - reverse_str == length - 1) {
				length = sum(length, length);
				reverse_str = (char*)realloc(reverse_str, sizeof(char) * length);
				
				if (!reverse_str) {
					return NO_MEMORY;
				}
			}

			*ptr++ = remainder > 9 ? subtraction(sum(remainder, 'A'), 10) : sum(remainder, '0');
			number >>= degree;
		}

		*ptr = 0;

		exit_code = reverse(res, reverse_str);
		free(reverse_str);

		return exit_code;
	}

	return INVALID_NOTATION;
}

int get_remainder(int number, int divider) {
	return number & (divider - 1);
}

int sum(int num_1, int num_2) {
    int sum = 0;
    int carry = 0;

    sum = num_1 ^ num_2;
    carry = (num_1 & num_2) << 1;
    
    while (carry) {
        int tmp = sum;
        sum = sum ^ carry;
        carry = (tmp & carry) << 1;
    }

    return sum;
}

int subtraction(int num_1, int num_2) {
	return sum(num_1, sum(~num_2, 1));
}

int reverse(char **res, char *str) {
	*res = (char*)malloc(sizeof(char) * (strlen(str) + 1));

	if (!*res) {
		return NO_MEMORY;
	}

	char *ptr_res = *res;
	char *ptr_str = str + strlen(str) - 1;

	while (ptr_str - str >= 0) {
		*ptr_res++ = *ptr_str--;
	}

	*ptr_res = 0;

	return SUCCESS;
}