#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "../headers/string.h"

int len_number(int number) {
	int len = 0;

	while (number) {
		len++;
		number /= 10;
	}

	return len;
}

int join_symb(char symb, char **str, int *size) {
	if (*size == 0) {
		*size = 2;
		*str = (char*)malloc(sizeof(char) * *size);
		if (!*str) {
			return NO_MEMORY;
		}

		(*str)[0] = symb;
		(*str)[1] = 0;

		return SUCCESS;
	}

	if (strlen(*str) == *size - 1) {
		*size *= 2;
		*str = (char*)realloc(*str, sizeof(char) * *size);
		
		if (!*str) {
			return NO_MEMORY;
		}
	}

	(*str)[strlen(*str) + 1] = 0;
	(*str)[strlen(*str)] = symb;

	return SUCCESS;
}

int get_str(char **str, FILE *input) {
	char symb = fgetc(input);
	int size = 0;

	while (symb != '\n' && symb != EOF) {
		if (join_symb(symb, str, &size) != SUCCESS) {
			return NO_MEMORY;
		}
		
		symb = fgetc(input);
	}

	if (*str) {
		*str = (char*)realloc(*str, sizeof(char) * (strlen(*str) + 1));
	}

	return SUCCESS;
}

int get_token_str(char **ptr, char **token) {
	int size = 0;

	while (isspace(**ptr)) {
		(*ptr)++;
	}

	while (!isspace(**ptr) && **ptr && **ptr != EOF) {
		if (join_symb(**ptr, token, &size) == NO_MEMORY) {
			return NO_MEMORY;
		}
		(*ptr)++;
	}

	if (*token) {
		*token = (char*)realloc(*token, sizeof(char) * (strlen(*token) + 1));
	}

	return SUCCESS;
}

int token_count(char *str) {
	char _c = ' ';
	char *ptr = str - 1;
	int count = 0;

	while (*++ptr) {
		if (isspace(_c) && !isspace(*ptr)) {
			count++;
		}
		_c = *ptr;
	}

	return count;
}

int strip(char **str) {
	char *res_str = NULL;
	char *ptr_left = *str;
	char *ptr_right = *str + strlen(*str) - 1;

	while (isspace(*ptr_left)) {
		ptr_left++;
	}

	while (isspace(*ptr_right)) {
		ptr_right--;
	}

	res_str = (char*)malloc(sizeof(char) * (ptr_right - ptr_left + 2));
	
	if (!res_str) {
		return NO_MEMORY;
	}
	
	strncpy(res_str, ptr_left, ptr_right - ptr_left + 1);
	res_str[ptr_right - ptr_left + 1] = 0;
	free(*str);
	*str = res_str;

	return SUCCESS;
}

int correct_date(char **date) {
	char *date_correct = (char*)malloc(sizeof(char) * 20);
	char *ptr = *date - 1;
	char *ptr_correct = date_correct;

	if (!date_correct) {
		return NO_MEMORY;
	}

	while (!isspace(*++ptr)) {
		*ptr_correct++ = *ptr;
	}
	*ptr_correct++ = ' ';

	while (isspace(*ptr)) {
		ptr++;
	}

	strcpy(ptr_correct, ptr);
	free(*date);
	*date = date_correct;
}

int create_str(string *destination, char *source) {
	if (source) {
		destination->length = strlen(source);
		destination->text = (char*)malloc(sizeof(char) * destination->length);

		if (!destination->text) {
			return NO_MEMORY;
		}

		strncpy(destination->text, source, destination->length);

		return SUCCESS;
	}

	return SUCCESS;
}

int delete_char_str(int index, string *str) {
	if (str->text) {
		if (index >= 0 && index <= str->length) {
			char *new_str = (char*)malloc(sizeof(char) * str->length);
			
			if (!new_str) {
				return NO_MEMORY;
			}

			char *ptr = new_str;

			for (int i = 0; i < str->length; i++) {
				if (i != index) {
					*ptr++ = str->text[i];
				}
			}

			*ptr = 0;

			free(str->text);
			create_str(str, new_str);
			free(new_str);

			return SUCCESS;
		} else {
			return INVALID_INDEX;
		}
	}
	
	return SUCCESS;
}

//1 - первая строка больше
//0 - строки равны
//-1 - первая строка меньше

int cmp(string str_1, string str_2) {
	for (int i = 0; i < str_1.length && i < str_2.length; i++) {
		if (str_1.text[i] != str_2.text[i]) {
			return str_1.text[i] > str_2.text[i] ? 1 : -1;
		}
	}

	if (str_1.length == str_2.length) {
		return 0;
	}

	return str_1.length > str_2.length ? 1 : -1;
}

int cmp_str(string str_1, string str_2, int (*cmp)(string, string)) {
	return cmp(str_1, str_2);
}

int cpy_str(string *destination, string source) {
	if (destination->text) {
		free(destination->text);
		destination->length = 0;
	}

	destination->length = source.length;
	destination->text = (char*)malloc(sizeof(char) * source.length);
	
	if (!destination->text) {
		return NO_MEMORY;
	}
	
	strncpy(destination->text, source.text, destination->length);

	return SUCCESS;
}

