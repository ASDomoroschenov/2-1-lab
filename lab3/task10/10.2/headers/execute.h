#ifndef EXECUTE_H
#define EXECUTE_H

#include "types.h"

int save_in_file(char*, List**);
int delete_instructions(List**, int);
int instruction_processing(char*, int);
void print_error(int);

#endif // EXECUTE_H