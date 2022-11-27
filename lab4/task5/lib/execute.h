#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdio.h>
#include "./types.h"

int check_args(int, char**);
void print_error(int, FILE*);
int files_processing(int, char**);

#endif // EXECUTE_H