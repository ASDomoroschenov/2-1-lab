#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "binom_tree.h"
#include "binom_heap.h"
#include "employee.h"

int main(int argc, char const *argv[]) {
	char *str = NULL;
	employee **workers = NULL;
	size_t size = 0;
	int exit_code = get_workers("test.txt", &workers, &size);
	binom_heap *heap = get_heap_from_arr(workers, size);
	output_heap(heap);
	employee *worker = del_min(heap);
	printf("%d\n", worker->id);
	free_worker(&worker);
	worker = del_min(heap);
	printf("%d\n", worker->id);
	free_worker(&worker);
	worker = del_min(heap);
	printf("%d\n", worker->id);
	free_worker(&worker);
	worker = del_min(heap);
	printf("%d\n", worker->id);
	free_worker(&worker);
	worker = del_min(heap);
	printf("%d\n", worker->id);
	free_worker(&worker);
	free(workers);
	free(heap->root);
	free(heap);

	return 0;
}