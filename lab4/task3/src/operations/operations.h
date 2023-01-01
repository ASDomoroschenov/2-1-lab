#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../types/types.h"

typedef int (*operation)(int, int);

int get_operator(char*, char**);
int to_10(char*, int, int*); // (str_num, base_str_num, res)
int to_base(int, int, char**); // (num, base, res)
int disjunction(int, int);
int conjunction(int, int);
int implication(int, int);
int inverse_implication(int, int);
int logical_equivalent(int, int);
int logical_addition(int, int);
int coimplication(int, int);
int scheffer_stroke(int, int);
int pier_arrow(int, int);
int negation(int, int);
int execute_operation(char*, int, int);

#endif // OPERATIONS_H