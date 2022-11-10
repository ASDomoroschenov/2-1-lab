#ifndef TYPES_H
#define TYPES_H

enum ERRORS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	OPEN_FILE = -3,
	INVALID_ARGS = -4
};

typedef struct tag_node {
	struct tag_node *next;
	char *key;
} Node;

typedef struct {
	Node *root;
} List;

typedef struct {
	Node *head;
	Node *tail;
} Queue;

typedef struct {
	Node *top;
} Stack;

#endif // TYPES_H