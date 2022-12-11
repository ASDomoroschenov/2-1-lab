#ifndef OPERATIONS_H
#define OPERATIONS_H

typedef int (*operation)(int, int);

int conjunction(int, int);
int disjunction(int, int);
int inversion(int, int);
int implication(int, int);
int coimplication(int, int);
int logical_addition(int, int);
int logical_equivalent(int, int);
int scheffer_stroke(int, int);
int vebb_function(int, int);
int execute(char*, int, int);

#endif //OPERATIONS_H