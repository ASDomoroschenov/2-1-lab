#include <stdio.h>
#include <stdlib.h>
#include "../headers/stack.h"

int init_root_stack(Stack **stack, node_stack *init_node) {
	if (!*stack) {
		*stack = (Stack*)malloc(sizeof(Stack));

		if (!*stack) {
			return NO_MEMORY;
		}
	}

	(*stack)->root = init_node;
	(*stack)->top = init_node;
	(*stack)->root->next = NULL;
	(*stack)->root->prev = NULL;

	return SUCCESS;
}

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
	(*init_node)->prev = NULL;

	return SUCCESS;
}

int add_stack(Stack **stack, node_tree *node) {
	node_stack *add_node = NULL;
	int exit_code = init_node_stack(&add_node, node);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*stack) {
		exit_code = init_root_stack(stack, add_node);

		if (exit_code != SUCCESS) {
			return exit_code;
		}

		return SUCCESS;
	}

	add_node->prev = (*stack)->top;
	(*stack)->top->next = add_node;
	(*stack)->top = add_node;

	return SUCCESS;
}

int pop_stack(Stack **stack) {
	if ((*stack)->top != (*stack)->root) {
		node_stack *temp = (*stack)->top;

		(*stack)->top = (*stack)->top->prev;
		(*stack)->top->next = NULL;

		free(temp);
		temp = NULL;
	} else {
		free((*stack)->root);
		(*stack)->root = NULL;
		free(*stack);
		*stack = NULL;
	}

	return SUCCESS;
}