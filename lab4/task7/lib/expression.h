#ifndef POSTFIX_H
#define POSTFIX_H

#include "array_vars.h"
#include "string.h"
#include "stack.h"
#include "types.h"

int check_brackets(char*);
int get_priority(char);
int is_operator(char);
int get_string_token(char*, int*, char**);
int to_postfix(char*, char**);
double execute(double, double, char);
int count_expression(char*, array_vars, double*);

#endif // POSTFIX_H