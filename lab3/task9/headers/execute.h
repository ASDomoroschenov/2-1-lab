#ifndef EXECUTE_H
#define EXECUTE_H

#include "../headers/types.h"

int cmp(T*, T*);
int check_args(int, char**);
int is_all_file_closed(int*, int);
int get_list(dbl_list**, int, char**);
int get_heap(binom_heap**, int, char**);
int output_list_file(dbl_list**, char*);
int output_heap_file(binom_heap**, char*);
void print_error(int);

#endif // EXECUTE_H