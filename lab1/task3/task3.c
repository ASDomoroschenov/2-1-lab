#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE_FILE_NAME 40

bool check_args(int argc, char const *argv[]);
void get_answer(int argc, char const *argv[]);
bool delete_numbers_from_file(int argc, char const *argv[]);
bool count_letters_in_lines(int argc, char const *argv[]);
bool count_special_letters_in_lines(int argc, char const *argv[]);
bool character_replacement(int argc, char const *argv[]);
bool replacement_of_tokens(int argc, char const *argv[]);

int main(int argc, char const *argv[]) {
	if (check_args(argc, argv)) {
		get_answer(argc, argv);
	} else {
		fprintf(stderr, "%s\n", "Invalid aruments");
	}

	return 0;
}

bool check_args(int argc, char const *argv[]) {
	char flags[] = "disaf";
	char begin_flags[] = "-/";

	if (strchr(begin_flags, argv[1][0])) {
		if (argv[1][1] == 'n') {
			if (strlen(argv[1]) == 3 && strchr(flags, argv[1][2]) && (argc > 2 && argc < 5)) {
				if (argc == 4) {
					if (strcmp(argv[2] + (strlen(argv[2]) - 4), ".txt") == 0 &&
						strcmp(argv[3] + (strlen(argv[3]) - 4), ".txt") == 0) {
						return true;
					}
				} else {
					if (strcmp(argv[2] + (strlen(argv[2]) - 4), ".txt") == 0) {
						return true;
					}
				}
			}
		} else {
			if (strchr(flags, argv[1][1]) && argc == 3) {
				if (strcmp(argv[2] + (strlen(argv[2]) - 4), ".txt") == 0) {
					return true;
				}
			}
		}
	}

	return false;
}

void get_answer(int argc, char const *argv[]) {
	if (strchr(argv[1], 'd')) {
		if (!delete_numbers_from_file(argc, argv)) {
			fprintf(stderr, "%s\n", "File did not open");
		}
	}
	if (strchr(argv[1], 'i')) {
		if (!count_letters_in_lines(argc, argv)) {
			fprintf(stderr, "%s\n", "File did not open");
		}
	}
	if (strchr(argv[1], 's')) {
		if (!count_special_letters_in_lines(argc, argv)) {
			fprintf(stderr, "%s\n", "File did not open");
		}
	}
	if (strchr(argv[1], 'a')) {
		if (!character_replacement(argc, argv)) {
			fprintf(stderr, "%s\n", "File did not open");
		}
	}
	if (strchr(argv[1], 'f')) {
		if (!replacement_of_tokens(argc, argv)) {
			fprintf(stderr, "%s\n", "File did not open");
		}
	}
}

bool delete_numbers_from_file(int argc, char const *argv[]) {
	FILE *file = NULL;
	FILE *temp = NULL;
	char symb = 0;
	char filename[SIZE_FILE_NAME] = "out_";
	strcat(filename, argv[2]);
	
	if (argv[1][1] == 'n') {
		if (argc == 4) {
			temp = fopen(argv[3], "w");
		} else {
			temp = fopen(filename, "w");
		}
	} else {
		temp = fopen(filename, "w");
	}

	if ((file = fopen(argv[2], "r")) != NULL) {
		while (!feof(file)) {
			symb = fgetc(file);
			if (!isdigit(symb) && (symb != EOF) || isspace(symb)) {
				fputc(symb, temp);
			}
		}

		fclose(file);
		fclose(temp);

		if (argv[1][1] != 'n') {
			if ((file = fopen(argv[2], "w")) != NULL &&
				(temp = fopen(filename, "r")) != NULL) {
				while (!feof(temp)) {
					symb = fgetc(temp);
					if (symb != EOF) {
						fputc(symb, file);
					}
				}
			} else {
				return false;
			}

			fclose(file);
			fclose(temp);
			remove(filename);
		}
	} else {
		return false;
	}

	return true;
}

