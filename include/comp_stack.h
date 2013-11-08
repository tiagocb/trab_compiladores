/**
 * @file   comp_stack.h
 * @brief  Estrutura de uma pilha.
 *
 * Cada nodo da pilha é composto por um ponteiro para um tipo de dado qualquer e um ponteiro para o próximo elemento.
 */

#ifndef _COMP_STACK_H
#define _COMP_STACK_H

/**
 * @brief Estrutura da pilha.
 *
 * Cada nodo da lista encadeada contém um ponteiro para um dado qualquer e um ponteiro para o próximo elemento.
 */
typedef struct _comp_stack_node {
	void *data;											/**< Dados associados. */
	struct _comp_stack_node *next;	/**< Ponteiro para o próximo elemento. */
} comp_stack_t;


//!  Cria uma pilha
void createStack(comp_stack_t **stack);
//!  Limpa a pilha, libera toda a memória associada a ela
void clearStack(comp_stack_t **stack);
//!  Conta o número de elementos da pilha
int countStackNodes(comp_stack_t *stack);
//!  Insere um elemento no topo da pilha
int push(comp_stack_t **stack, void *data);
//!  Retorna o dado associado do primeiro elemento da pilha
void *getTop(comp_stack_t *stack);
//!  Testa se a pilha está vazia (não contém nenhum elemento)
int isStackEmpty(comp_stack_t *stack);
//!  Remove o elemento que está no topo da pilha
int pop(comp_stack_t **stack);

#endif