int cat_str(string *destination, int count, ...) {
	va_list sources;
	va_start(sources, count);

	for (int i = 0; i < count; i++) {
		char *source = va_arg(sources, char*);

		if (!destination) {
			if (create_str(destination, source) == NO_MEMORY) {
				return NO_MEMORY;
			}
		} else {
			char *temp = NULL;
			temp = (char*)realloc(destination->text, sizeof(char) * (destination->length + strlen(source)));

			if (!temp) {
				return NO_MEMORY;
			}

			destination->text = temp;
			char *ptr = destination->text + destination->length - 1;
			destination->length += strlen(source);

			for (int i = 0; i < strlen(source); i++) {
				*++ptr = source[i];
			}
		}
	}

	return SUCCESS;
}

int duplicate_str(string *destination, string source) {
	destination->length = source.length;
	destination->text = (char*)malloc(sizeof(char) * destination->length);

	if (!destination->text) {
		return NO_MEMORY;
	}

	strncpy(destination->text, source.text, destination->length);

	return SUCCESS;
}

unsigned long long get_seconds_date_str(string str) {
	int day_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int minute_date[3] = {1440, 1440, 525600};
	int minute_time[2] = {60, 1};
	int count = 0;
	int date = 0;
	int i = 0;
	unsigned long long seconds = 0;

	for (i = 0; str.text[i] != ' '; i++) {
		if (str.text[i] == '.') {
			if (count == 1) {
				for (int j = 0; j < date; j++) {
					seconds += day_in_month[j] * minute_date[count];
				}
			} else {
				seconds += date * minute_date[count];
			}
			date = 0;
		} else {
			date = date * 10 + (str.text[i] - '0');
		}
	}
	seconds += minute_date[2] * date;
	date = 0;
	count = 0;
	i++;

	for (int j = i; j < str.length; j++) {
		if (str.text[i] == ':') {
			seconds += date * minute_time[count++];
			date = 0;
		} else {
			date = date * 10 + (str.text[i] - '0');
		}
	}

	return seconds * 60 + date;
}

unsigned long long get_seconds_date_chr(char *str) {
	int day_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int minute_date[3] = {1440, 1440, 525600};
	int minute_time[2] = {60, 1};
	int count = 0;
	int date = 0;
	int i = 0;
	char *ptr = str;
	unsigned long long seconds = 0;

	while (*ptr != ' ') {
		if (*ptr == '.') {
			if (count == 1) {
				for (int j = 0; j < date; j++) {
					seconds += day_in_month[j] * minute_date[count];
				}
			} else {
				seconds += date * minute_date[count];
			}
			count++;
			date = 0;
		} else {
			date = date * 10 + (*ptr - '0');
		}
		ptr++;
	}
	seconds += minute_date[2] * date;
	date = 0;
	count = 0;
	ptr++;

	while (*ptr) {
		if (*ptr == ':') {
			seconds += date * minute_time[count++];
			date = 0;
		} else {
			date = date * 10 + (*ptr - '0');
		}

		ptr++;
	}

	return seconds * 60 + date;
}

void print_str(string str, FILE *output) {
	for (int i = 0; i < str.length; i++) {
		fprintf(output, "%c", str.text[i]);
	}
	fprintf(output, "\n");
}

void print_centre_str(int len_space, string str, FILE *output) {
	int count_space = (len_space - str.length) / 2;

	for (int i = 0; i < count_space; i++) {
		fprintf(output, " ");
	}
	for (int i = 0; i < str.length; i++) {
		fprintf(output, "%c", str.text[i]);
	}
	
	if ((len_space - str.length) % 2 != 0) {
		count_space++;
	}

	for (int i = 0; i < count_space; i++) {
		fprintf(output, " ");
	}
	fprintf(output, "|");
}

void print_centre_int(int len_space, int number, FILE *output) {
	int count_space = (len_space - len_number(number)) / 2;

	for (int i = 0; i < count_space; i++) {
		fprintf(output, " ");
	}
	
	fprintf(output, "%d", number);
	
	if ((len_space - len_number(number)) % 2 != 0) {
		count_space++;
	}

	for (int i = 0; i < count_space; i++) {
		fprintf(output, " ");
	}
	fprintf(output, "|");
}

void print_centre_double(int len_space, double number, FILE *output) {
	int count_space = (len_space - len_number((int)number) - 3) / 2;

	for (int i = 0; i < count_space; i++) {
		fprintf(output, " ");
	}
	
	fprintf(output, "%.2lf", number);
	
	if ((len_space - len_number((int)number) - 3) % 2 != 0) {
		count_space++;
	}

	for (int i = 0; i < count_space; i++) {
		fprintf(output, " ");
	}
	fprintf(output, "|");
}

void print_error(int exit_code) {
	if (exit_code == NO_MEMORY) {
		fprintf(stderr, "NO_MEMORY: The system is out of memory\n");
	}
	if (exit_code == INVALID_ARGS) {
		fprintf(stderr, "INVALID_ARGS: Run with invalid arguments\n");
	}
	if (exit_code == INVALID_MAIL) {
		fprintf(stderr, "INVALID_MAIL: You enter invalid mail\n");
	}
	if (exit_code == INVALID_MAIL_ID) {
		fprintf(stderr, "INVALID_MAIL_ID: You enter invalid mail id\n");
	}
	if (exit_code == EMPTY_MAILS) {
		fprintf(stderr, "EMPTY_MAILS: You can find or delete in collections of empty mails\n");
	}
}

void free_str(string *str) {
	if (str->text) {
		free(str->text);
		str->text = NULL;
		str->length = 0;
	}
}