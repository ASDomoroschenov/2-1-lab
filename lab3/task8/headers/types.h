#ifndef TYPES_H
#define TYPES_H

enum ERRORS {
	SUCCESS = -1,
	NO_MEMORY = -2,
	OPEN_FILE = -3,
	NOT_TXT_FILE = -4,
	INVALID_ARGS = -5
};

typedef struct tag_node_bin_tree{
	char *token;
	struct tag_node_bin_tree *parent;
	struct tag_node_bin_tree *left;
	struct tag_node_bin_tree *right;
	int num_matches;
} node_bin_tree;

typedef struct {
	node_bin_tree *root;
} bin_tree;

typedef struct tag_node_pref_tree {
	struct tag_node_pref_tree **subtrees;
	int curr_size;
	int size;
	char lexeme_char;
	node_bin_tree *lexem;
} node_pref_tree;

typedef struct {
	node_pref_tree *root;
} pref_tree;

#endif // TYPES_H