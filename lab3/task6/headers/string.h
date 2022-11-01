#ifndef STRING_H
#define STRING_H

#include "types.h"

int len_number(int);
int join_symb(char, char**, int*);
int get_str(char**, FILE*);
int get_token_str(char**, char**);
int token_count(char*);
int strip(char**);
int correct_date(char**);
int create_str(string*, char*);
int delete_char_str(int, string*);
int cmp(string, string);
int cmp_str(string, string, int (*cmp)(string, string));
int cpy_str(string*, string);
int cat_str(string*, int, ...);
int duplicate_str(string*, string);
unsigned long long get_seconds_date_chr(char*);
unsigned long long get_seconds_date_str(string);
void print_str(string, FILE*);
void print_centre_str(int, string, FILE*);
void print_centre_int(int, int, FILE*);
void print_centre_double(int, double, FILE*);
void print_error(int);
void free_str(string*);

#endif // STRING_H