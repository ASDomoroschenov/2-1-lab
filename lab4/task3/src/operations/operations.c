#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../string/string.h"
#include "../check/check.h"
#include "../operations/operations.h"

int to_10(char *num_base, int base, int *res) {
	char *ptr = num_base - 1;
	int sign = 1;

	if (*num_base == '-') {
		ptr++;
		sign = -1;
	}

	while (*++ptr) {
		*res = *res * base + ((isdigit(*ptr)) ? (*ptr - '0') : (*ptr - 'A' + 10));
 	}

 	*res = sign * *res;

 	return SUCCESS;
}

int to_base(int num, int base, char **res) {
	char buff[BUFSIZ];
	char *ptr = buff + BUFSIZ - 1;
	int sign = 1;
	*ptr = 0;
	ptr--;

	if (num < 0) {
		num = abs(num);
		sign = -1;
	}

	while (num) {
		*ptr-- = num % base < 10 ? num % base + '0' : num % base + 'A' - 10;
		num /= base;
	}

	if (sign < 0) {
		*ptr = '-';
	} else {
		ptr++;
	}

	*res = (char*)malloc(sizeof(char) * (strlen(ptr) + 1));

	if (!*res) {
		return NO_MEMORY;
	}

	strcpy(*res, ptr);

	return SUCCESS;
}

int disjunction(int item_1, int item_2) { // +
	return item_1 | item_2;
}

int conjunction(int item_1, int item_2) { // &
	return item_1 & item_2;
}

int implication(int item_1, int item_2) { // ->
	return ~item_1 | item_2;
}

int inverse_implication(int item_1, int item_2) { // <-
	return ~item_2 | item_1;
}

int logical_equivalent(int item_1, int item_2) { // ~
	return item_1 == item_2;
}

int logical_addition(int item_1, int item_2) { // <>
	return item_1 ^ item_2;
}

int coimplication(int item_1, int item_2) { // +>
	return !implication(item_1, item_2);
}

int scheffer_stroke(int item_1, int item_2) { // 
	return !(conjunction(item_1, item_2));
}

int pier_arrow(int item_1, int item_2) {
	return !(disjunction(item_1, item_2));
}

int negation(int item_1, int item_2) {
	return abs(~item_2);
}

int execute_operation(char *operator, int item_1, int item_2) {
	operation func_operations[10] = {
		disjunction,
		conjunction,
		implication,
		inverse_implication,
		logical_equivalent,
		logical_addition,
		coimplication,
		scheffer_stroke,
		pier_arrow,
		negation
	};

	char *name_operations[10] = {
		"+",
		"&",
		"->",
		"<-",
		"~",
		"<>",
		"+>",
		"?",
		"!",
		"\\"
	};

	for (int i = 0; i < 10; i++) {
		if (strcmp(operator, name_operations[i]) == 0) {
			return func_operations[i](item_1, item_2);
		}
	}

	return SUCCESS;
}