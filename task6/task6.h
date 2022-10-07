#ifndef TASK6_H
#define TASK6_H

enum error {
	NO_MEMORY = -2,
	CANT_OPEN_FILE = -3,
	SUCCESS = -4
};

typedef struct tag_coord_tokens {
	int count_line;
	int count_symb;
} coord_tokens;

int find_substr(char **filenames, coord_tokens **tokens, int *sizes, char *str, int count, ...);
int find_substr_in_file(coord_tokens **tokens, char *substr, char *filename);
int my_substr(char *str, char *substr);
int join_symb(char **str, int *size, char symb);
int get_token(char **token ,FILE *file, bool *is_new_line, int *count_symb);
void free_tokens(coord_tokens **tokens, int count);
void free_filenames(char **filenames, int count);

#endif // TASK6_H