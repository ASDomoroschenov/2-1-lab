#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

bool check_args(int argc, char const *argv[]);
bool is_str_digit(const char *str);
int my_atoi(const char *string);
void get_answer(int digit, char flag);
void multiples(int digit);
void is_primary(int digit);
void numbers_of_digit(int digit);
void numbers_in_degree(int digit);
void sum_numbers(int digit);
int factorial(int digit);

int main(int argc, char const *argv[]) {
	if (check_args(argc, argv)) {
		get_answer(my_atoi(argv[1]), argv[2][1]);
	} else {
		fprintf(stderr, "%s\n", "Invalid arguments");
	}

	return 0;
}

bool check_args(int argc, char const *argv[]) {
	const char flags[] = "hpseaf";
	const char begin_flag[] = "-/";

	if (argc == 3) {
		if (is_str_digit(argv[1])) {
			if (strchr(begin_flag, argv[2][0]) && strchr(flags, argv[2][1]) && strlen(argv[2]) == 2) {
				return true;
			}
		}
	}

	return false;
}

bool is_str_digit(const char *str) {
	if ((*str == '-' || *str == '+') && (strlen(str) > 1)) {
		str++;
	}

	while (*str != '\0') {
		if (!isdigit(*str)) {
			return false;
		}
		str++;
	}

	return true;
}

int my_atoi(const char *string) {
	int result = 0;
	int sign = 1;

	if (*string == '-') {
		sign = -1;
		*string++;
	}

	while (*string != '\0') {
		result = result * 10 + (*string - '0');
		string++;
	}

	return sign * result;
}

void get_answer(int digit, char flag) {
	if (flag == 'h') {
		multiples(digit);
	}
	if (flag == 'p') {
		is_primary(digit);
	}
	if (flag == 's') {
		numbers_of_digit(digit);
	}
	if (flag == 'e') {
		numbers_in_degree(digit);
	}
	if (flag == 'a') {
		sum_numbers(digit);
	}
	if (flag == 'f') {
		if (digit > 0) {
			printf("Factorial: %d\n", factorial(digit));
		} else {
			printf("Invalid digit\n");
		}
	}
}

void multiples(int digit) {
	int count_multiples = 0;

	if (digit != 0) {
		for (int i = 1; i <= 100; i++) {
			if (i % digit == 0) {
				printf("%d ", i);
				count_multiples++;
			}
		}
	}

	if (count_multiples == 0) {
		printf("No such numbers\n");
	} else {
		printf("\n");
	}
}

void is_primary(int digit) {
	bool primary = true;

	if (digit > 1) {
		for (int i = 2; i <= (int)sqrt(digit); i++) {
			if (digit % i == 0) {
				primary = false;
			}
		}
	} else {
		primary = false;
	}

	if (primary) {
		printf("Digit is primary\n");
	} else {
		printf("Digit is composite\n");
	}
}

void numbers_of_digit(int digit) {
	int numbers[SIZE];
	int size = 0;
	digit = digit > 0 ? digit : abs(digit);

	if (digit != 0) {
		while (digit != 0) {
			numbers[size++] = digit % 10;
			digit /= 10;
		}

		for (int i = 0; i < size; i++) {
			printf("%d ", numbers[size - i - 1]);
		}

		printf("\n");
	} else {
		printf("0\n");
	}
}

void numbers_in_degree(int digit) {
	if (digit < 10) {
		if (digit > 0) {
			for (int i = 1; i <= 10; i++) {
				printf("%d - ", i);
				for (int j = 1; j <= digit; j++) {
					printf("%d ", (int)pow(i, j));
				}
				printf("\n");
			}
		} else {
			for (int i = 1; i <= 10; i++) {
				printf("%d - ", i);
				for (int j = 1; j >= digit; j--) {
					printf("%f ", pow(i, j));
				}
				printf("\n");
			}
		}
	} else {
		fprintf(stderr, "%s\n", "Number greater than 10 or equals 10");
	}
}

void sum_numbers(int digit) {
	int sum = 0;

	if (digit > 0) {
		for (int i = 1; i <= digit; i++) {
			sum += i;
		}
	} else {
		for (int i = 1; i >= digit; i--) {
			sum += i;
		}
	}

	printf("Sum: %d\n", sum);
}

int factorial(int digit) {
	return digit > 1 ? digit * factorial(digit - 1) : 1;
}
