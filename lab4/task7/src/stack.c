#include <stdlib.h>
#include <string.h>
#include "../lib/stack.h"

int create_stack(stack_t **stack, enum TYPE type) {
	*stack = (stack_t*)malloc(sizeof(stack_t));

	if (!*stack) {
		return NO_MEMORY;
	}

	(*stack)->top = NULL;
	(*stack)->type = type;

	return SUCCESS;
}

int init_node_stack(node_stack **init_node, void *data, enum TYPE type) {
	if (!data) {
		return EMPTY_STRING;
	}

	*init_node = (node_stack*)malloc(sizeof(node_stack));

	if (!*init_node) {
		return NO_MEMORY;
	}

	if (type == T_CHAR) {
		(*init_node)->data.symb = *((char*)data);
	} else {
		(*init_node)->data.number = *((double*)data);
	}

	(*init_node)->next = NULL;

	return SUCCESS;
}

int push(stack_t **stack, void *data) {
	node_stack *push_node = NULL;
	int exit_code = init_node_stack(&push_node, data, (*stack)->type);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	push_node->next = (*stack)->top;
	(*stack)->top = push_node;

	return SUCCESS;
}

int pop(stack_t **stack) {
	if (*stack) {
		if ((*stack)->top) {
			node_stack *temp = (*stack)->top;

			(*stack)->top = (*stack)->top->next;

			free(temp);
			temp = NULL;
		}
	}

	return SUCCESS;
}

void *pick(stack_t *stack) {
	return stack->type == T_CHAR ? (void*)(&(stack->top->data.symb)) : (void*)(&(stack->top->data.number));
}

int is_empty(stack_t *stack) {
	if (stack) {
		return stack->top == NULL;
	}

	return 1;
}

int free_stack(stack_t **stack) {
	if (*stack) {
		while ((*stack)->top) {
			pop(stack);
		}

		free(*stack);
		*stack = NULL;
	}
}