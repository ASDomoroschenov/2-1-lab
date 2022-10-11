#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define eps 0.0000000001

int have_div_5(double number);
int have_div_2(double number);
char *double_to_str(double number);

int main() {
	char *str = double_to_str((double)0.125);
	char *ptr = str - 1;

	while (*++ptr) {
		printf("%c", *ptr);
	}
	printf("\n");

	return 0;
}

char *double_to_str(double number) {
	int size = 1;
	char *str = (char*)malloc(sizeof(char));
	char *ptr = str;

	while (number > eps) {
		number *= 10;
		if (ptr - str == size) {
			size *= 2;
			str = (char*)realloc(str, sizeof(char) * size);
		}
		*ptr++ = (int)number + '0';
		number -= (long long)number;
		printf("%.100lf\n", number);
	}

	if (str - ptr == size) {
		size *= 2;
		str = (char*)realloc(str, sizeof(char) * size);
		*ptr = 0;
	}

	return str;
}