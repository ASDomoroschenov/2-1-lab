#ifndef POSTFIX_H
#define POSTFIX_H

int check_expression(char*);
int check_brackets(char*);
int get_priority(char);
int is_operator(char);
int get_string_number(char*, int*, char**);
int to_postfix(char*, char**);
double execute(double, double, char);
int count_expression(char*, double*);

#endif // POSTFIX_H