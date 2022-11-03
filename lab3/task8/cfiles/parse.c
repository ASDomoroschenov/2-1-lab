#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/bin_tree.h"
#include "../headers/pref_tree.h"
#include "../headers/string.h"

int parse(char *name_file, bin_tree **b_tree, pref_tree **p_tree) {
	node_bin_tree *add_bin_node = NULL;
	FILE *read_file = NULL;
	char *lexem = NULL;
	int exit_code = 0;

	if ((read_file = fopen(name_file, "r")) != NULL) {
		while (!feof(read_file)) {
			exit_code = get_token(&lexem, read_file, is_bad, is_good);
			if (exit_code != SUCCESS) {
				return exit_code;
			}
			if (lexem) {
				if (have_lexem(*p_tree, lexem)) {
					node_bin_tree *node = find_node((*b_tree)->root, lexem);
					node->num_matches++;
					rebuild_bin_tree(&node, b_tree);
					set_ptr_lexem(*b_tree, p_tree, lexem);
				} else {
					exit_code = add_bin_lexem(&add_bin_node, b_tree, lexem);
					if (exit_code != SUCCESS) {
						return exit_code;
					}
					exit_code = add_pref_lexem(add_bin_node, p_tree, lexem);
					if (exit_code != SUCCESS) {
						return exit_code;
					}
				}
				free(lexem);
				lexem = NULL;
			}
		}
		fclose(read_file);
	}

	return SUCCESS;
}