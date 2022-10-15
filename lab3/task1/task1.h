#ifndef TASK1_H
#define TASK1_H

enum ERRORS {
	SUCCESS = -1,
	INVALID_NOTATION = -2,
	NO_MEMORY = -3,
	NOT_INTEGER = -4
};

int scanf_int(int *number);
int notation(char**, int, int);
int get_remainder(int, int);
int sum(int, int);
int subtraction(int, int);
int reverse(char**, char*);

#endif // TASK1_H