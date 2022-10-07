#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define eps 0.00000000000001

typedef struct tag_ans {
	double number;
	bool have_repres;
} num_repres;

bool is_prime(int number);
int *get_prime_dividers(int number, int *curr_size);
bool finite_representation(int number, int notation);
double get_number_after_dot(double number);
num_repres *have_numbers_repres(int notation, int count, ...);

int main() {
	float test = 0.0;
	printf("%lf\n", get_number_after_dot(0.12343231312));

	return 0;
}

bool is_prime(int number) {
	number = abs(number);

	if (number == 0 || number == 1) {
		return false;
	}

	for (int i = 2; i <= floor(sqrt(number)); i++) {
		if (number % i == 0) {
			return false;
		}
	}

	return true;
}

int *get_prime_dividers(int number, int *curr_size) {
	int *dividers = NULL;
	int temp = number;
	int index = 0;
	int size = 0;

	for (int i = 2; i <= number / 2; i++) {
		if (is_prime(i) && number % i == 0) {
			if (!size || index == size) {
				size = !size ? 1 : size * 2;
				dividers = (int*)realloc(dividers, sizeof(int) * size);
			}
			dividers[index++] = i;
		}
	}

	*curr_size = index;

	return dividers;
}

bool finite_representation(int number, int notation) {
	int size_div_number = 0;
	int size_div_notation = 0;
	int *dividers_number = get_prime_dividers(number, &size_div_number);
	int *dividers_notation = get_prime_dividers(notation, &size_div_notation);

	if (size_div_notation != size_div_number || (size_div_notation == 0 && size_div_number == 0)) {
		free(dividers_number);
		free(dividers_notation);
		
		return false;
	} else {
		for (int i = 0; i < size_div_number; i++) {
			if (dividers_number[i] != dividers_notation[i]) {
				free(dividers_number);
				free(dividers_notation);
				
				return false;
			}
		}
	}

	free(dividers_number);
	free(dividers_notation);
	
	return true;
}

double get_number_after_dot(double number) {
	while (fmod(number, 1) >= eps) {
		number *= 10.0;
	}

	return number;
}

num_repres *have_numbers_repres(int notation, int count, ...) {
	num_repres *results = (num_repres*)malloc(sizeof(num_repres) * count);
	va_list args;
	va_start(args, count);
	char *number = 0;
}