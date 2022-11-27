#include <stdlib.h>
#include "../lib/stack_char.h"

int init_node_stack_double(node_stack_double **init_node, double symb) {
	*init_node = (node_stack_double*)malloc(sizeof(node_stack_double));

	if (!*init_node) {
		return NO_MEMORY;
	}

	(*init_node)->next = NULL;
	(*init_node)->data = symb;

	return SUCCESS;
}

int push_double(stack_double **stack, double symb) {
	node_stack_double *push_node = NULL;
	int exit_code = init_node_stack_double(&push_node, symb);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*stack) {
		*stack = (stack_double*)malloc(sizeof(stack_double));

		if (!*stack) {
			free(push_node);
			return NO_MEMORY;
		}

		(*stack)->top = NULL;
	}

	push_node->next = (*stack)->top;
	(*stack)->top = push_node;

	return SUCCESS;
}

double pop_double(stack_double **stack) {
	double res = (*stack)->top->data;
	node_stack_double *temp = (*stack)->top;
	(*stack)->top = (*stack)->top->next;
	free(temp);
	temp = NULL;

	return res;
}

int is_empty_double(stack_double *stack) {
	if (stack) {
		return stack->top == NULL;
	}

	return 1;
}

int free_stack_double(stack_double **stack) {
	if (*stack) {
		while ((*stack)->top) {
			pop_double(stack);
		}

		free(*stack);
		*stack = NULL;
	}
}