#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SIZE 128
#define SIZE_TOKEN 30

void get_answer(int *arr, char const *argv[]);
int check_args(int argc, char const *argv[]);
bool is_number(char *string);
long long my_atoi(const char *digit);
bool get_array(const char *filename, int *arr);
void output_arr(int *arr);
void get_odd_positions(int *arr, int *result_arr);
void get_even_digits(int *arr, int *result_arr);
int get_distant_number(int *arr, int index);
int sum_previous_digits(int *arr, int index);
int sum_of_smaller(int *arr, int index);

int main(int argc, char const *argv[]) {
	int arr[MAX_SIZE];
	int check = check_args(argc, argv);
	if (check == 0) {
		get_array(argv[2], arr);
		output_arr(arr);
		get_answer(arr, argv);
	} else {
		if (check == 1) {
			fprintf(stderr, "%s\n", "File didn't open");
		}
		if (check == 2) {
			fprintf(stderr, "%s\n", "Ivalid options");
		}
		if (check == 3) {
			fprintf(stderr, "%s\n", "Invalis count of arguments");
		}
		if (check == 4) {
			fprintf(stderr, "%s\n", "Invalid file extension");
		}
	}

	return 0;
}

void get_answer(int *arr, char const *argv[]) {
	int result_arr[MAX_SIZE];
	int index = 0;
	if (*arr != 0) {
		if (strchr(argv[1], 'a')) {
			get_odd_positions(arr, result_arr);
			if (*result_arr != 0) {
				output_arr(result_arr);
			} else {
				printf("There are no such numbers\n");
			}
		}
		if (strchr(argv[1], 'b')) {
			get_even_digits(arr, result_arr);
			if (*result_arr != 0) {
				output_arr(result_arr);
			} else {
				printf("There are no such numbers\n");
			}
		}
		if (strchr(argv[1], 'c')) {
			if ((scanf("%d", &index) == 1) && (index >= 0) && (index < *arr)) {
				printf("%d\n", get_distant_number(arr, index));
			} else {
				fprintf(stderr, "%s\n", "For option c: invalid index");
			}
		}
		if (strchr(argv[1], 'd')) {
			if ((scanf("%d", &index) == 1) && (index >= 0) && (index < *arr)) {
				printf("%d\n", sum_previous_digits(arr, index));
			} else {
				fprintf(stderr, "%s\n", "For option d: invalid index");
			}
		}
		if (strchr(argv[1], 'e')) {
			if ((scanf("%d", &index) == 1) && (index >= 0) && (index < *arr)) {
				printf("%d\n", sum_of_smaller(arr, index));
			} else {
				fprintf(stderr, "%s\n", "For option e: invalid index");
			}
		}
	} else {
		printf("File is empty\n");
	}
}

int check_args(int argc, char const *argv[]) {
	FILE *file = NULL;
	char flags[] = "abcde";

	if (argc == 3) {
		if (strcmp(argv[2] + strlen(argv[2]) - 4, ".txt") == 0) {
			if ((file = fopen(argv[2], "r")) != NULL) {
				fclose(file);
			} else {
				return 1;
			}
			if (strlen(argv[1]) >= 2 &&
			    argv[1][0] == '-' && 
			    strchr(flags, argv[1][1]) != NULL) {
				return 0;
			}
			return 2;
		}
		return 4;
	}

	return 3;
}

bool is_number(char *string) {
	if (strlen(string) > 1 && *string == '-') {
		string++;
	}

	while (*string) {
		if (!isdigit(*string)) {
			return false;
		}
		string++;
	}

	return true;
}

long long my_atoi(const char *string) {
	int result = 0;
	int sign = 1;

	if (*string == '-') {
		sign = -1;
		*string++;
	}

	while (*string != '\0') {
		result = result * 10 + (*string - '0');
		string++;
	}

	return sign * result;
}

bool get_array(const char *filename, int *arr) {
	FILE *file = NULL;
	char digit[SIZE_TOKEN];
	int index = 1;

	if ((file = fopen(filename, "r")) != NULL) {
		while (!feof(file) && index < 129) {
			if (fscanf(file, "%s", digit) != EOF && is_number(digit)) {
				arr[index] = my_atoi(digit);
				index++;
			}
		}

		*arr = index;
	} else {
		return false;
	}

	return true;
}

void output_arr(int *arr) {
	for (int i = 1; i < *arr; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void get_odd_positions(int *arr, int *result_arr) {
	int index_odd = 1;

	for (int i = 1; i < *arr; i++) {
		if ((i - 1) % 2 != 0) {
			result_arr[index_odd] = arr[i];
			index_odd++;
		}
	}

	*result_arr = index_odd;
}

void get_even_digits(int *arr, int *result_arr) {
	int index_even = 1;

	for (int i = 1; i < *arr; i++) {
		if (arr[i] % 2 == 0) {
			result_arr[index_even] = arr[i];
			index_even++;
		}
	}

	*result_arr = index_even;
}

int get_distant_number(int *arr, int index) {
	int most_distant_index = 1;
	index++;

	for (int i = 2; i < *arr; i++) {
		if (abs(arr[index] - arr[most_distant_index]) < abs(arr[index] - arr[i])) {
			most_distant_index = i;
		}
	}

	return arr[most_distant_index];
}

int sum_previous_digits(int *arr, int index) {
	index++;
	int sum = 0;

	for (int i = 1; i < index; i++) {
		sum += arr[i];
	}

	return sum;
}

int sum_of_smaller(int *arr, int index) {
	int sum = 0;
	index++;

	for (int i = 1; i < *arr; i++) {
		if (arr[i] < arr[index]) {
			sum += arr[i];
		}
	}

	return sum;
}