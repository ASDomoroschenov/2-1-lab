#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE_TOKEN 50

int check_args(int argc, char const *argv[]);
void swap_column(char const *argv[]);
void put_line(FILE *file, char (*tokens)[SIZE_TOKEN]);
void file_overwrite(const char *to_here, const char *from_here);

int main(int argc, char const *argv[]) {
	int check = check_args(argc, argv);
	if (check == 0) {
		swap_column(argv);
	} else {
		if (check == 1) {
			fprintf(stderr, "%s\n", "Invalid counts of token in line");
		}
		if (check == 2) {
			fprintf(stderr, "%s\n", "Invalid arguments");
		}
		if (check == 3) {
			fprintf(stderr, "%s\n", "File didn't open");
		}
		if (check == 4) {
			fprintf(stderr, "%s\n", "Wrong file extension");
		}
	}

	return 0;
}

int check_args(int argc, char const *argv[]) {
	FILE *file = NULL;
	int count_tokens = 0;
	char token_1[SIZE_TOKEN];
	char token_2[SIZE_TOKEN];
	char token_3[SIZE_TOKEN];
	char symb = 0;

	if (argc == 2) {
		if (strcmp(argv[1] + strlen(argv[1]) - 4, ".txt") == 0) {
			if ((file = fopen(argv[1], "r")) != NULL) {
				while (!feof(file)) {
					count_tokens = fscanf(file, "%s%s%s", token_1, token_2, token_3);
					if (count_tokens != 3 && count_tokens != -1) {
						return 1;
					}
				}
				fclose(file);
				return 0;
			}
			return 3;
		}
		return 4;
	}

	return 2;
}

void swap_column(char const *argv[]) {
	FILE *file = NULL;
	FILE *temp = fopen("temp.txt", "w");
	int count_tokens = 0;
	char tokens[3][SIZE_TOKEN];
	char symb = 0;

	if ((file = fopen(argv[1], "r")) != NULL) {
		while (!feof(file)) {
			count_tokens = fscanf(file, "%s%s%s", tokens[0], tokens[1], tokens[2]);
			if (count_tokens == 3) {
				put_line(temp, tokens);
			}
		}
		fclose(file);
		fclose(temp);
		file_overwrite(argv[1], "temp.txt");
		remove("temp.txt");
	}
}

void put_line(FILE *file, char (*tokens)[SIZE_TOKEN]) {
	fputs(tokens[2], file);
	fputc(' ', file);
	fputs(tokens[0], file);
	fputc(' ', file);
	fputs(tokens[1], file);
	fputc('\n', file);
}

void file_overwrite(const char *to_here, const char *from_here) {
	char symb = 0;
	FILE *file = NULL;
	FILE *temp = NULL;

	if ((file = fopen(to_here, "w")) != NULL &&
		(temp = fopen(from_here, "r")) != NULL) {
		while (!feof(temp)) {
			symb = fgetc(temp);
			if (symb != EOF) {
				fputc(symb, file);
			}
		}
	}

	fclose(file);
	fclose(temp);
}