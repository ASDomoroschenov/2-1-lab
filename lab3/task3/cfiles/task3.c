#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/binom_tree.h"
#include "../headers/fib_heap.h"
#include "../headers/employee.h"

int cmp_bigger(employee *item_1, employee *item_2) {
	return item_1->wage > item_2->wage;
}

int cmp_lower(employee *item_1, employee *item_2) {
	return item_1->wage < item_2->wage;
}

int main(int argc, char *argv[]) {
	employee **workers = NULL;
	int count_workers = 0;
	int exit_code = 0;
	exit_code = get_workers(argv[1], &workers, &count_workers);

	if (exit_code == SUCCESS) {
		output_workers(workers, count_workers);
		fib_heap *heap = NULL;
		exit_code = get_heap(&heap, workers, count_workers, cmp_bigger);

		if (exit_code == SUCCESS) {
			employee *worker = NULL;

			for (int i = 0; i < count_workers; i++) {
				exit_code = get_extreme_heap(&worker, &heap, cmp_bigger);
				
				if (exit_code == SUCCESS) {
					printf("%d %s %s %lf\n", worker->id, worker->name, worker->surname, worker->wage);
					free_worker(&worker);
				} else {
					print_error(exit_code);
					return 0;
				}
			}

			free(heap);
			free(workers);
		} else {
			print_error(exit_code);
		}
	} else {
		print_error(exit_code);
	}

	return 0;
}