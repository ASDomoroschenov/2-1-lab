#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/binom_tree.h"
#include "../headers/fib_heap.h"
#include "../headers/employee.h"

typedef int (*func_cmp)(employee*, employee*);

int cmp_bigger(employee *item_1, employee *item_2) {
	return item_1->wage >= item_2->wage;
}

int cmp_lower(employee *item_1, employee *item_2) {
	return item_1->wage <= item_2->wage;
}

int check_args(int argc, char *argv[]) {
	if (argc == 4) {
		if (argv[1][0] == '-' && (argv[1][1] == 'a' || argv[1][1] == 'd')) {
			if (strlen(argv[2]) > 4 && strcmp(".txt", argv[2] + strlen(argv[2]) - 4) == 0 &&
				strlen(argv[3]) > 4 && strcmp(".txt", argv[3] + strlen(argv[3]) - 4) == 0) {
				if (argv[1][1] == 'a') {
					return 1;
				} else {
					return 0;
				}
			}
		}
	}

	return -1;
}

int main(int argc, char *argv[]) {
	func_cmp cmp[2] = {cmp_bigger, cmp_lower};
	int check = check_args(argc, argv);

	if (check != -1) {
		employee **workers = NULL;
		int count_workers = 0;
		int exit_code = 0;
		exit_code = get_workers(argv[2], &workers, &count_workers);

		if (exit_code == SUCCESS) {
			fib_heap *heap = NULL;
			exit_code = get_heap(&heap, workers, count_workers, cmp[check]);

			if (exit_code == SUCCESS) {
				employee *worker = NULL;
				FILE *output = NULL;

				if ((output = fopen(argv[3], "w")) != NULL) {
					for (int i = 0; i < count_workers; i++) {
						exit_code = get_extreme_heap(&worker, &heap, cmp[check]);
						
						if (exit_code == SUCCESS) {
							fprintf(output, "%d %s %s %lf\n", worker->id, worker->name, worker->surname, worker->wage);
							free_worker(&worker);
						} else {
							print_error(exit_code);
							return 0;
						}
					}

					free(heap);
					free(workers);
					fclose(output);
				} else {
					print_error(FILE_DIDNT_OPEN);
				}
			} else {
				print_error(exit_code);
			}
		} else {
			print_error(exit_code);
		}
	} else {
		print_error(INVALID_ARGS);
	}

	return 0;
}