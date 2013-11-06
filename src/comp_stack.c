#include <stdlib.h>
#include <stdio.h>
#include "comp_stack.h"

void createStack(comp_stack_t **stack){
	*stack = NULL;
}

int countStackNodes(comp_stack_t *stack){
	int counter = 0;
	comp_stack_t *ptAux = stack;
	while(ptAux != NULL){
		counter++;
		ptAux = ptAux->next;
	}
	return counter;
}

int push(comp_stack_t **stack, void *data){
	comp_stack_t *newNode;
	newNode = malloc(sizeof(comp_stack_t));
	if(newNode == NULL) return 1;//couldnt alloc
	newNode->data = data;
	newNode->next = *stack;
	*stack = newNode;
	return 0;
}

void *getTop(comp_stack_t *stack){
	if(stack == NULL) return NULL;//empty stack
	return stack->data;
}

int isStackEmpty(comp_stack_t *stack){
	return stack == NULL;
}

void clearStack(comp_stack_t **stack){
	comp_stack_t *ptAux;
	while(*stack != NULL) {
		ptAux = *stack;
		*stack = (*stack)->next;
		free(ptAux);
	}
	*stack = NULL;
}

int pop(comp_stack_t **stack){
	if(*stack == NULL) return 1;//empty stack
	comp_stack_t *ptAux = *stack;
	*stack = (*stack)->next;
	free(ptAux);
	return 0;
}
