#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define SIZE_TOKEN 30

int get_max_modulo();
int check_number(char *number, int base);
bool is_number(char *string);
int get_min_base_notation(char *string);
long long notation_10(char *string, int base);
void conv_notation(int number, int base);

int main() {
	int check = get_max_modulo();

	if (check == 1) {
		fprintf(stderr, "%s\n", "Invalid base of notation");
	}
	if (check == 2) {
		fprintf(stderr, "%s\n", "Entered invalid number");
	}
	if (check == 3) {
		fprintf(stderr, "%s\n", "Entered number with wrong notation");
	}

	return 0;
}

int get_max_modulo() {
	char result[SIZE_TOKEN];
	char token[SIZE_TOKEN];
	int base = 0;

	if (fscanf(stdin, "%d", &base) != EOF && base >= 2 && base <= 36) {
		fscanf(stdin, "%s", result);
		if (strcmp(result, "Stop") != 0) {
			if (check_number(result, base) == 0) {
				if (strcmp(result, "Stop") != 0) {
					while (strcmp(token, "Stop") != 0) {
						if (fscanf(stdin, "%s", token) != EOF && strcmp(token, "Stop") != 0) {
							if (check_number(token, base) == 0) {
								if (abs(notation_10(result, base)) < abs(notation_10(token, base))) {
									for (int i = 0; i < strlen(token); i++) {
										result[i] = token[i];
									}
									result[strlen(token)] = '\0';
								}
							} else {
								return check_number(result, base);
							}
						}
					}
				}
			} else {
				return check_number(result, base);
			}
		}
	} else {
		return 1;
	}

	conv_notation(notation_10(result, base), 9);
	conv_notation(notation_10(result, base), 18);
	conv_notation(notation_10(result, base), 27);
	conv_notation(notation_10(result, base), 36);

	return 0;
}

int check_number(char *number, int base) {
	if (is_number(number)) {
		if (strcmp(number, "0") != 0){
			if (get_min_base_notation(number) <= base) {
				return 0;
			} else {
				return 3;
			}
		} else {
			return 0;
		}
	} else {
		return 2;
	}
}

bool is_number(char *string) {
	for (char *ptr = *string == '-' ? string + 1: string; *ptr; ptr++) {
		if (!(isdigit(*ptr) || isalpha(*ptr))) {
			return false;
		}
	}

	return true;
}

int get_min_base_notation(char *string) {
	int min_base = 0;
	char *ptr = NULL;

	if (*string == '-') {
		ptr = string;
	} else {
		ptr = string - 1;
	}

	while (*++ptr) {
		if (*ptr - '0' > min_base) {
			min_base = isalpha(*ptr) ? toupper(*ptr) - 'A' + 10 : *ptr - '0';
		}
	}

	return min_base + 1;
}

long long notation_10(char *string, int base) {
	int sign = 1;
	long long res = 0;
	char *ptr = NULL;
	if (*string == '-') {
		sign = -1;
		ptr = string;
	} else {
		ptr = string - 1;
	}

	while (*++ptr) {
		res = res * base + (isdigit(*ptr) ? (*ptr - '0') : (toupper(*ptr) - 'A' + 10));
	}

	return sign * res;
}

void conv_notation(int number, int base) {
	char buff[SIZE_TOKEN];
	int number_for_out = number;
	char *ptr = buff + SIZE_TOKEN - 1;
	*ptr-- = '\0';
	int temp = 0;
	char sign = number > 0 ? 0 : '-'; 
	number = number > 0 ? number : abs(number);

	while (number) {
		temp = number % base;
		*ptr-- = temp > 9 ? temp - 10 + 'A' : temp + '0';
		number /= base;
	}

	
	printf("%d - (%d) %c%s\n", number_for_out, base, sign, ptr + 1);
}