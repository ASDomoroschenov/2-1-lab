#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include "task7.h"

int main() {
	coord *coordinates = NULL;
	int exit_code_convex = get_coordinates(&coordinates, 6, (double)2.0, (double)9.0, (double)5.0, (double)7.0, (double)7.0, (double)3.0);
	double result_polynomial = get_polynomial(2, 4, (double)0, (double)1, (double)2, (double)3, (double)4);

	if (exit_code_convex == SUCCES) {
		if (is_convex(coordinates, 3)) {
			printf("The polygon is convex\n");
		} else {
			printf("The polygon is not convex\n");
		}
	} else {
		if (exit_code_convex == INVALID_SIZE) {
			fprintf(stderr, "%s\n", "Invalid count of points");
		}
		if (exit_code_convex == NO_MEMORY) {
			fprintf(stderr, "%s\n", "No memory");
		}
	}

	if (result_polynomial - (double)NO_MEMORY > 0.0001) {
		printf("Polynomial value : %lf\n", result_polynomial);
	} else {
		fprintf(stderr, "%s\n", "No memory");
	}

	free(coordinates);

	return 0;
}

int get_coordinates(coord **coordinates, int count, ...) {
	*coordinates = (coord*)malloc(sizeof(coord) * (count / 2));
	
	if (*coordinates) {
		if (count % 2 == 0 && count >= 6) {
			va_list arg;
			va_start(arg, count);

			for (int i = 0; i < count / 2; i++) {
				(*coordinates)[i].x = va_arg(arg, double);
				(*coordinates)[i].y = va_arg(arg, double);
			}

			va_end(arg);

			return SUCCES;
		}

		return INVALID_SIZE;
	}

	return NO_MEMORY;
}

bool is_convex(coord *coordinates, int size) {
	int sign = 1;
	int prev_sign = 1;
	if (size > 2) {
		for (int i = 0; i < size; i++) {
			if (i > 1 && prev_sign != sign || (sign == 0 && prev_sign == 0)) {
				return false;
			}
			// направление вектора сегмента
			double x21 = i == 0 ? coordinates[i].x - coordinates[size - 1].x : coordinates[i].x - coordinates[i - 1].x;
			double x32 = i == size - 1 ? coordinates[0].x - coordinates[i].x : coordinates[i + 1].x - coordinates[i].x;
			double y21 = i == 0 ? coordinates[i].y - coordinates[size - 1].y : coordinates[i].y - coordinates[i - 1].y;
			double y32 = i == size - 1 ? coordinates[0].y - coordinates[i].y : coordinates[i + 1].y - coordinates[i].y;
			
			prev_sign = sign;
			sign = x21 * y32 - y21 * x32 > 0.0001 ? 1 : -1;
		}

		return true;
	}

	return false;
}

double get_polynomial(double x, int step, ...) {
	va_list arg;
	va_start(arg, step);
	double *odds = (double*)malloc(sizeof(double) * (step + 1));
	
	if (odds) {
		for (int i = 0; i <= step; i++) {
			odds[i] = va_arg(arg, double); 
		}

		double res = odds[step];

		for (int i = step - 1; i >= 0; i--) {
			res = x * res + odds[i];
		}

		free(odds);
		return res;
	}

	return (double)NO_MEMORY;
}