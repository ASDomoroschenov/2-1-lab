#ifndef TYPES_H
#define TYPES_H

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	INVALID_EXPRESSION = -3,
	INVALID_BRACKETS = -4,
	EMPTY_STRING = -5,
	DIVISION_BY_ZERO = -6,
	CANT_OPEN_FILE = -7,
	INVALID_COUNT_ARGS = -8,
	INVALID_FILE_IN_ARGS = -9
};

typedef struct tag_node_stack_char {
	struct tag_node_stack_char *next;
	char data;
} node_stack_char;

typedef struct {
	node_stack_char *top;
} stack_char;

typedef struct tag_node_stack_double {
	struct tag_node_stack_double *next;
	double data;
} node_stack_double;

typedef struct {
	node_stack_double *top;
} stack_double;

#endif // TYPES_H