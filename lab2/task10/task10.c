#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

//Чтобы перевести дробную часть в СС, нужно умножать дробную часть на СС
//и отбрасывать из результата целую часть, до тех пор, пока в дробной части
//результата не будет 0(из этого и следует алгоритм)

//Под числом понимается часть числа после точки

//Под делителями имеется в виду простые делители

//Если СС имеет делитель 5, то чтобы число перевелось
//нужно, чтобы число само имело делители только 2

//Если СС имеет делитель 2, то чтобы число переволось
//нужно, чтобы число само имело делители только 5

//Если СС имеет делители 5 и 2, то любое число переведется

#define eps 0.000000000000000000001

enum ERRORS {
	NO_MEMORY = -1,
	SUCCESS = -2,
	INVALID_NOTATION = -3,
	INVALID_NUMBER = -4
};

int is_prime(unsigned long long number);
int get_dividers(unsigned long long number, int **dividers, int *length);
unsigned long long fractional_part_to_int(double number);
int have_div_5(int *dividers, int length);
int have_div_2(int *dividers, int length);
int have_performance(double number, int notation);
int representation_of_fractions(int notation, int count, ...);

int main() {
	representation_of_fractions(4, 3, (double)0.5, (double)0.15, (double)0.2);	

	return 0;
}

int is_prime(unsigned long long number) {
	number = abs(number);
	
	if (number == 1) {
		return 0;
	}

	for (unsigned long long i = 2; i <= sqrt(number); i++) {
		if (number % i == 0) {
			return 0;
		}
	}

	return 1;
}

int get_dividers(unsigned long long number, int **dividers, int *length) {
	int size = 0;
	int index = 0;

	if (*dividers) {
		free(*dividers);
	}

	for (unsigned long long i = 2; i <= number; i ++) {
		if (number % i == 0 && is_prime(i)) {
			if (index == size) {
				size = ((size != 0) ? (size * 2) : 1);
				*dividers = (int*)realloc(*dividers, sizeof(int) * size);

				if (!*dividers) {
					return NO_MEMORY;
				}
			}
			(*dividers)[index++] = i;
		}
	}

	*length = index;

	return SUCCESS;
}

unsigned long long fractional_part_to_int(double number) {
	while (fabs(number - (long long)number) > eps) {
		number *= 10;
	}

	return (unsigned long long)number;
}

int have_div_5(int *dividers, int length) {
	for (int i = 0; i < length; i++) {
		if (dividers[i] == 5) {
			return 1;
		}
	}

	return 0;
}

int have_div_2(int *dividers, int length) {
	for (int i = 0; i < length; i++) {
		if (dividers[i] == 2) {
			return 1;
		}
	}

	return 0;
}

int have_performance(double number, int notation) {
	unsigned long long fractional = fractional_part_to_int(number);
	int length_num = 0;
	int length_not = 0;
	int *dividers_num = NULL;
	int *dividers_not = NULL;

	if (get_dividers(fractional, &dividers_num, &length_num) == SUCCESS &&
		get_dividers((unsigned long long)notation, &dividers_not, &length_not) == SUCCESS) {
		if (have_div_5(dividers_not, length_not) && have_div_2(dividers_not, length_not)) {
			free(dividers_num);
			free(dividers_not);

			return 1;
		}

		if (have_div_5(dividers_not, length_not) && length_num == 1 && dividers_num[0] == 2) {
			free(dividers_num);
			free(dividers_not);

			return 1;
		}

		if (have_div_2(dividers_not, length_not) && length_num == 1 && dividers_num[0] == 5) {
			free(dividers_num);
			free(dividers_not);

			return 1;
		}

		if (dividers_num) {
			free(dividers_num);
		}

		if (dividers_not) {
			free(dividers_not);
		}

		return 0;
	}

	if (dividers_num) {
		free(dividers_num);
	}

	if (dividers_not) {
		free(dividers_not);
	}

	return NO_MEMORY;
}

void output_divs(int *dividers, int length) {
	for (int i = 0; i < length; i++) {
		printf("%d ", dividers[i]);
	}
	printf("\n");
}

int representation_of_fractions(int notation, int count, ...) {
	if (notation >= 2 && notation <= 36) {
		va_list args;
		va_start(args, count);
		double number;
		int exit_code = 0;

		for (int i = 0; i < count; i++) {
			number = va_arg(args, double);

			if (number > eps && number < 1.0) {
				exit_code = have_performance(number, notation);

				if (exit_code == NO_MEMORY) {
					return NO_MEMORY;
				} else {
					if (exit_code == 1) {
						printf("%lf - YES\n", number);
					} else {
						printf("%lf - NO\n", number);
					}
				}
			} else {
				return INVALID_NUMBER;
			}
		}

		va_end(args);

		return SUCCESS;
	}

	return INVALID_NOTATION;
}