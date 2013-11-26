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
#include "iloc_code.h"

//! Constantes para identificar o tipo de coercao
#define IKS_COERCAO_NENHUMA 	0
#define IKS_COERCAO_INT_FLOAT	1
#define IKS_COERCAO_INT_BOOL	2
#define IKS_COERCAO_FLOAT_INT	3
#define IKS_COERCAO_FLOAT_BOOL	4
#define IKS_COERCAO_BOOL_INT	5
#define IKS_COERCAO_BOOL_FLOAT	6

/**
 * @brief Estrutura da árvore
 *
 * Cada nó da árvore contém um valor associado, chave do nó, ponteiro para uma entrada do dicionário, ponteiro para o pai, para o próximo irmão e para o primeiro filho.
 */
typedef struct _comp_tree_t {
	int type;												/**< Tipo do nó. */
	int value;											/**< Valor associado. */
	comp_dict_item_t *dictPointer;	/**< Ponteiro para uma entrada do dicionário. */
	int tipoCoercao;								/**< Tipo da coercao que deve ser realizada. */
	int resultRegister;							/**< Registrador que armazena o resultado. */
	iloc_code *code;								/**< Código ILOC. */
	struct _comp_tree_t *parent;		/**< Ponteiro para o pai. */
	struct _comp_tree_t *brother;		/**< Ponteiro para o próximo irmão. */
	struct _comp_tree_t *child;			/**< Ponteiro para o primeiro filho. */
} comp_tree_t;


//!  Cria uma nova árvore
void createTree(comp_tree_t **tree);
//!  Imprime a árvore utilizando o caminhamento pré-fixado
void printTree(comp_tree_t *tree);
//!  Destrói a árvore, libera toda a memória associada a ela
void destroyTree(comp_tree_t **tree);
//!  Conta o número de nós caminhando por toda a árvore
int countTreeNodes(comp_tree_t *tree);
//!  Conta as folhas da árvore
int countLeafs(comp_tree_t *tree);
//!  Conta a profundidade da árvore
int countDepth(comp_tree_t *tree);
//!  Testa se a árvore está vazia
int isTreeEmpty(comp_tree_t *tree);
//! Concatena uma árvore como filha de uma raíz
void appendOnChildPointer(comp_tree_t *root, comp_tree_t *tree);
//! Insere um nodo em uma árvore vazia
int insertNode(comp_tree_t **tree, int value);

#endif
