#ifndef TYPES_H
#define TYPES_H

enum ERRORS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	OPEN_FILE = -3,
	INVALID_EXPRESSION = -4,
	HAVENT_VAR = -5
};

typedef struct {
	char name;
	int value;
} variable;

typedef struct {
	variable *vars;
	int current_size;
	int size;
} array_variables;

#endif // TYPES_H