#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "binom_tree.h"
#include "binom_heap.h"
#include "employee.h"

typedef int (*func_cmp)(employee*, employee*);

int cmp_bigger(employee *item_1, employee *item_2) {
	return item_1->wage > item_2->wage;
}

int cmp_lower(employee *item_1, employee *item_2) {
	return item_1->wage < item_2->wage;
}

int check_args(int argc, char const *argv[]) {
	if (argc == 3) {
		if (argv[1][0] == '-' && (argv[1][1] == 'a' || argv[1][1] == 'd')) {
			if (strlen(argv[2]) > 4 && strcmp(".txt", argv[2] + strlen(argv[2]) - 4) == 0) {
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

int main(int argc, char const *argv[]) {
	func_cmp cmp[2] = {cmp_bigger, cmp_lower};
	int check = check_args(argc, argv);

	if (check != -1) {
		FILE *output = fopen(argv[2], "w");
		employee **workers = NULL;
		int func_index = check;
		size_t size = 0;
		int exit_code = get_workers("test.txt", &workers, &size);

		if (exit_code == SUCCESS) {
			binom_heap *heap = get_heap(workers, size, cmp[func_index]);
			employee *worker = NULL;

			for (int i = 0; i < size; i++) {
				worker = get_extreme_heap(heap, cmp[func_index]);
				fprintf(output, "%d %s %s %lf\n", worker->id, worker->name, worker->surname, worker->wage);
				free_worker(&worker);
			}

			free(heap);
			free(workers);
			fclose(output);
		} else {
			if (exit_code == FILE_DIDNT_OPEN) {
				fprintf(stderr, "%s\n", "FILE_DIDNT_OPEN: Can't open file");
			}
			if (exit_code == NO_MEMORY) {
				fprintf(stderr, "%s\n", "NO_MEMORY: The system is out of memory");
			}
		}
	}

	return 0;
}