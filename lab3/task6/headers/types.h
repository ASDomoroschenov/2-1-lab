#ifndef TYPES_H
#define TYPES_H

enum ERRORS {
	NO_MEMORY = -2,
	SUCCESS = -3,
	INVALID_ARGS = -4,
	INVALID_INDEX = -5,
	INVALID_MAIL_ID = -6,
	INVALID_MAIL = -7,
	EMPTY_MAILS = -8,
	INVALID_DATETIME = -9
};

typedef struct {
	char *text;
	int length;
} string;

typedef struct {
	string city;
	string street;
	int home;
	int flat;
	string index;
} adress;

typedef struct {
	adress *sender;
	adress *recipient;
	double weight;
	string mail_id;
	string time_create;
	string time_delivery;
} mail;

typedef struct {
	adress *curr_post_office;
	mail **mails;
	int curr_size;
	int size;
} post;

typedef struct Node {
	struct Node **childs;
	int length;
	int degree;
	mail *key;
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
	node_list *extreme;
	int length;
} binom_heap;

#endif // TYPES_H