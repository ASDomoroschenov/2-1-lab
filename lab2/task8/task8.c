#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double function(double x);
double answer(double (*func)(double), double begin, double end, int accuracy);

int main() {
	printf("%.10lf\n", answer(function, (double)1.57, (double)3.14, 3));
	return 0;
}

double function(double x) {
	return x * x - 5 * sin(x);
}

double answer(double (*func)(double), double begin, double end, int accuracy) {
	double result = 0.0;
	double eps = pow(10, -accuracy);
	
	do {
		result = (begin + end) / 2;
		if (func(result) * func(begin) < eps) {
			end = result;
		} else {
			begin = result;
		}
	} while(fabs(begin - end) >= eps);

	return result;
}