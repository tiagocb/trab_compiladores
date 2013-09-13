/**
 * @file   comp_tree.h
 * @brief  Estrutura de árvore.
 *
 * A estrutura da árvore aceita um número arbitrário de filhos para um mesmo nó.
 * Cada nó tem uma chave única e um valor associado.
 */

#ifndef _COMP_TREE_H
#define _COMP_TREE_H
#include "comp_dict.h"

/**
 * @brief Estrutura da árvore
 *
 * Cada nó da árvore contém um valor associado, chave do nó, ponteiro para uma entrada do dicionário, ponteiro para o pai, para o próximo irmão e para o primeiro filho.
 */
typedef struct _comp_tree_t {
	int key;			/**< Chave do nó. */
	int value;			/**< Valor associado. */
	comp_dict_item_t *dictPointer;	/**< Ponteiro para uma entrada do dicionário. */
	struct _comp_tree_t *parent;	/**< Ponteiro para o pai. */
	struct _comp_tree_t *brother;	/**< Ponteiro para o próximo irmão. */
	struct _comp_tree_t *child;	/**< Ponteiro para o primeiro filho. */
} comp_tree_t;


//!  Dado uma chave, retorna um ponteiro para o nodo que possui a chave especificada. Se não encontrar, retorna NULL.
comp_tree_t *getKeyNode(comp_tree_t *tree, int key);
//!  Cria uma nova árvore.
void createTree(comp_tree_t **tree);
//!  Imprime a árvore utilizando o caminhamento pré-fixado.
void printTree(comp_tree_t *tree);
//!  Destrói a árvore, libera toda a memória associada a ela.
void destroyTree(comp_tree_t **tree);
//!  Conta o número de nós caminhando por toda a árvore.
int countTreeNodes(comp_tree_t *tree);
//!  Testa se algum nó da árvore possui o valor especificado.
int containsValue(comp_tree_t *tree, int value);
//!  Conta as folhas da árvore.
int countLeafs(comp_tree_t *tree);
//!  Conta a profundidade da árvore.
int countDepth(comp_tree_t *tree);
//!  Testa se a árvore está vazia.
int isTreeEmpty(comp_tree_t *tree);
//!  Conta o número de filhos de um nó.
int countChild (comp_tree_t *tree, int key);
//!  Testa se a árvore possui um nó com a chave especificada.
int containsKey(comp_tree_t *tree, int key);
//!  Dado uma chave, insere um novo nó que será filho do nó com a chave especificada.
int insert(comp_tree_t **tree, int value, int key, int parentKey);
//!  Atualiza o valor associado de um nó, dado sua chave.
int updateValue(comp_tree_t *tree, int key, int newValue);
//!  Remove um nó da árvore dado a sua chave, os filhos deste nó também são removidos.
int removeTreeNode(comp_tree_t **tree, int keyNode);
//! Concatena uma árvore como filha de uma raíz.
void appendOnChildPointer(comp_tree_t *root, comp_tree_t *tree);
//! Concatena uma árvore como irmã de uma raíz.
void appendOnBrotherPointer(comp_tree_t *root, comp_tree_t *tree);

#endif
