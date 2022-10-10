#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define pi 3.14159265358979323846
#define eps 0.000000000000000000001

double factorial(double n);
double exp_limit(double accuracy);
double exp_range(double accuracy);
double pi_limit(double accuracy);
double pi_range(double accuracy);
double ln2_limit(double accuracy);
double ln2_range(double accuracy);
double sqrt2_limit(double accuracy);
double sqrt2_range(double accuracy);
double for_gamma_limit(double k, double m);
double gamma_limit(double accuracy);
double gamma_range(double accuracy);
double func_pi(double x);
double func_exp(double x);
double func_ln2(double x);
double func_sqrt2(double x);
double func_gamma(double x);
double dichotomy_method(double (*func)(double), double begin, double end, double accuracy);

int main() {
	printf("Exponent over limit: %.10lf\n", exp_limit((double)0.0000001));
	printf("Exponent through series sum: %.10lf\n", exp_range((double)0.0001));
	printf("Exponent via Equation: %.10lf\n", dichotomy_method(func_exp, (double)2.0, (double)3.0, 0.00000001));
	printf("\n");
	printf("Pi over limit: %.10lf\n", pi_limit((double)0.00000001));
	printf("Pi through series sum: %.10lf\n", pi_range((double)0.0001));
	printf("Pi via Equation: %.10lf\n", dichotomy_method(func_pi, (double)3.0, (double)3.5, 0.0001));
	printf("\n");
	printf("Ln2 over limit: %.10lf\n", ln2_limit((double)0.0001));
	printf("Ln2 through series sum: %.10lf\n", ln2_range((double)0.0001));
	printf("Ln2 via Equation: %.10lf\n", dichotomy_method(func_ln2, (double)0.0, (double)3.5, 0.0001));
	printf("\n");
	printf("Sqrt2 over limit: %.10lf\n", sqrt2_limit((double)0.0001));
	printf("Sqrt2 through series sum: %.10lf\n", sqrt2_range((double)0.0001));
	printf("Sqrt2 via Equation: %.10lf\n", dichotomy_method(func_sqrt2, (double)0.0, (double)2.0, 0.0001));
	printf("\n");
	printf("Gamma over limit: %.10lf\n", gamma_limit((double)0.0001));
	printf("Gamma through series sum: %.10lf\n", gamma_range((double)0.00000000001));

	return 0;
}

double factorial(double n) {
	return (n < 2) ? 1 : n * factorial(n - 1);
}

double exp_limit(double accuracy) {
	double prev = 0.0;
	double now = 2.0;
	int i = 2;

	do {
		prev = now;
		now = pow(1 + 1.0 / i, i); 
		i++;
	} while (fabs(now - prev) > accuracy);

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
	} while (fabs(now - prev) > accuracy);

	return now;
}

double pi_limit(double accuracy) {
	double prev = 0.0;
	double now = 4.0;
	int i = 1;

	do {
		prev = now;
		now *= (4 * i * (i + 1)) / pow(2 * i + 1, 2); //если выразить множитель членов последовательности
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

double ln2_limit(double accuracy) {
	double prev = 0.0;
	double now = 1.0;
	int i = 2;

	do {
		prev = now;
		now = i * (pow(2, 1.0 / i) - 1.0);
		i++;
	} while (fabs(now - prev) > accuracy);

	return now;
}

double ln2_range(double accuracy) {
	double prev = 0.0;
	double now = 1.0;
	int i = 2;

	do {
		prev = now;
		now += ((i % 2 == 0) ? (-1.0) : (1.0)) / i;
		i++;
	} while (fabs(now - prev) > accuracy);

	return now;
}

double sqrt2_limit(double accuracy) {
	double prev = 0.0;
	double now = -0.5;

	do {
		prev = now;
		now = now - pow(now, 2) / 2 + 1.0;
	} while (fabs(now - prev) > accuracy);

	return now;
}

double sqrt2_range(double accuracy) {
	double prev = 0.0;
	double now = pow(2, pow(2, -2));
	double k = 3;

	do {
		prev = now;
		now *= pow(2, pow(2, -k));
		k++;
	} while (fabs(now - prev) > accuracy);

	return now;
}

double for_gamma_limit(double k, double m) {
	return factorial(m) / (factorial(k) * factorial(m - k));
}

double gamma_limit(double accuracy) {
	double prev = 0.0;
	double now = 0.0;
	double m = 2;

	do {
		prev = now;
		now = 0.0;
		for (int k = 1; k <= m; k++) {
			now += (for_gamma_limit(k, m) * ((k % 2 == 0) ? (1) : (-1)) * log(factorial(k))) / k;
		}
		m++;
	} while (fabs(now - prev) > accuracy);

	return now;
}

double gamma_range(double accuracy) {
	double prev = 0.0;
	double now = 0.5;
	double sum = 0.0;
	int k = 3;

	do {
		prev = now;
		sum = ((1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k));
		now += sum;
		if (sum < eps) {
			prev = 0.0;	
		}
		k++;
	} while (fabs(now - prev) > accuracy);

	return -pow(pi, 2) / 6.0 + now;
}

double func_exp(double x) {
	return log(x) - 1.0;
}

double func_pi(double x) {
	return -sin(x);
}

double func_ln2(double x) {
	return exp(x) - 2.0;
}

double func_sqrt2(double x) {
	return x * x - 2.0;
}

double dichotomy_method(double (*func)(double), double left, double right, double accuracy) {
	double result = 0.0;
	
	do {
		result = (right + left) / 2;

		if (func(result) * func(right) < accuracy) {
			left = result;
		} else {
			right = result;
		}
	} while (fabs(right - left) > accuracy);

	return result;
}