#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "../lib/check.h"

int is_unary(char *str, int index) {
	return str[index] == '!' || str[index] == '~';
}

int is_binary(char *str, int index) {
	switch (str[index]) {
		case '&': return 1;
		case '|': return 1;
		case '-': return str[index + 1] == '>';
		case '+': return str[index + 1] == '>';
		case '<': return str[index + 1] == '>';
		case '=': return 1;
		case '?': return 1;
	}

	return 0;
}

int check_unary(char *str, int pos) {
	int len = strlen(str);
	int index = pos + 1;

	for (;index < len && (isspace(str[index]) || str[index] == str[pos]); index++);
	
	if (str[index] != '(') {
		if ((!isalpha(str[index]) ||
			str[index] == str[pos]) &&
			str[index] != '1' &&
			str[index] != '0') {
			return 0;
		}
	}

	return 1;
}

int check_binary(char *str, int pos) {
	int len = strlen(str);
	int left_index = pos - 1;
	int right_index = pos + 1;

	if (str[pos] == '-' ||
		str[pos] == '+' ||
		str[pos] == '<') {
		right_index++;
	}

	for (;left_index > 0 && isspace(str[left_index]); left_index--);
	for (;right_index < len && isspace(str[right_index]); right_index++);

	if (str[left_index] == ')' &&
		str[right_index] == '(') {
		return 1;
	}

	if (str[left_index] == ')' &&
		(isalpha(str[right_index]) ||
		 str[right_index] == '1' ||
		 str[right_index] == '0')) {
		return 1;
	}

	if (str[right_index] == '(' ||
		is_unary(str, right_index)) {
		if (!isalpha(str[left_index]) &&
			(str[left_index] != '0' ||
			str[left_index] != '1')) {
			printf("1\n");
			return 0;
		}
	} else {
		if (!((isalpha(str[left_index]) && isalpha(str[right_index])) ||
			   (isalpha(str[left_index]) && (str[right_index] == '1' || str[right_index] == '0')) ||
			   (isalpha(str[right_index]) && (str[left_index] == '1' || str[left_index] == '0')) ||
			   ((str[right_index] == '1' || str[right_index] == '0') && (str[left_index] == '1' || str[left_index] == '0')))) {
			return 0;
		}
	}

	return 1;
}

int count_operands(char *str) {
	if (str) {
		int count = 0;
		int len = strlen(str);

		for (int i = 0; i < len; i++) {
			if (isalpha(str[i]) ||
				str[i] == '1' ||
				str[i] == '0')
				count++;
		}

		return count;
	}

	return 0;
}

int count_bin_operators(char *str) {
	if (str) {
		int count = 0;
		int len = strlen(str);

		for (int i = 0; i < len; i++) {
			count += is_binary(str, i);
		}

		return count;
	}

	return 0;
}

int check_expression(char *str) {
	if (!str) {
		return 0;
	}

	int len = strlen(str);

	for (int i = 0; i < len; i++) {
		if (!isspace(str[i])) {
			if (!isalpha(str[i]) &&
				str[i] != '1' &&
				str[i] != '0') {
				if (is_unary(str, i)) {
					if (!check_unary(str, i)) {
						return 0;
					}
				} else {
					if (is_binary(str, i)) {
						if (!check_binary(str, i)) {
							return 0;
						}

						if (str[i] == '-' ||
							str[i] == '+' ||
							str[i] == '<') {
							i++;
						}
					}
				}
			}
		}
	}

	return count_bin_operators(str) == count_operands(str) - 1;
}

int check_brackets(char *str) {
	if (str) {
		char *ptr = str - 1;
		int count = 0;

		while (*++ptr) {
			if (count < 0) {
				return 0;
			}
			if (*ptr == '(') {
				count ++;
			}
			if (*ptr == ')') {
				count--;
			}
		}

		return count == 0;
	}

	return EMPTY_STRING;
}

int is_operator(char *str) {
	if (strcmp(str, "(") == 0 ||
		strcmp(str, "|") == 0 ||
		strcmp(str, "->") == 0 ||
		strcmp(str, "<>") == 0 ||
		strcmp(str, "=") == 0 ||
		strcmp(str, "?") == 0 ||
		strcmp(str, "!") == 0 ||
		strcmp(str, "+>") == 0 ||
		strcmp(str, "&") == 0 ||
		strcmp(str, "~") == 0) {
		return 1;
	}

	return 0;
}

int is_operand(char *token) {
	if (strlen(token) == 1) {
		return isalpha(token[0]) || token[0] == '1' || token[0] == '0';
	}

	return 0;
}