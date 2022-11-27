#include <stdlib.h>
#include "../lib/stack_char.h"

int init_node_stack_char(node_stack_char **init_node, char symb) {
	*init_node = (node_stack_char*)malloc(sizeof(node_stack_char));

	if (!*init_node) {
		return NO_MEMORY;
	}

	(*init_node)->next = NULL;
	(*init_node)->data = symb;

	return SUCCESS;
}

int push_char(stack_char **stack, char symb) {
	node_stack_char *push_node = NULL;
	int exit_code = init_node_stack_char(&push_node, symb);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*stack) {
		*stack = (stack_char*)malloc(sizeof(stack_char));

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

char pop_char(stack_char **stack) {
	char res = (*stack)->top->data;
	node_stack_char *temp = (*stack)->top;
	(*stack)->top = (*stack)->top->next;
	free(temp);
	temp = NULL;

	return res;
}

int is_empty_char(stack_char *stack) {
	if (stack) {
		return stack->top == NULL;
	}

	return 1;
}

int free_stack_char(stack_char **stack) {
	if (*stack) {
		while ((*stack)->top) {
			pop_char(stack);
		}

		free(*stack);
		*stack = NULL;
	}
}