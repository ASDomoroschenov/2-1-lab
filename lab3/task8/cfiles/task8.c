#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../headers/types.h"
#include "../headers/string.h"
#include "../headers/parse.h"
#include "../headers/bin_tree.h"
#include "../headers/pref_tree.h"

//префиксная форма

int main(int argc, char *argv[]) {
	bin_tree *b_tree = NULL;
	pref_tree *p_tree = NULL;

	if (argc == 2) {
		if (strcmp(argv[1] + strlen(argv[1]) - 4, ".txt") == 0) {
			parse(argv[1], &b_tree, &p_tree);
			if (b_tree) {
				printf("%d %s\n", max(b_tree->root)->num_matches, max(b_tree->root)->token);
				printf("%d %s\n", min(b_tree->root)->num_matches, min(b_tree->root)->token);
				printf("%d\n", depth(b_tree->root));
				printf("%s\n", b_tree->root->token);
				save_tree_in_file(&b_tree, "b_tree.txt");
				unload_tree_from_file(&b_tree, "b_tree.txt");
				free_pref_tree(&(p_tree->root));
				free(b_tree);
				free(p_tree);
			}
		} else {
			print_error(NOT_TXT_FILE);
		}
	} else {
		print_error(INVALID_ARGS);
	}

	return 0;
}