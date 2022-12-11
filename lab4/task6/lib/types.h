#ifndef TYPES_H
#define TYPES_H

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	INVALID_EXPRESSION = -3,
	INVALID_BRACKETS = -4,
	EMPTY_STRING = -5,
	CANT_OPEN_FILE = -6,
	INVALID_COUNT_ARGS = -7,
	INVALID_FILE_IN_ARGS = -8
};

enum TYPE {
	STRING,
	NODE_BIN_TREE
};

typedef struct tag_node_tree {
	struct tag_node_tree *left;
	struct tag_node_tree *right;
	char *data;
} node_bin_tree;

typedef struct {
	node_bin_tree *root;
} bin_tree;

typedef union data_union {
	char *string;
	node_bin_tree *node;
} data_types;

typedef struct tag_node_stack {
	struct tag_node_stack *next;
	data_types *data;
} node_stack;

typedef struct {
	node_stack *top;
	enum TYPE type;
} stack_t;

typedef struct {
	char name;
	int value;
} variable;

typedef struct {
	variable *var_names;
	int size;
} array_var_names;

typedef struct {
	int **arr;
	int row;
	int col;
} table_t;

#endif // TYPES_H