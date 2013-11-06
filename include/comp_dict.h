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
	char *key;										/**< Chave. (VARIAVEL, VETOR, FUNCAO, LITERAL) */
	int valueType;								/**< Tipo do valor. (VARIAVEL, VETOR, FUNCAO, LITERAL) */
	int nodeType;									/**< Tipo do nodo. (VARIAVEL, VETOR, LITERAL, FUNCAO) */
	int line;											/**< Linha. (VARIAVEL, VETOR, FUNCAO, LITERAL) */
	union{
		int intValue;								/**< Valor inteiro. (LITERAL) */
		float floatValue;						/**< Valor float. (LITERAL) */
		char charValue;							/**< Valor char. (LITERAL) */
		char *stringValue;					/**< Valor string. (LITERAL) */
		int boolValue;							/**< Valor booleano. (LITERAL) */
	};
	int numBytes;									/**< N�mero de bytes. (VARIAVEL, VETOR, LITERAL, FUNCAO) */
	void *functionSymbolTable;		/**< Ponteiro para tabela de s�mbolos da fun��o. (FUNCAO) */
	comp_list_t *parametersList;	/**< Lista dos tipos dos parametros. (FUNCAO) */
	comp_list_t *dimensionList;		/**< Lista dos tamanhos das dimens�es de um vetor multidimensional. (VETOR) */
	int address;									/**< Endere�o. (VARIAVEL, VETOR) */
} comp_dict_item_t;

/**
 * @brief Estrutura da lista encadeada de itens da tabela de s�mbolos.
 *
 * Cont�m um ponteiro para um item e um ponteiro para o pr�ximo nodo da lista encadeada.
 */
typedef struct _comp_dict_node_t {
	comp_dict_item_t *item;					/**< Ponteiro para o item. */
	struct _comp_dict_node_t *next;	/**< Ponteiro para o pr�ximo nodo. */
} comp_dict_node_t;

/**
 * @brief Estrutura da tabela de s�mbolos
 *
 * Cont�m um vetor de listas encadeadas de itens, o tamanho deste vetor, o n�mero de itens armazenados e um ponteiro para uma tabela pai (implementa��o de escopo aninhado).
 */
typedef struct _comp_dict_t {
	int numberOfLists;						/**< N�mero de listas de itens. */
	int numberOfElements;					/**< N�mero de itens. */
	comp_dict_node_t **table;			/**< Vetor de listas encadeadas de itens. */
	struct _comp_dict_t *parent;	/**< Ponteiro para tabela pai. */
} comp_dict_t;

//!  Cria a tabela de s�mbolos com um dado n�mero de listas de itens. Este n�mero � fixo durante o tempo de vida da tabela
int createDictionaty(comp_dict_t *dict, int numberOfLists, comp_dict_t *parent);
//!  Retorna o n�mero de itens da tabela de s�mbolos
int getNumberOfKeys(comp_dict_t dict);
//!  Fun��o hash para selecionar uma lista para inserir/procurar um item
unsigned int hashFunction(int numberOfLists, char *key);
//!  Dado uma chave, procura por ela na tabela de s�mbolos
comp_dict_item_t *searchKey(comp_dict_t dict, char *key);
//!  Insere um novo item na tabela (a chave dele deve ser �nica). Se conseguir inserir, retorna o ponteiro para o item, caso contr�rio, retorna o ponteiro do item que j� tinha sido inserido
comp_dict_item_t *insertKey(comp_dict_t *dict, char *key, int valueType, int line);
//!  Atualiza o campo de tipo de um item
int updateKey(comp_dict_t dict, char *key, int newValueType);
//!  Remove um item da tabela
int deleteKey(comp_dict_t *dict, char *key);
//!  Remove todos os itens da tabela de s�mbolos
void clearDictionaryContent(comp_dict_t *dict);
//!  Destr�i a tabela de s�mbolos, libera toda a mem�ria associada a ela
void destroyDictionary(comp_dict_t *dict);
//!  Imprime a tabela de s�mbolos
void printDictionary(comp_dict_t dict);

#endif
