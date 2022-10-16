#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum ERRORS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	NOT_INTEGER = -3
};

int scanf_int(int*);
int count_units(int, int);
int count_consecutive_units(int, int);
void have_l_units(int*, int, int, int**, int*);
void have_l_consecutive_units(int*, int, int, int**, int*);
void output_res(int*, int);

int main() {
	int *res_units = NULL;
	int *res_consecutive_units = NULL;
	int length_res_units = 0;
	int length_res_consecutive_units = 0;
	int bits = 0;
	int units = 0;
	int exit_code = 0;

	printf("Put in count of bits: ");
	
	if (scanf_int(&bits) == SUCCESS && bits >= 0) {
		printf("Put in count of units: ");
		if (scanf_int(&units) == SUCCESS && units >= 0) {
			if (units <= bits) {
				have_l_units(&exit_code, units, bits, &res_units, &length_res_units);
				if (exit_code == SUCCESS) {
					printf("Numbers having exactly the entered number of units: ");
					output_res(res_units, length_res_units);
					free(res_units);
				} else {
					fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
				}
				
				have_l_consecutive_units(&exit_code, units, bits, &res_consecutive_units, &length_res_consecutive_units);
				if (exit_code == SUCCESS) {
					printf("Numbers having exactly entered consecutive number of units: ");
					output_res(res_consecutive_units, length_res_consecutive_units);
					free(res_consecutive_units);
				} else {
					fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
				}
			} else {
				fprintf(stderr, "%s\n", "INVALID_COUNT_UNITS: The number of ones cannot exceed the number of bits");
			}
		} else {
			fprintf(stderr, "%s\n", "INVALID_NUMBER: Invalid number of units entered");	
		}
	} else {
		fprintf(stderr, "%s\n", "INVALID_NUMBER: Invalid number of bits entered");
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

int count_units(int number, int units) {
	int count = 0;

	while (number) {
		count += number & 1;
		number >>= 1;
	}

	return count == units;
}

int count_consecutive_units(int number, int units) {
	int count = 0;

	if (number == 0 && units == 0) {
		return 1;	
	}
	
	while (number) {
		count = number & 1 ? count + 1 : 0;

		if (count == units) {
			return 1;
		}
		
		number >>= 1;
	}

	return 0;
}

void have_l_units(int *exit_code, int units, int bits, int **res, int *length) {
	int size = 0;
	*exit_code = 0;

	for (int number = 0; number <= pow(2, bits) - 1; number++) {
		if (count_units(number, units)) {
			if (*length == size) {
				size = size ? size * 2 : 1;
				*res = (int*)realloc(*res, sizeof(int) * size);
				if (!*res) {
					*exit_code = NO_MEMORY;
					return;
				}
			}

			(*res)[(*length)++] = number;
		}
	}

	*exit_code = SUCCESS;
}

void have_l_consecutive_units(int *exit_code, int units, int bits, int **res, int *length) {
	int size = 0;
	*exit_code = 0;

	for (int number = 0; number <= pow(2, bits) - 1; number++) {
		if (count_consecutive_units(number, units)) {
			if (*length == size) {
				size = size ? size * 2 : 1;
				*res = (int*)realloc(*res, sizeof(int) * size);
				if (!*res) {
					*exit_code = NO_MEMORY;
					return;
				}
			}

			(*res)[(*length)++] = number;
		}
	}

	*exit_code = SUCCESS;
}

void output_res(int *res, int length) {
	for (int i = 0; i < length; i++) {
		printf("%d ", res[i]);
	}
	printf("\n");
}