#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double my_pow(double b, double n);
double exp_limit(int l);
int factorial (int n);
double exp_series(int l);
double function(double x);
double exp_equation(double (*func)(double), double begin, double end, int l);

int main() {
	printf("limit : %.10lf\n", exp_limit(7));
	printf("series : %.10lf\n", exp_series(7));
	printf("equation : %.10lf\n", exp_equation(function, (double)2.0, (double)3.0, 7));
	printf("original : %.10lf\n", exp(1));

	return 0;
}

double exp_limit(int l) {
	double accuracy = pow(10, -l);
	double prev = 0.0;
	double now = 0.0;
	int i = 1;

	do {
		prev = pow(1.0 + 1.0 / i, i);
		now = pow(1.0 + 1.0 / (i + 1), i + 1);
		printf("%.10lf\n", now - prev);
		i++;
	} while (fabs(now - prev) >= accuracy);

	return prev;
}

int factorial (int n) {
  return (n < 2) ? 1 : n * factorial(n - 1);
}

double exp_series(int l) {
	int i = 0;
	double accuracy = pow(10, -l);
	double sum = 0.0;

	while(1.0 / factorial(i) > accuracy) {
		sum += 1.0 / factorial(i++);
	}

	return sum;
}

double function(double x) {
	return log(x) - 1;
}

double exp_equation(double (*func)(double), double begin, double end, int l) {
	double result = 0.0;
	double accuracy = pow(10, -l);

	do {
		result = (begin + end) / 2;
		if (func(result) * func(begin) < 0 || fabs(func(result) * func(begin)) < accuracy) {
			end = result;
		} else {
			begin = result;
		}
	} while(fabs(begin - end) >= accuracy);

	return result;
}

double binpow (double a, double n) {
	if (n == 0)
		return 1.0;
	if (n % 2 == 1)
		return binpow(a, n - 1) * a;
	else {
		double b = binpow(a, n / 2);
		return b * b;
	}
}