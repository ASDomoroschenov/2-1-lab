#ifndef TASK3_H
#define TASK3_H

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
double for_gamma_eq(double accuracy);
double func_pi(double x);
double func_exp(double x);
double func_ln2(double x);
double func_sqrt2(double x);
double func_gamma(double x);
double dichotomy_method(double (*func)(double), double begin, double end, double accuracy);
int is_prime(int number);

#endif //TASK3_H