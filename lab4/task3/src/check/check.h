#ifndef CHECK_H
#define CHECK_H

int is_operator(char);
int is_unary(char);
int check_binary(char*);
int check_unary(char*);
int check_read(char*);
int check_write(char*);
int check_expression(char*);
int is_binary_exp(char*);
int is_valid_num(char*, int); // (char *number, int base)

#endif // CHECK_H