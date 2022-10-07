#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

bool check_args(int argc, char const *argv[]);
bool is_str_digit(const char *str);
bool is_str_integer(const char *str);
double my_atoi(const char *string);
void get_answer(int argc, char const *argv[]);
void quadratic_equation(char const *argv[]);
void multiplicity(char const *argv[]);
void is_sides_of_triangle(char const *argv[]);

int main(int argc, char const *argv[]) {
	if (check_args(argc, argv)) {
		get_answer(argc, argv);
	} else {
		fprintf(stderr, "%s\n", "Invalid arguments");
	}

	return 0;
}

bool check_args(int argc, char const *argv[]) {
	const char flags[] = "qmt";
	const char begin_flag[] = "-/";

	if (argc == 5) {
		if (strchr(begin_flag, argv[1][0]) && strchr(flags, argv[1][1]) && argv[1][1] == 'q') {
			if (is_str_digit(argv[2]) && is_str_digit(argv[3]) && is_str_digit(argv[3])) {
				return true;
			}
		} else {
			if (strchr(begin_flag, argv[1][0]) && strchr(flags, argv[1][1]) && argv[1][1] == 't') {
				if (is_str_digit(argv[2]) && is_str_digit(argv[3]) && is_str_digit(argv[3]) &&
					my_atoi(argv[2]) != 0.0 && my_atoi(argv[3]) != 0.0 && my_atoi(argv[4]) != 0.0) {
					return true;
				}
			}
		}
	} else {
		if (argc == 4) {
			if (strchr(begin_flag, argv[1][0]) && strchr(flags, argv[1][1]) && argv[1][1] == 'm') {
				if (is_str_integer(argv[2]) && is_str_integer(argv[3]) &&
					my_atoi(argv[2]) != 0.0 && my_atoi(argv[3]) != 0.0) {
					return true;
				}
			}
		}
	}

	return false;
}

bool is_str_integer(const char *str) {
	if ((*str == '-' || *str == '+') && (strlen(str) > 1)) {
		str++;
	}

	while (*str != '\0') {
		if (!isdigit(*str)) {
			return false;
		}
		str++;
	}

	return true;
}

bool is_str_digit(const char *str) {
	if ((*str == '-' || *str == '+') && (strlen(str) > 1)) {
		str++;
	}

	while (*str != '\0') {
		if (*str != '.' && !isdigit(*str)) {
			return false;
		}
		str++;
	}

	return true;
}

double my_atoi(const char *string) {
	int result = 0;
	double result_after_point = 0.0;
	int sign = 1;
	int step = 0;
	bool before_point = true;

	if (*string == '-') {
		sign = -1;
		*string++;
	}

	while (*string != '\0') {
		if (*string == '.') {
			before_point = false;
			string++;
		} else {
			result = result * 10 + (*string - '0');
			string++;

			if (!before_point) {
				step++;
			}
		}
	}

	return sign * (result / pow(10, step));
}

void get_answer(int argc, char const *argv[]) {
	if (argv[1][1] == 'q') {
		quadratic_equation(argv);
	}
	if (argv[1][1] == 'm') {
		multiplicity(argv);
	}
	if (argv[1][1] == 't') {
		is_sides_of_triangle(argv);
	}
}

void quadratic_equation(char const *argv[]) {
	double a = my_atoi(argv[2]);
	double b = my_atoi(argv[3]);
	double c = my_atoi(argv[4]);
	double D = sqrt(b * b - 4 * a * c);
	printf("a b c:\n");
	if (D >= 0.0) {
		double x1 = (-b + D) / (2 * a);
		double x2 = (-b - D) / (2 * a);
		printf("x1 = %lf\nx2 = %lf\n\n", x1, x2);
	} else {
		printf("No roots\n\n");
	}

	a = my_atoi(argv[2]);
	b = my_atoi(argv[4]);
	c = my_atoi(argv[3]);
	D = sqrt(b * b - 4 * a * c);
	printf("a c b:\n");
	if (D >= 0.0) {
		double x1 = (-b + D) / (2 * a);
		double x2 = (-b - D) / (2 * a);
		printf("x1 = %lf\nx2 = %lf\n\n", x1, x2);
	} else {
		printf("No roots\n\n");
	}

	a = my_atoi(argv[3]);
	b = my_atoi(argv[2]);
	c = my_atoi(argv[4]);
	D = sqrt(b * b - 4 * a * c);
	printf("b a c:\n");
	if (D >= 0.0) {
		double x1 = (-b + D) / (2 * a);
		double x2 = (-b - D) / (2 * a);
		printf("x1 = %lf\nx2 = %lf\n\n", x1, x2);
	} else {
		printf("No roots\n\n");
	}

	a = my_atoi(argv[3]);
	b = my_atoi(argv[4]);
	c = my_atoi(argv[2]);
	D = sqrt(b * b - 4 * a * c);
	printf("b c a:\n");
	if (D >= 0.0) {
		double x1 = (-b + D) / (2 * a);
		double x2 = (-b - D) / (2 * a);
		printf("x1 = %lf\nx2 = %lf\n\n", x1, x2);
	} else {
		printf("No roots\n\n");
	}

	a = my_atoi(argv[4]);
	b = my_atoi(argv[2]);
	c = my_atoi(argv[3]);
	D = sqrt(b * b - 4 * a * c);
	printf("c a b:\n");
	if (D >= 0.0) {
		double x1 = (-b + D) / (2 * a);
		double x2 = (-b - D) / (2 * a);
		printf("x1 = %lf\nx2 = %lf\n\n", x1, x2);
	} else {
		printf("No roots\n\n");
	}

	a = my_atoi(argv[4]);
	b = my_atoi(argv[3]);
	c = my_atoi(argv[2]);
	D = sqrt(b * b - 4 * a * c);
	printf("c b a:\n");
	if (D >= 0.0) {
		double x1 = (-b + D) / (2 * a);
		double x2 = (-b - D) / (2 * a);
		printf("x1 = %lf\nx2 = %lf\n\n", x1, x2);
	} else {
		printf("No roots\n\n");
	}
}

void multiplicity(char const *argv[]) {
	int first_digit = (int)my_atoi(argv[2]);
	int second_digit = (int)my_atoi(argv[3]);

	if (first_digit % second_digit == 0) {
		printf("YES\n");
	} else {
		printf("NO\n");
	}
}

void is_sides_of_triangle(char const *argv[]) {
	double side_a = my_atoi(argv[2]);
	double side_b = my_atoi(argv[3]);
	double side_c = my_atoi(argv[4]);

	if (side_a + side_b > side_c &&
		side_a + side_c > side_b &&
		side_b + side_c > side_a) {
		printf("YES\n");
	} else {
		printf("NO\n");
	}
}