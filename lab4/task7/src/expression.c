#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../lib/expression.h"

#define eps 0.0000000001

int check_brackets(char *str) {
	if (str) {
		char *ptr = str - 1;
		int count = 0;

		while (*++ptr) {
			if (count < 0) {
				return 0;
			}
			if (*ptr == '(') {
				count ++;
			}
			if (*ptr == ')') {
				count--;
			}
		}

		return count == 0;
	}

	return EMPTY_STRING;
}

int get_priority(char symb) {
	switch (symb) {
		case '(': return 0;
		case '+': return 1;
		case '-': return 1;
		case '*': return 2;
		case '/': return 2;
		case '%': return 2;
		case '^': return 3;
	}
}

int is_operator(char symb) {
	if (symb == '(' ||
		symb == '+' ||
		symb == '-' ||
		symb == '*' ||
		symb == '/' ||
		symb == '%' ||
		symb == '^') {
		return 1;
	}

	return 0;
}

int get_string_token(char *str, int *index, char **res_str) {
	*res_str = (char*)malloc(sizeof(char) * (strlen(str) + 1));

	if (!*res_str) {
		return NO_MEMORY;
	}

	char *ptr_res_str = *res_str;

	for (;*index < strlen(str) && isalnum(str[*index]); (*index)++) {
		*ptr_res_str++ = str[*index];
	}
	
	(*index)--;
	*ptr_res_str = 0;

	*res_str = (char*)realloc(*res_str, sizeof(char) * (strlen(*res_str) + 1));

	if (!*res_str) {
		return NO_MEMORY;
	}

	return SUCCESS;
}

int to_postfix(char *str, char **res_str) {
	if (*res_str) {
		free(*res_str);
		*res_str = NULL;
	}

	if (!str) {
		return EMPTY_STRING;
	}
	if (check_brackets(str) == 0) {
		return INVALID_BRACKETS;
	}

	stack_t *stack = NULL;
	char *token = NULL;
	int len_str = strlen(str);
	int exit_code = create_stack(&stack, T_CHAR);

	for (int i = 0; i < len_str; i++) {
		if (isalnum(str[i])) {
			exit_code = get_string_token(str, &i, &token);

			if (exit_code != SUCCESS) {
				free(*res_str);
				free_stack(&stack);
				*res_str = NULL;

				return exit_code;
			}

			exit_code = add_str(res_str, 2, token, " ");

			if (exit_code != SUCCESS) {
				free(*res_str);
				free_stack(&stack);
				free(token);
				*res_str = NULL;

				return exit_code;
			}

			free(token);
			token = NULL;
		} else {
			if (str[i] == '(') {
				exit_code = push(&stack, &str[i]);

				if (exit_code != SUCCESS) {
					free(*res_str);
					free_stack(&stack);
					*res_str = NULL;

					return exit_code;
				}
			} else {
				if (str[i] == ')') {
					while (!is_empty(stack) && stack->top->data.symb != '(') {
						exit_code = add_chr(res_str, 2, *((char*)pick(stack)), ' ');
						pop(&stack);

						if (exit_code != SUCCESS) {
							free(*res_str);
							free_stack(&stack);
							*res_str = NULL;

							return exit_code;
						}
					}

					pop(&stack);
				} else {
					if (is_operator(str[i])) {
						while (!is_empty(stack) && (get_priority(stack->top->data.symb) >= get_priority(str[i]))) {
							exit_code = add_chr(res_str, 2, *((char*)pick(stack)), ' ');
							pop(&stack);

							if (exit_code != SUCCESS) {
								free(*res_str);
								free_stack(&stack);
								*res_str = NULL;
								
								return exit_code;
							}
						}

						exit_code = push(&stack, &str[i]);

						if (exit_code != SUCCESS) {
							free(*res_str);
							free_stack(&stack);
							*res_str = NULL;

							return exit_code;
						}
					}
				}
			}
		}
	}

	while (!is_empty(stack)) {
		exit_code = add_chr(res_str, 2, *((char*)pick(stack)), ' ');
		pop(&stack);

		if (exit_code != SUCCESS) {
			free(*res_str);
			free_stack(&stack);
			*res_str = NULL;

			return exit_code;
		}
	}

	free(stack);

	if (!*res_str) {
		return EMPTY_STRING;
	}

	return SUCCESS;
}

double execute(double first, double second, char operator) {
	switch (operator) {
		case '+': return first + second;
		case '-': return first - second;
		case '*': return first * second;
		case '/': return first / second;
		case '%': return (int)first % (int)second;
		case '^': return pow(first, second);  
	}
}

int count_expression(char *str, array_vars array, double *result) {
	stack_t *stack = NULL;
	char *expression = NULL;
	double d_number = 0.0;
	char *token = NULL;
	int exit_code = to_postfix(str, &expression);
	if (exit_code != SUCCESS) {
		return exit_code;
	}

	int len_expression = strlen(expression);

	exit_code = create_stack(&stack, T_DOUBLE);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	for (int i = 0; i < len_expression; i++) {
		if (isalnum(expression[i])) {
			exit_code = get_string_token(expression, &i, &token);

			if (exit_code != SUCCESS) {
				free(expression);
				free_stack(&stack);

				return exit_code;
			}

			if (is_number(token)) {
				d_number = atof(token);
			} else {
				exit_code = get_var_value(array, token, &d_number);

				if (exit_code != SUCCESS) {
					return exit_code;
				}
			}

			exit_code = push(&stack, &d_number);

			if (exit_code != SUCCESS) {
				free(expression);
				free_stack(&stack);

				return exit_code;
			}
			
			free(token);
			token = NULL;
		} else {
			if (is_operator(expression[i])) {
				double second = !is_empty(stack) ? *((double*)pick(stack)) : 0;
				pop(&stack);
				double first = !is_empty(stack) ? *((double*)pick(stack)) : 0;
				pop(&stack);

				if (expression[i] == '/' && (second - eps < 0)) {
					free(expression);
					free_stack(&stack);

					return DIVISION_BY_ZERO;
				}

				double res_execute = execute(first, second, expression[i]);

				exit_code = push(&stack, &res_execute);

				if (exit_code != SUCCESS) {
					free(expression);
					free_stack(&stack);

					return exit_code;
				}
			}
		}
	}

	free(expression);
	*result = *((double*)pick(stack));
	pop(&stack);
	free(stack);
	
	return SUCCESS;
}