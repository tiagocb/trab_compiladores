/**
 * @file   comp_dict.h
 * @brief  Estrutura da tabela de símbolos.
 *
 * A estrutura da tabela de símbolos é uma tabela hash. Ela é composta por listas encadeadas que armazenam os itens.
 * Cada item tem uma chave única e possui um série de valores associados.
 */

#ifndef _COMP_DICT_H
#define _COMP_DICT_H

/* Constantes a serem utilizadas para diferenciar os lexemas que estão registrados na tabela de símbolos. */
#define IKS_SIMBOLO_INT    1
#define IKS_SIMBOLO_FLOAT  2
#define IKS_SIMBOLO_CHAR   3
#define IKS_SIMBOLO_STRING 4
#define IKS_SIMBOLO_BOOL   5
#define IKS_SIMBOLO_INDEFINIDO	6

/**
 * @brief Estrutura do item da tabela de símbolos
 *
 * Contém chave, tipo, valor (inteiro, float, char, string e bool) e a linha.
 */
typedef struct {
	char *key;			/**< Chave. */
	int type;			/**< Tipo. */
	union{
		int intValue;		/**< Valor inteiro. */
		float floatValue;	/**< Valor float. */
		char charValue;		/**< Valor char. */
		char *stringValue;	/**< Valor string. */
		int boolValue;		/**< Valor booleano. */
	};
	int line;			/**< Linha. */
} comp_dict_item_t;

/**
 * @brief Estrutura da lista encadeada de itens da tabela de símbolos
 *
 * Contém um ponteiro para um item e um ponteiro para o próximo nodo da lista encadeada.
 */
typedef struct _comp_dict_node_t {
	comp_dict_item_t *item;		/**< Ponteiro para o item. */
	struct _comp_dict_node_t *next;	/**< Ponteiro para o próximo nodo. */
} comp_dict_node_t;

/**
 * @brief Estrutura da tabela de símbolos
 *
 * Contém um vetor de listas encadeadas de itens, o tamanho deste vetor e o número de itens.
 */
typedef struct {
	int numberOfLists;		/**< Número de listas de itens. */
	int numberOfElements;		/**< Número de itens. */
	comp_dict_node_t **table;	/**< Vetor de listas encadeadas de itens. */
} comp_dict_t;


//!  Dado uma chave, procura por ela na tabela de símbolos. Se a encontrou, retorna o item qua a contém, caso contrário, retorna NULL.
comp_dict_item_t *searchKey(comp_dict_t dict, char *key);
//!  Função hash para selecionar uma lista para inserir/procurar um item.
unsigned int hashFunction(int numberOfLists, char *key);
//!  Remove todos os itens da tabela de símbolos.
void clearDictionaryContent(comp_dict_t *dict);
//!  Destrói a tabela de símbolos, libera toda a memória associada a ela.
void destroyDictionary(comp_dict_t *dict);
//!  Imprime a tabela de símbolos.
void printDictionary(comp_dict_t dict);
//!  Cria a tabela de símbolos com um dado número de listas de itens. Este número é fixo durante o tempo de vida da tabela. Retorna 1 se conseguiu criar e 0 caso contrário.
int createDictionaty(comp_dict_t *dict, int numberOfLists);
//!  Insere um novo item na tabela, a chave dele deve ser única. Se conseguir inserir, retorna o ponteiro para o item, caso contrário, retorna o ponteiro do item que já tinha sido inserido.
comp_dict_item_t *insertKey(comp_dict_t *dict, char *key, int type, int line);
//!  Atualiza o campo de tipo de um item, retorna 1 se conseguiu atualizar e 0 caso contrário.
int updateKey(comp_dict_t dict, char *key, int newType);
//!  Remove um item da tabela, retorna 1 se conseguiu remover e 0 caso contrário.
int deleteKey(comp_dict_t *dict, char *key);
//!  Retorna o número de itens da tabela de símbolos.
int getNumberOfKeys(comp_dict_t dict);

#endif
