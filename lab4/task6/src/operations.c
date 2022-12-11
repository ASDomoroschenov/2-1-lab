#include <string.h>
#include "../lib/operations.h"

int conjunction(int item_1, int item_2) {
	return item_1 & item_2;
}

int disjunction(int item_1, int item_2) {
	return item_1 | item_2;
}

int inversion(int item_1, int item_2) {
	return ~item_1;
}

int implication(int item_1, int item_2) {
	return !item_1 | item_2;
}

int coimplication(int item_1, int item_2) {
	return !implication(item_1, item_2);
}

int logical_addition(int item_1, int item_2) {
	return item_1 ^ item_2;
}

int logical_equivalent(int item_1, int item_2) {
	return item_1 == item_2;
}

int scheffer_stroke(int item_1, int item_2) {
	return !item_1;
}

int vebb_function(int item_1, int item_2) {
	return !(item_1 | item_2);
}

int execute(char *operator, int item_1, int item_2) {
	operation func_operations[9] = {
		conjunction,
		disjunction,
		inversion,
		implication,
		coimplication,
		logical_addition,
		logical_equivalent,
		scheffer_stroke,
		vebb_function
	};

	char *name_operations[9] = {
		"&",
		"|",
		"~",
		"->",
		"+>",
		"<>",
		"=",
		"!",
		"?"
	};

	for (int i = 0; i < 9; i++) {
		if (strcmp(operator, name_operations[i]) == 0) {
			return func_operations[i](item_1, item_2);
		}
	}
}