/**
 * @file   comp_dict.h
 * @brief  Estrutura da tabela de s�mbolos.
 *
 * A estrutura da tabela de s�mbolos � uma tabela hash. Ela � composta por listas encadeadas que armazenam os itens.
 * Cada item tem uma chave �nica e possui um s�rie de valores associados.
 */

#ifndef _COMP_DICT_H
#define _COMP_DICT_H

#include "comp_list.h"

//! Tipos dos valores dos elementos da tabela de s�mbolos
#define IKS_INT			1
#define IKS_FLOAT  		2
#define IKS_CHAR   		3
#define IKS_STRING 		4
#define IKS_BOOL   		5
#define IKS_UNDEFINED	6

//! Tamanhos dos tipos dos elementos da tabela de s�mbolos
#define IKS_INT_SIZE	4
#define IKS_FLOAT_SIZE	8
#define IKS_CHAR_SIZE	1
#define IKS_STRING_SIZE	1
#define IKS_BOOL_SIZE	1

//! Tipos dos identificadores dos elementos da tabela de s�mbolos
#define IKS_VARIABLE_ITEM  	0
#define IKS_VECTOR_ITEM		1
#define IKS_FUNCTION_ITEM  	2
#define IKS_LITERAL_ITEM	3
#define IKS_UNDEFINED_ITEM  4

/**
 * @brief Estrutura do item da tabela de s�mbolos.
 *
 * Cont�m chave, tipo do valor, tipo do identificador, linha, valor (caso for literal), n�mero de bytes (caso for vari�vel) e ponteiro para outra tabela de s�mbolos (caso for fun��o).
 */
typedef struct {
	char *key;			/**< Chave. */
	int valueType;		/**< Tipo do valor. */
	int nodeType;		/**< Tipo do nodo. */
	int line;			/**< Linha. */
	union{
		int intValue;				/**< Valor inteiro. */
		float floatValue;			/**< Valor float. */
		char charValue;				/**< Valor char. */
		char *stringValue;			/**< Valor string. */
		int boolValue;				/**< Valor booleano. */
	};
	int numBytes;					/**< N�mero de bytes. */
	void *functionSymbolTable;		/**< Ponteiro para tabela de s�mbolos da fun��o. */
	comp_list_t *parametersList;
} comp_dict_item_t;

/**
 * @brief Estrutura da lista encadeada de itens da tabela de s�mbolos.
 *
 * Cont�m um ponteiro para um item e um ponteiro para o pr�ximo nodo da lista encadeada.
 */
typedef struct _comp_dict_node_t {
	comp_dict_item_t *item;			/**< Ponteiro para o item. */
	struct _comp_dict_node_t *next;	/**< Ponteiro para o pr�ximo nodo. */
} comp_dict_node_t;

/**
 * @brief Estrutura da tabela de s�mbolos
 *
 * Cont�m um vetor de listas encadeadas de itens, o tamanho deste vetor, o n�mero de itens armazenados e um ponteiro para uma tabela pai (implementa��o de escopo aninhado).
 */
typedef struct _comp_dict_t {
	int numberOfLists;				/**< N�mero de listas de itens. */
	int numberOfElements;			/**< N�mero de itens. */
	comp_dict_node_t **table;		/**< Vetor de listas encadeadas de itens. */
	struct _comp_dict_t *parent;	/**< Ponteiro para tabela pai. */
} comp_dict_t;

//!  Cria a tabela de s�mbolos com um dado n�mero de listas de itens. Este n�mero � fixo durante o tempo de vida da tabela. Retorna 1 se conseguiu criar e 0 caso contr�rio.
int createDictionaty(comp_dict_t *dict, int numberOfLists, comp_dict_t *parent);
//!  Retorna o n�mero de itens da tabela de s�mbolos.
int getNumberOfKeys(comp_dict_t dict);
//!  Fun��o hash para selecionar uma lista para inserir/procurar um item.
unsigned int hashFunction(int numberOfLists, char *key);
//!  Dado uma chave, procura por ela na tabela de s�mbolos. Se a encontrou, retorna o item qua a cont�m, caso contr�rio, retorna NULL.
comp_dict_item_t *searchKey(comp_dict_t dict, char *key);
//!  Insere um novo item na tabela (a chave dele deve ser �nica). Se conseguir inserir, retorna o ponteiro para o item, caso contr�rio, retorna o ponteiro do item que j� tinha sido inserido.
comp_dict_item_t *insertKey(comp_dict_t *dict, char *key, int valueType, int line);
//!  Atualiza o campo de tipo de um item, retorna 1 se conseguiu atualizar e 0 caso contr�rio.
int updateKey(comp_dict_t dict, char *key, int newValueType);
//!  Remove um item da tabela, retorna 1 se conseguiu remover e 0 caso contr�rio.
int deleteKey(comp_dict_t *dict, char *key);
//!  Remove todos os itens da tabela de s�mbolos.
void clearDictionaryContent(comp_dict_t *dict);
//!  Destr�i a tabela de s�mbolos, libera toda a mem�ria associada a ela.
void destroyDictionary(comp_dict_t *dict);
//!  Imprime a tabela de s�mbolos.
void printDictionary(comp_dict_t dict);

#endif
