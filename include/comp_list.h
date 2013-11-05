/**
 * @file   comp_list.h
 * @brief  Estrutura de lista encadeada.
 *
 * Cada nodo da lista é composto pelo seu valor e um ponteiro para o próximo elemento.
 */

#ifndef _COMP_LIST_H
#define _COMP_LIST_H

/**
 * @brief Estrutura da lista encadeada.
 *
 * Cada nodo da lista encadeada contém um valor associado e um ponteiro para o próximo elemento.
 */
typedef struct _comp_list_node {
	int value;										/**< Valor associado. */
	struct _comp_list_node *next;	/**< Ponteiro para o próximo elemento. */
} comp_list_t;


//!  Cria uma lista
void createList(comp_list_t **list);
//!  Limpa a lista, libera toda a memória associada a lista
void clearList(comp_list_t **list);
//!  Conta o número de elementos da lista
int countListNodes(comp_list_t *list);
//!  Imprime a lista
void printList(comp_list_t *list);
//!  Insere um elemento no início da lista
int insertHead (comp_list_t **list, int value);
//!  Insere um elemento no fim da lista
int insertTail(comp_list_t **list, int value);
//!  Deleta um elemento em uma posição específica
int delete(comp_list_t **list, int position);
//!  Retorna o valor associado do primeiro elemento da lista
int getFirst(comp_list_t *list);
//!  Testa se a lista está vazia (não contém nenhum elemento)
int isListEmpty(comp_list_t *list);
//!  Atualiza o valor associado de um elemento da lista dado a sua posição
int update(comp_list_t *list, int position, int newValue);
//!  Compara duas listas. Retorna 0 se são iguais e 1 se são diferentes
int compare(comp_list_t *list1, comp_list_t *list2);
//!  Multiplica todos os valores da lista
int multiplyAll(comp_list_t *list);

#endif
