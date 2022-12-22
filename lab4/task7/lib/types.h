#ifndef TYPES_H
#define TYPES_H

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	EMPTY_STRING = -3,
	DIVISION_BY_ZERO = -4,
	INVALID_BRACKETS = -5,
	INVALID_EXPRESSION = -6,
	OPEN_FILE = -7,
	HAVENT_VAR = -8,
	EMPTY_ARRAY = -9
};

enum TYPE {
	T_CHAR,
	T_DOUBLE
};

typedef struct {
	char *name;
	double value;
} MemoryCell;

typedef struct {
	MemoryCell *vars;
	int current_size;
	int size;
} array_vars;

typedef union data_union {
	char symb;
	double number;
} data_types;

typedef struct tag_node_stack {
	struct tag_node_stack *next;
	data_types data;
} node_stack;

typedef struct {
	node_stack *top;
	enum TYPE type;
} stack_t;

#endif // TYPES_H