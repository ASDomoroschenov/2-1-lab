#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum errors {
	GOOD_ARGS,
	INVALID_COUNT_ARG,
	INVALID_OPTION
};

int check_args(int argc, char *argv[]);
int length(char *str);
char *reverse(char *str);
char to_upper(int symb);
char *odd_symb_upper(char *str);
bool is_digit(int symb);
bool is_alpha(int symb);
char *digits_letters_other(char *str);
char *my_cat(char *str_first, char *str_second);
void print_answer(char *argv[]);

int main(int argc, char *argv[]) {
	int check = check_args(argc, argv);

	if (check == GOOD_ARGS) {
		print_answer(argv);
	} else {
		if (check == INVALID_OPTION) {
			fprintf(stderr, "%s\n", "Invalid option");
		}
		if (check == INVALID_COUNT_ARG) {
			fprintf(stderr, "%s\n", "Invalid count of argumemts");
		}
	}

	return 0;
}
 
int check_args(int argc, char *argv[]) {
	char flags[] = "lrunc";

	if (argc >= 3) {
		if (argv[1][0] == '-' && strchr(flags, argv[1][1])) {
			if (argv[1][1] == 'c') {
				if (argc == 4) {
					return GOOD_ARGS;
				}
				return INVALID_COUNT_ARG;
			}

			return GOOD_ARGS;
		}
		return INVALID_OPTION;
	}

	return INVALID_COUNT_ARG;
}

bool is_digit(int symb) {
	return symb >= 48 && symb <= 57;
}

bool is_alpha(int symb) {
	return (symb >= 65 && symb <= 90) || (symb >= 97 && symb <= 122);
}

char to_upper(int symb) {
	return symb >= 97 && symb <= 122 ? symb - 32 : symb;
}

int length(char *str) {
	char *ptr = str;

	while (*(++ptr)) ;

	return ptr - str;
}

char *reverse(char *str) {
	char *reverse_str = (char*)malloc(sizeof(char) * length(str) + 1);
	char *ptr_rev = reverse_str + length(str);
	char *ptr_str = str;
	*ptr_rev-- = 0;

	while (*ptr_str) {
		*ptr_rev-- = *ptr_str++;
	}
	
	return reverse_str;
}

char *odd_symb_upper(char *str) {
	char *res_str = (char*)malloc(sizeof(char) * length(str) + 1);
	res_str[length(str)] = 0;

	for (int i = 0; i < length(str); i++) {
		res_str[i] = i % 2 != 0 ? to_upper(str[i]) : str[i];
	}

	return res_str;
}

char *digits_letters_other(char *str) {
	char *res_str = (char*)malloc(sizeof(char*) * length(str) + 1);
	char *ptr_res = res_str;
	char *ptr_str = str;

	while (*ptr_str) {
		if (is_digit(*ptr_str)) {
			*ptr_res++ = *ptr_str;
		}
		ptr_str++;
	}
	ptr_str = str;

	while (*ptr_str) {
		if (is_alpha(*ptr_str)) {
			*ptr_res++ = *ptr_str;
		}
		ptr_str++;
	}
	ptr_str = str;

	while (*ptr_str) {
		if (!is_digit(*ptr_str) && !is_alpha(*ptr_str)) {
			*ptr_res++ = *ptr_str;
		}
		ptr_str++;
	}

	*ptr_res = 0;

	return res_str;
}

char *my_cat(char *str_first, char *str_second) {
	char *res_str = (char*)malloc(sizeof(char) * length(str_first) + sizeof(char) * length(str_second) + 1);
	char *ptr_res = res_str;
	char *ptr_first = str_first;
	char *ptr_second = str_second;

	while (*ptr_first) {
		*ptr_res++ = *ptr_first++;
	}

	while (*ptr_second) {
		*ptr_res++ = *ptr_second++;
	}

	*ptr_res = 0;

	return res_str;
}

void print_answer(char *argv[]) {
	switch (argv[1][1]) {
		case 'l': 
			printf("Length: %d\n", length(argv[2])); 
			break;
		case 'r': 
			printf("Reverse: %s\n", reverse(argv[2]));
			break;
		case 'u': 
			printf("Element in an odd position in uppercase: %s\n", odd_symb_upper(argv[2]));
			break;
		case 'n': 
			printf("In order(numbers, letters, etc.): %s\n", digits_letters_other(argv[2]));
			break;
		case 'c': 
			printf("String concatenation: %s\n", my_cat(argv[2], argv[3]));
			break;
	}
}