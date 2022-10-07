#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.1415926535897

long long factorial (int n);
double exp_limit(double accuracy);
double exp_range(double accuracy);
double func_exp(double x);
double pi_limit(double accuracy);
double pi_range(double accuracy);
double func_pi(double x);
double dichotomy_method(double (*func)(double), double begin, double end, double accuracy);


int main() {
	printf("%.20lf\n", dichotomy_method(func_pi, (double)3.0, (double)3.5, 0.0001));

	return 0;
}

long long factorial(int n) {
  return (n < 2) ? 1 : n * factorial (n - 1);
}

double exp_limit(double accuracy) {
	double prev = 0.0;
	double now = 2.0;
	int i = 2;

	do {
		prev = now;
		now = pow(1 + 1.0 / i, i); 
		i++;
	} while (abs(now - prev) > accuracy);

	return now;
}

double exp_range(double accuracy) {
	double now = 1.0;
	double prev = 0.0;
	int i = 1;

	do {
		prev = now;
		now += 1.0 / factorial(i);
		i++; 
	} while (now - prev > accuracy);

	return now;
}

double func_exp(double x) {
	return log(x) - 1.0;
}

double pi_limit(double accuracy) {
	double prev = 0.0;
	double now = 4.0;
	int i = 2;

	do {
		prev = now;
		now = pow(pow(2, i) * factorial(i), 4) / (i * pow(factorial(2 * i), 2)); 
		i++;
	} while (fabs(now - prev) > accuracy);

	return now;
}

double pi_range(double accuracy) {
	double prev = 0.0;
	double now = 1.0;
	int i = 2;

	do {
		prev = now;
		now += ((i % 2 == 0) ? (-1.0) : (1.0)) / (2 * i - 1);
		i++; 
	} while (fabs(now - prev) > accuracy);

	return 4.0 * now;
}

double func_pi(double x) {
	return cos(pi * x) + 1.0;
}

double dichotomy_method(double (*func)(double), double begin, double end, double accuracy) {
	double result = 0.0;
	
	do {
		result = (begin + end) / 2;

		if (fabs(func(result)) < accuracy) {
			return result;
		} else {
			if (func(result) * func(begin) < accuracy) {
				end = result;
			} else {
				begin = result;
			}
		}
	} while (fabs(begin - end) > accuracy);

	return result;
}