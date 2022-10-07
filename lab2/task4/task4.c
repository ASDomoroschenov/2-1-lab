#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

double geometric_mean(int count, ...);
double my_pow(double number, int step);

int main() {
	printf("Geometric mean 2 4 8: %lf\n", geometric_mean(3, (double)2, (double)4, (double)8));
	printf("My pow 2^(-4): %lf\n", my_pow(2, -4));
	printf("My pow 2^5: %lf\n", my_pow(2, 5));

	return 0;
}

double geometric_mean(int count, ...) {
	double multiplication = 1.0;
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		multiplication *= va_arg(args, double);
	}
	
	va_end(args);

	return pow(multiplication, 1.0 / count);
}

double my_pow(double number, int step) {
	if (step == 0) {
		return 1;
	}

	return step > 0 ? number * my_pow(number, step - 1) : 1.0 / (number * my_pow(number, abs(step) - 1));
}