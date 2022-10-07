#include <stdio.h>
#include <stdlib.h>
#define SIZE_ARR 10

void input_arr(int *arr);
void output_arr(int *arr);
void find_max_min(int *arr, int *index_max, int *index_min);
void swap_max_min(int *arr);

int main() {
	int arr[SIZE_ARR];

	input_arr(arr);
	output_arr(arr);
	swap_max_min(arr);
	output_arr(arr);

	return 0;
}

void input_arr(int *arr) {
	for (int i = 0; i < SIZE_ARR; i++) {
		arr[i] = rand() % 100;
	}
}

void output_arr(int *arr) {
	for (int i = 0; i < SIZE_ARR; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void find_max_min(int *arr, int *index_max, int *index_min) {
	*index_max = 0;
	*index_min = 0;

	for (int i = 1; i < SIZE_ARR; i++) {
		if (arr[i] > arr[*index_max]) {
			*index_max = i;
		}
		if (arr[i] < arr[*index_min]) {
			*index_min = i;
		}
	}
}

void swap_max_min(int *arr) {
	int index_min = 0;
	int index_max = 0;
	find_max_min(arr, &index_max, &index_min);
	int temp = arr[index_max];

	arr[index_max] = arr[index_min];
	arr[index_min] = temp;
}