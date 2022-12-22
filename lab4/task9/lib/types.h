#ifndef TYPES_H
#define TYPES_H

enum ERROR {
	SUCCESS = -1,
	NO_MEMORY = -2,
	CANT_OPEN_FILE = -3,
	INVALID_ARGS = -4
};

typedef struct tag_node_list {
	struct tag_node_list *next;
	char *def_name;
	char *value;
	long long hash;
} node_list;

typedef struct {
	node_list *root;
	int size;
} list_t;

typedef struct {
	list_t **array_list;
	int max_size_list;
	int min_size_list;
	int size;
} hash_table;

#endif // TYPES_H