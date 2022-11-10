#include <stdio.h>
#include <stdlib.h>
#include "../headers/stack.h"

int init_node_stack(node_stack **init_node, node_tree *node) {
	if (*init_node) {
		free(*init_node);
		*init_node = NULL;
	}

	*init_node = (node_stack*)malloc(sizeof(node_stack));

	if (!*init_node) {
		return NO_MEMORY;
	}

	(*init_node)->key_stack = node;
	(*init_node)->next = NULL;

	return SUCCESS;
}

int add_stack(Stack **stack, node_tree *node) {
	node_stack *add_node = NULL;
	int exit_code = init_node_stack(&add_node, node);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*stack) {
		*stack = (Stack*)malloc(sizeof(Stack));

		if (!*stack) {
			free(add_node);
			add_node = NULL;

			return NO_MEMORY;
		}

		(*stack)->top = add_node;
	
		return SUCCESS;
	}

	add_node->next = (*stack)->top;
	(*stack)->top = add_node;

	return SUCCESS;
}

int pop_stack(Stack **stack) {
	if ((*stack)->top->next == NULL) {
		free((*stack)->top);
		(*stack)->top = NULL;
		free(*stack);
		*stack = NULL;

		return SUCCESS;
	}

	node_stack *temp = (*stack)->top;

	(*stack)->top = (*stack)->top->next;
	free(temp);
	temp = NULL;

	return SUCCESS;
}

void free_stack(Stack **stack) {
	if (*stack) {
		while ((*stack)->top) {
			pop_stack(stack);
		}

		free(*stack);
		*stack = NULL;
	}
}