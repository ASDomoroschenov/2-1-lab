#ifndef TYPES_H
#define TYPES_H

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	EMPTY_LIST = -3,
	EMPTY_HEAP = -4,
	OPEN_FILE = -5,
	INVALID_ARGS = -6,
	NOT_TXT_FILE = -7
};

typedef struct {
	char *text;
	int length;
} string;

typedef struct {
	int prior;
	string text_field;
} T;

typedef struct tag_list_node {
	T *key;
	struct tag_list_node *prev;
	struct tag_list_node *next;
} list_node;

typedef struct {
	list_node *root;
} dbl_list;

typedef struct Node {
	struct Node **childs;
	int length;
	int degree;
	T *key;
} node_tree;

typedef struct {
	node_tree *root;
} binom_tree;

typedef struct Tag_node_list {
	struct Tag_node_list *next;
	binom_tree *tree;
} node_list;

typedef struct {
	node_list *root;
	node_list *min;
	int length;
} binom_heap;

#endif // TYPES_H