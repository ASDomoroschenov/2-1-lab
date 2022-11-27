#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../lib/string.h"
#include "../lib/stack_char.h"
#include "../lib/stack_double.h"
#include "../lib/types.h"

#define eps 0.0000000001

int check_expression(char *str) {
	if (str) {
		char *ptr = str - 1;

		while (*++ptr) {
			if (isalpha(*ptr)) {
				return 0;
			}
		}

		return 1;
	}

	return EMPTY_STRING;
}

int check_brackets(char *str) {
	if (str) {
		char *ptr = str - 1;
		stack_char *stack = NULL;
		int exit_code = 0;
		int res = 0;

		while (*++ptr) {
			if (*ptr == '(') {
				exit_code = push_char(&stack, *ptr);

				if (exit_code != SUCCESS) {
					return exit_code;
				}
			}
			if (*ptr == ')') {
				if (stack->top) {
					pop_char(&stack);
				} else {
					free(stack);
					stack = NULL;

					return 0;
				}
			}
		}

		if (is_empty_char(stack)) {
			free(stack);
			stack = NULL;

			return 1;
		}

		free_stack_char(&stack);

		return 0;
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

int get_string_number(char *str, int *index, char **res_str) {
	*res_str = (char*)malloc(sizeof(char) * (strlen(str) + 1));

	if (!*res_str) {
		return NO_MEMORY;
	}

	char *ptr_res_str = *res_str;

	for (;*index < strlen(str) && isdigit(str[*index]); (*index)++) {
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
	if (check_expression(str) == 0) {
		return INVALID_EXPRESSION;
	}

	stack_char *stack = NULL;
	char *number = NULL;
	int exit_code = 0;

	for (int i = 0; i < strlen(str); i++) {
		if (isdigit(str[i])) {
			exit_code = get_string_number(str, &i, &number);

			if (exit_code != SUCCESS) {
				free(*res_str);
				free_stack_char(&stack);
				*res_str = NULL;

				return exit_code;
			}

			exit_code = add_str(res_str, 2, number, " ");

			if (exit_code != SUCCESS) {
				free(*res_str);
				free_stack_char(&stack);
				free(number);
				*res_str = NULL;

				return exit_code;
			}

			free(number);
			number = NULL;
		} else {
			if (str[i] == '(') {
				exit_code = push_char(&stack, str[i]);

				if (exit_code != SUCCESS) {
					free(*res_str);
					free_stack_char(&stack);
					*res_str = NULL;

					return exit_code;
				}
			} else {
				if (str[i] == ')') {
					while (!is_empty_char(stack) && stack->top->data != '(') {
						exit_code = add_chr(res_str, 2, pop_char(&stack), ' ');
						
						if (exit_code != SUCCESS) {
							free(*res_str);
							free_stack_char(&stack);
							*res_str = NULL;

							return exit_code;
						}
					}

					pop_char(&stack);
				} else {
					if (is_operator(str[i])) {
						while (!is_empty_char(stack) && (get_priority(stack->top->data) >= get_priority(str[i]))) {
							exit_code = add_chr(res_str, 2, pop_char(&stack), ' ');
							
							if (exit_code != SUCCESS) {
								free(*res_str);
								free_stack_char(&stack);
								*res_str = NULL;
								
								return exit_code;
							}
						}

						exit_code = push_char(&stack, str[i]);

						if (exit_code != SUCCESS) {
							free(*res_str);
							free_stack_char(&stack);
							*res_str = NULL;

							return exit_code;
						}
					}
				}
			}
		}
	}

	while (!is_empty_char(stack)) {
		exit_code = add_chr(res_str, 2, pop_char(&stack), ' ');
		
		if (exit_code != SUCCESS) {
			free(*res_str);
			free_stack_char(&stack);
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
		case '%': return first % second;
		case '^': return pow(first, second);  
	}
}

int count_expression(char *str, double *result) {
	stack_double *stack = NULL;
	char *expression = NULL;
	int exit_code = to_postfix(str, &expression);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	for (int i = 0; i < strlen(expression); i++) {
		if (isdigit(expression[i])) {
			char *number = NULL;
			exit_code = get_string_number(expression, &i, &number);

			if (exit_code != SUCCESS) {
				free(expression);
				free_stack_double(&stack);

				return exit_code;
			}

			exit_code = push_double(&stack, atof(number));

			if (exit_code != SUCCESS) {
				free(expression);
				free_stack_double(&stack);

				return exit_code;
			}
			
			free(number);
			number = NULL;
		} else {
			if (is_operator(expression[i])) {
				double second = !is_empty_double(stack) ? pop_double(&stack) : 0;
				double first = !is_empty_double(stack) ? pop_double(&stack) : 0;
				
				if (expression[i] == '/' && (second - eps < 0)) {
					free(expression);
					free_stack_double(&stack);

					return DIVISION_BY_ZERO;
				}

				exit_code = push_double(&stack, execute(first, second, expression[i]));

				if (exit_code != SUCCESS) {
					free(expression);
					free_stack_double(&stack);

					return exit_code;
				}
			}
		}
	}

	free(expression);
	*result = pop_double(&stack);
	free(stack);
	
	return SUCCESS;
}