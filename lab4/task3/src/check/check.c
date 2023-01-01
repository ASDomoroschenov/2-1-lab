#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../string/string.h"
#include "check.h"

typedef int (*check_func)(char*);

int is_operator(char symb) {
	if (symb == '+' ||
		symb == '&' ||
		symb == '-' ||
		symb == '<' ||
		symb == '~' ||
		symb == '?' ||
		symb == '!' ||
		symb == '\\') {
		return 1;
	}

	return 0;
}

int is_unary(char c) {
	return c == '\\';
}

int check_binary(char *str) {
	if (!isalpha(*str)) {
		return 0;
	}

	if (!strstr(str, ":=")) {
		return 0;
	}

	char *ptr = str + 1;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!(*ptr == ':' && *(ptr + 1) == '=')) {
		return 0;
	}

	ptr += 2;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!isalpha(*ptr)) {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != '+' &&
		*ptr != '&' &&
		*ptr != '-' &&
		*ptr != '<' &&
		*ptr != '~' &&
		*ptr != '?') {
		return 0;
	}

	if ((*ptr == '-' && *(ptr + 1) == '>') ||
		(*ptr == '+' && *(ptr + 1) == '>') ||
		(*ptr == '<' && *(ptr + 1) == '-') ||
		(*ptr == '<' && *(ptr + 1) == '>')) {
		ptr += 2;
	} else {
		ptr++;
	}

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!isalpha(*ptr)) {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	return !(*ptr);
}

int check_unary(char *str) {
	if (!isalpha(*str)) {
		return 0;
	}

	if (!strstr(str, ":=")) {
		return 0;
	}

	char *ptr = str + 1;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!(*ptr == ':' && *(ptr + 1) == '=')) {
		return 0;
	}

	ptr += 2;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != '!' && *ptr != '\\') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!isalpha(*ptr)) {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	return !(*ptr);
}

int check_read(char *str) {
	if (strstr(str, "read") != str) {
		return 0;
	}

	char *ptr = str + 4;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != '(') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!isalpha(*ptr)) {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != ',') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	while (!isspace(*ptr) && *ptr != ')') {
		if (!isdigit(*ptr)) {
			return 0;
		}

		ptr++;
	}

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != ')') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	return !(*ptr);
}

int check_write(char *str) {
	if (strstr(str, "write") != str) {
		return 0;
	}

	char *ptr = str + 5;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != '(') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (!isalpha(*ptr)) {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != ',') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	while (!isspace(*ptr) && *ptr != ')') {
		if (!isdigit(*ptr)) {
			return 0;
		}

		ptr++;
	}

	while (isspace(*ptr)) {
		ptr++;
	}

	if (*ptr != ')') {
		return 0;
	}

	ptr++;

	while (isspace(*ptr)) {
		ptr++;
	}

	return !(*ptr);
}

int check_expression(char *str) {
	int exit_code = 0;

	if ((exit_code = strip(&str)) != SUCCESS) {
		return exit_code;
	}

	check_func funcs[4] = {
		check_binary,
		check_unary,
		check_read,
		check_write
	};

	for (int i = 0; i < 4; i++) {
		if (funcs[i](str)) {
			return SUCCESS;
		}
	}

	return INVALID_EXPRESSION;
}

int is_binary_exp(char *expression) {
	char *ptr = expression - 1;

	while (*++ptr) {
		if (is_operator(*ptr)) {
			return is_unary(*ptr) ? 0 : 1;
		}
	}

	return INVALID_EXPRESSION;
}