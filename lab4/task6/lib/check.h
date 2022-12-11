#ifndef CHECK_H
#define CHECK_H

#include "types.h"

int is_unary(char*, int);
int is_binary(char*, int);
int check_unary(char*, int);
int check_binary(char*, int);
int count_operands(char*);
int count_bin_operators(char*);
int check_expression(char*);
int check_brackets(char*);
int is_operand(char*);
int is_operator(char*);

#endif // CHECK_H