#include <stdlib.h>
#include <string.h>
#include "../lib/stack.h"

int init_node_stack(node_stack **init_node, void *data, int type) {
	if (!data) {
		return EMPTY_STRING;
	}

	*init_node = (node_stack*)malloc(sizeof(node_stack));

	if (!*init_node) {
		return NO_MEMORY;
	}

	(*init_node)->data = (data_types*)malloc(sizeof(data_types));

	if (!(*init_node)->data) {
		return NO_MEMORY;
	}

	if (type == STRING) {
		(*init_node)->data->string = (char*)malloc(sizeof(char*) * (strlen((char*)data) + 1));

		if (!(*init_node)->data->string) {
			return NO_MEMORY;
		}

		strcpy((*init_node)->data->string, (char*)data);
	} else {
		(*init_node)->data->node = (node_bin_tree*)data;
	}

	(*init_node)->next = NULL;

	return SUCCESS;
}

int push(stack_t **stack, void *data, int type) {
	node_stack *push_node = NULL;
	int exit_code = init_node_stack(&push_node, data, type);

	if (exit_code != SUCCESS) {
		return exit_code;
	}

	if (!*stack) {
		*stack = (stack_t*)malloc(sizeof(stack_t));

		if (!*stack) {
			free(push_node);
			return NO_MEMORY;
		}

		(*stack)->top = NULL;
		(*stack)->type = type;
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

			if ((*stack)->type == STRING) {
				free(temp->data->string);
			}

			free(temp->data);
			free(temp);
			temp = NULL;
		}
	}

	return SUCCESS;
}

void *pick(stack_t *stack) {
	return stack->type == NODE_BIN_TREE ? (void*)(stack->top->data->node) : (void*)stack->top->data->string;
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