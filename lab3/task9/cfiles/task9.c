#include <stdio.h>
#include <stdlib.h>
#include "../headers/types.h"
#include "../headers/string.h"
#include "../headers/check.h"
#include "../headers/dbl_list.h"
#include "../headers/binom_heap.h"
#include "../headers/execute.h"

int main(int argc, char *argv[]) {
	dbl_list *list = NULL;
	binom_heap *heap = NULL;
	int exit_code = check_args(argc, argv);

	if (exit_code == SUCCESS) {
		exit_code = get_list(&list, argc, argv);
		if (exit_code == SUCCESS) {
			exit_code = get_heap(&heap, argc, argv);
			if (exit_code == SUCCESS) {
				exit_code = output_list_file(&list, "./output/output_list.txt");
				if (exit_code != SUCCESS) {
					print_error(exit_code);
				}
				exit_code = output_heap_file(&heap, "./output/output_heap.txt");
				if (exit_code != SUCCESS) {
					print_error(exit_code);
				}
			} else {
				print_error(exit_code);
			}
		} else {
			print_error(exit_code);
		}
	} else {
		print_error(exit_code);
	}

	return 0;
}