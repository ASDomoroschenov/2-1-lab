#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>

#define eps 0.00000000001

typedef struct {
	unsigned long long numerator;
	unsigned long long denominator;
} Fraction;

typedef struct {
	char *fraction;
	int status;
} Fraction_res;

enum ERRORS {
	NO_MEMORY = -1,
	INVALID_DOUBLE = -2,
	INVALID_NOTATION = -3,
	SUCCESS = -4
};

int valid_double(char*);
int valid_notation(int);
int get_fraction_from_str(Fraction**, char*);
Fraction *fraction_reduction(Fraction*);
int has_representation(int, char*);
int representation_fractions(Fraction_res **, int, int, ...);
void output_res(Fraction_res*, int);

int main() {
	Fraction_res *res = NULL;
	int exit_code = representation_fractions(&res, 5, 3, "0.1233333334", "0.125", "0.124");
		
	Fraction *test;
	get_fraction_from_str(&test, "0.0067");
	test = fraction_reduction(test);
	printf("%ld %ld\n", test->numerator, test->denominator);

	if (exit_code == SUCCESS) {
		output_res(res, 3);
	} else {
		if (exit_code == NO_MEMORY) {
			fprintf(stderr, "%s\n", "NO MEMORY: The system is out of memory");
		}
		if (exit_code == INVALID_DOUBLE) {
			fprintf(stderr, "%s\n", "INVALID NUMBER: All numbers must be between 0 and 1");
		}
		if (exit_code == INVALID_NOTATION) {
			fprintf(stderr, "%s\n", "INVALID NOTATION: Notation must be between 2 and 36");
		}
	}

	free(res);

	return 0;
}

int valid_double(char *number) {
	char *ptr = number;
	
	if (isdigit(*ptr) && *ptr == '0') {
		if (*++ptr == '.') {
			while (*++ptr) {
				if (!isdigit(*ptr)) {
					return 0;
				}
			}

			return 1;
		}
	}

	return 0;
}

int valid_notation(int notation) {
	return notation >= 2 && notation <= 36;
}

int get_fraction_from_dbl(Fraction **fraction, char *number) {
	if (valid_double(number)) {
		unsigned long long numerator = 0;
		unsigned long long denominator = 1;
		char *ptr = number + 1;

		*fraction = (Fraction*)malloc(sizeof(Fraction));

		if (*fraction) {
			while (*++ptr) {
				numerator = numerator * 10 + (*ptr - '0');
				denominator *= 10;
			}
 		} else {
			return NO_MEMORY;
		}

		(*fraction)->numerator = numerator;
		(*fraction)->denominator = denominator;

		return SUCCESS;
	}

	return INVALID_DOUBLE;
}

Fraction *fraction_reduction(Fraction *fraction) {
	int div = 2;

	while (div <= (int)ceil(sqrt(fraction->numerator)) && div <= fraction->numerator) {
		while (fraction->numerator % div == 0 && fraction->denominator % div == 0) {
			fraction->numerator /= div;
			fraction->denominator /= div;
		}
		div++;
	}

	return fraction;
}

int has_representation(int notation, char *number) {
	Fraction *fraction = NULL;
	int exit_code = get_fraction_from_dbl(&fraction, number);
	
	if (exit_code == SUCCESS) {
		fraction = fraction_reduction(fraction);
		int denominator = fraction->denominator;

		while (denominator > 1) {
			if (denominator % notation != 0) {
				free(fraction);
				return 0;
			}
			denominator /= notation;
		}

		free(fraction);

		return 1;
	}

	return exit_code;
}

int representation_fractions(Fraction_res **res, int notation, int count, ...) {
	if (valid_notation(notation)) {
		*res = (Fraction_res*)malloc(sizeof(Fraction_res) * count);
		char *number;
		int exit_code = 0;
		va_list args;
		va_start(args, count);

		for (int i = 0; i < count; i++) {
			number = va_arg(args, char*);
			(*res)[i].fraction = number;
			exit_code = has_representation(notation, number);
			if (exit_code == 0 || exit_code == 1) {
				(*res)[i].status = exit_code;
			} else {
				return exit_code;
			}
		}
		
		return SUCCESS;
	}

	return INVALID_NOTATION;
}

void output_res(Fraction_res *res, int count) {
	char *res_status[] = {"NO", "YES"};
	
	for (int i = 0; i < count; i++) {
		printf("%s - %s\n", res[i].fraction, res_status[res[i].status]);
	}
}