bool count_letters_in_lines(int argc, char const *argv[]) {
	FILE *file = NULL;
	FILE *temp = NULL;
	char symb = 0;
	int count = 0;
	char filename[SIZE_FILE_NAME] = "out_";
	strcat(filename, argv[2]);
	
	if (argv[1][1] == 'n') {
		if (argc == 4) {
			temp = fopen(argv[3], "w");
		} else {
			temp = fopen(filename, "w");
		}
	} else {
		temp = fopen(filename, "w");
	}


	if ((file = fopen(argv[2], "r")) != NULL) {
		while (!feof(file)) {
			symb = fgetc(file);
			if (isalpha(symb) && (symb != EOF)) {
				count++;
			}
			if (symb == '\n' || symb == EOF) {
				fprintf(temp, "%d\n", count);
				count = 0;
			}
		}

		fclose(file);
		fclose(temp);
	} else {
		return false;
	}

	return true;
}

bool count_special_letters_in_lines(int argc, char const *argv[]) {
	FILE *file = NULL;
	FILE *temp = NULL;
	char symb = 0;
	int count = 0;
	char filename[SIZE_FILE_NAME] = "out_";
	strcat(filename, argv[2]);
	
	if (argv[1][1] == 'n') {
		if (argc == 4) {
			temp = fopen(argv[3], "w");
		} else {
			temp = fopen(filename, "w");
		}
	} else {
		temp = fopen(filename, "w");
	}

	if ((file = fopen(argv[2], "r")) != NULL) {
		while (!feof(file)) {	
			symb = fgetc(file);
			if ((!(isalpha(symb) || isdigit(symb) || isspace(symb))) && (symb != EOF) && (symb != '\n')) {
				count++;
			}
			if (symb == '\n' || symb == EOF) {
				fprintf(temp, "%d\n", count);
				count = 0;
			}
		}
	} else {
		return false;
	}

	return true;
}

bool character_replacement(int argc, char const *argv[]) {
	FILE *file = NULL;
	FILE *temp = NULL;
	char symb = 0;
	char filename[SIZE_FILE_NAME] = "out_";
	strcat(filename, argv[2]);
	
	if (argv[1][1] == 'n') {
		if (argc == 4) {
			temp = fopen(argv[3], "w");
		} else {
			temp = fopen(filename, "w");
		}
	} else {
		temp = fopen(filename, "w");
	}

	if ((file = fopen(argv[2], "r")) != NULL) {
		while (!feof(file)) {
			symb = fgetc(file);
			if (isdigit(symb)) {
				fprintf(temp, "%c", symb);
			} else {
				fprintf(temp, "%d", symb);
			}
		}

		fclose(file);
		fclose(temp);

		if (argv[1][1] != 'n') {
			if ((file = fopen(argv[2], "w")) != NULL &&
				(temp = fopen(filename, "r")) != NULL) {
				while (!feof(temp)) {
					symb = fgetc(temp);
					if (symb != EOF) {
						fputc(symb, file);
					}
				}
			} else {
				return false;
			}

			fclose(file);
			fclose(temp);
			remove(filename);
		}
	} else {
		return false;
	}

	return true;
}

bool replacement_of_tokens(int argc, char const *argv[]) {
	FILE *file = NULL;
	FILE *temp = NULL;
	char symb = 0;
	int count_tokens = 1;
	bool met_letter = false;
	char filename[SIZE_FILE_NAME] = "out_";
	strcat(filename, argv[2]);
	
	if (argv[1][1] == 'n') {
		if (argc == 4) {
			temp = fopen(argv[3], "w");
		} else {
			temp = fopen(filename, "w");
		}
	} else {
		temp = fopen(filename, "w");
	}

	if ((file = fopen(argv[2], "r")) != NULL) {
		while (!feof(file)) {
			symb = fgetc(file);

			if ((count_tokens % 2 == 0 || count_tokens % 5 == 0) && 
				symb != '\n' && symb != EOF && symb != ' ' && symb != '\t') {
				fprintf(temp, "%d", symb);
			} else {
				if (symb != EOF) {
					fprintf(temp, "%c", symb);
				}
			}

			if (symb != ' ' && symb != '\n' && symb != EOF && symb != '\t') {
				met_letter = true;
			} else {
				if (met_letter) {
					count_tokens++;
					met_letter = false;
				}
			}
		}

		fclose(file);
		fclose(temp);

		if (argv[1][1] != 'n') {
			if ((file = fopen(argv[2], "w")) != NULL &&
				(temp = fopen(filename, "r")) != NULL) {
				while (!feof(temp)) {
					symb = fgetc(temp);
					if (symb != EOF) {
						fputc(symb, file);
					}
				}
			} else {
				return false;
			}

			fclose(file);
			fclose(temp);
			remove(filename);
		}
	} else {
		return false;
	}

	return true;
}
