#ifndef TYPES_H
#define TYPES_H

enum ERRORS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	INVALID_ARGS = -3,
	NOT_TXT_FILE = -4,
	OPEN_FILE = -5
};

typedef struct tag_node_tree {
	struct tag_node_tree **subtrees;
	char key_tree;
	int curr_size;
	int size;
} node_tree;

typedef struct {
	node_tree *root;
} Tree;

typedef struct tag_node_stack {
	struct tag_node_stack *next;
	node_tree *key_stack;
} node_stack;

typedef struct {
	node_stack *top;
} Stack;

#endif // TYPES_H