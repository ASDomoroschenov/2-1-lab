#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../lib/types.h"
#include "../lib/string.h"
#include "../lib/stack.h"
#include "../lib/check.h"

int get_priority(char *operator) {
	if (strcmp(operator, "(") == 0) {
		return 0;
	}

	if (strcmp(operator, "|") == 0 ||
		strcmp(operator, "->") == 0 ||
		strcmp(operator, "<>") == 0 ||
		strcmp(operator, "=") == 0) {
		return 1;
	}

	if (strcmp(operator, "?") == 0 ||
		strcmp(operator, "!") == 0 ||
		strcmp(operator, "+>") == 0 ||
		strcmp(operator, "&") == 0) {
		return 2;
	}

	if (strcmp(operator, "~") == 0) {
		return 3;
	}
}

int to_postfix(char *str, char **res_str) {
	if (*res_str) {
		free(*res_str);
		*res_str = NULL;
	}

	if (!str) {
		return EMPTY_STRING;
	}
	if (!check_expression(str)) {
		return INVALID_EXPRESSION;
	}
	if (!check_brackets(str)) {
		return INVALID_BRACKETS;
	}

	stack_t *stack = NULL;
	int exit_code = 0;
	char *token = NULL;

	for (int i = 0; i < strlen(str); i++) {
		if (!isspace(str[i])) {
			if (str[i] == '-' ||
				str[i] == '<' ||
				str[i] == '+') {

				exit_code = add_chr(&token, 2, str[i], str[i + 1]);
				i++;
				
				if (exit_code != SUCCESS) {
					free_stack(&stack);

					return exit_code;
				}
			} else {
				exit_code = add_chr(&token, 1, str[i]);

				if (exit_code != SUCCESS) {
					free_stack(&stack);

					return exit_code;
				}
			}

			if (isalpha(token[0]) ||
				token[0] == '0' ||
				token[0] == '1') {
				exit_code = add_str(res_str, 2, token, " ");

				if (exit_code != SUCCESS) {
					free(*res_str);
					free(token);
					free_stack(&stack);
					*res_str = NULL;

					return exit_code;
				}
			} else {
				if (token[0] == '(') {
					exit_code = push(&stack, token, STRING);

					if (exit_code != SUCCESS) {
						free(*res_str);
						free(token);
						free_stack(&stack);
						*res_str = NULL;

						return exit_code;
					}
				} else {
					if (token[0] == ')') {
						while (!is_empty(stack) && strcmp(stack->top->data->string, "(") != 0) {
							exit_code = add_str(res_str, 2, (char*)pick(stack), " ");

							if (exit_code != SUCCESS) {
								free(*res_str);
								free(token);
								free_stack(&stack);
								*res_str = NULL;

								return exit_code;
							}

							pop(&stack);
						}

						pop(&stack);
					} else {
						if (is_operator(token)) {
							while (!is_empty(stack) && (get_priority(stack->top->data->string) >= get_priority(token))) {
								exit_code = add_str(res_str, 2, (char*)pick(stack), " ");
								
								if (exit_code != SUCCESS) {
									free(*res_str);
									free(token);
									free_stack(&stack);
									*res_str = NULL;
									
									return exit_code;
								}

								pop(&stack);
							}

							exit_code = push(&stack, token, STRING);

							if (exit_code != SUCCESS) {
								free(*res_str);
								free(token);
								free_stack(&stack);
								*res_str = NULL;

								return exit_code;
							}
						}
					}
				}
			}

			free(token);
			token = NULL;
		}
	}

	while (!is_empty(stack)) {
		exit_code = add_str(res_str, 2, (char*)pick(stack), " ");
	
		if (exit_code != SUCCESS) {
			free(*res_str);
			free_stack(&stack);
			*res_str = NULL;

			return exit_code;
		}

		pop(&stack);
	}

	free(stack);

	if (!*res_str) {
		return EMPTY_STRING;
	}

	return SUCCESS;
}