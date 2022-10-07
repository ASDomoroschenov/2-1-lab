#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ERROR 0.000001

double **generate_matrix(int *row, int *col);
void output_matrix(double **matrix, int row, int col);
double **multiply_matrix(double **matrix_1, int row_1, int col_1, double **matrix_2, int row_2, int col_2);
void row_decrement(double *row_begin, double *row, double dec, int col);
double determinant(double **matrix, int row, int col);
void free_matrix(double **matrix, int row);

int main() {
	srand(time(NULL));
	int row_1 = 0;
	int col_1 = 0;
	int row_2 = 0;
	int col_2 = 0;
	double **matrix_1 = generate_matrix(&row_1, &col_1);
	double **matrix_2 = generate_matrix(&row_2, &col_2);
	double **multiply = multiply_matrix(matrix_1, row_1, col_1, matrix_2, row_2, col_2);

	printf("A:\n");
	output_matrix(matrix_1, row_1, col_1);
	printf("B:\n");
	output_matrix(matrix_2, row_2, col_2);

	if (multiply != NULL) {
		printf("A x B:\n");
		output_matrix(multiply, row_1, col_2);
	} else {
		fprintf(stderr, "%s\n\n", "A x B:\nMultiplication cannot be done");
	}

	if (determinant(matrix_1, row_1, col_1) != ERROR) {
		printf("det(A) = %ld\n", (long long)determinant(matrix_1, row_1, col_1));
	} else {
		fprintf(stderr, "%s\n\n", "det(A): Invalid count of row and col");
	}

	if (determinant(matrix_2, row_2, col_2) != ERROR) {
		printf("det(B) = %ld\n", (long long)determinant(matrix_2, row_2, col_2));
	} else {
		fprintf(stderr, "%s\n\n", "det(B): Invalid count of row and col");
	}

	if (multiply != NULL) {
		if (determinant(multiply, row_1, col_2) != ERROR) {
			printf("det(A x B) = %ld\n", (long long)determinant(multiply, row_1, col_2));
		} else {
			fprintf(stderr, "%s\n\n", "det(A x B): Invalid count of row and col");
		}
		free_matrix(multiply, row_1);
	} else {
		fprintf(stderr, "%s\n\n", "det(A x B): Multiplication cannot be done");
	}

	free_matrix(matrix_1, row_1);
	free_matrix(matrix_2, row_2);

	return 0;
}

double **generate_matrix(int *row, int *col) {
	*row = rand() % 10 + 1;
	*col = rand() % 10 + 1;

	double **matrix = (double**)malloc(sizeof(double*) * *row);
	for (int i = 0; i < *row; i++) {
		matrix[i] = (double*)malloc(sizeof(double) * *col);
		for (int j = 0; j < *col; j++) {
			matrix[i][j] = rand() % 200 - 100;
		}
	}

	return matrix;
}

void output_matrix(double **matrix, int row, int col) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%10.0lf ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


double **multiply_matrix(double **matrix_1, int row_1, int col_1, double **matrix_2, int row_2, int col_2) {
	double **result = NULL;

	if (col_1 == row_2) {
		result = (double**)malloc(sizeof(double*) * row_1);
		for (int i = 0; i < row_1; i++) {
			result[i] = (double*)malloc(sizeof(double) * col_2);
		}

		for (int i = 0; i < row_1; i++) {
			for (int j = 0; j < col_2; j++) {
				result[i][j] = 0;
				for (int k = 0; k < col_1; k++) {
					result[i][j] += matrix_1[i][k] * matrix_2[k][j];
				}
			}
		}
	}

	return result;
}

void row_decrement(double *row_begin, double *row, double dec, int col) {
	for (int i = 0; i < col; i++) {
		row[i] -= dec * row_begin[i];
	}
}

double determinant(double **matrix, int row, int col) {
	if (row == col) {
		double res = 1.0;
		int size = row;
		double **copy_matrix = (double**)malloc(sizeof(double*) * size);

		for (int i = 0; i < size; i++) {
			copy_matrix[i] = (double*)malloc(sizeof(double) * size);
			for (int j = 0; j < size; j++) {
				copy_matrix[i][j] = matrix[i][j];
			}
		}

		for (int row_begin = 0; row_begin < size; row_begin++) {
			for (int row_dec = row_begin + 1; row_dec < size; row_dec++) {
				row_decrement(copy_matrix[row_begin], copy_matrix[row_dec], copy_matrix[row_dec][row_begin] / copy_matrix[row_begin][row_begin], size);
			}
		}

		for (int i = 0; i < row; i++) {
			res *= copy_matrix[i][i];
		}

		free_matrix(copy_matrix, size);

		return res;
	}

	return ERROR;
}

void free_matrix(double **matrix, int row) {
	for (int i = 0; i < row; i++) {
		free(matrix[i]);
	}
	free(matrix);
}
