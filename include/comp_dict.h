/*
  comp_dict.h

  Cont�m estrutura de um dicion�rio implementado como uma tabela hash, e opera��es:
  - Criar dicion�rio
  - Procurar por elemento dada uma chave
  - Inserir um elemento e a sua chave associada
  - Atualizar um elemento dada a sua chave
  - Deletar um elemento dada a sua chave
  - Obter o n�mero de elementos armazenados
  - Remover todo o conte�do do dicion�rio
  - Destruir o dicion�rio
  - Imprimir o dicion�rio
*/

#include "common.h"

typedef struct {
	char *key;
	int type;
	union{
		int intValue;
		float floatValue;
		char charValue;
		char *stringValue;
		int boolValue;
	};
	int line;
} comp_dict_item_t;

typedef struct _comp_dict_node_t {
	comp_dict_item_t *item;
	struct _comp_dict_node_t *next;
} comp_dict_node_t;

typedef struct {
	int numberOfLists;
	int numberOfElements;
	comp_dict_node_t **table;
} comp_dict_t;


comp_dict_item_t *searchKey(comp_dict_t dict, char *key);
unsigned int hashFunction(int numberOfLists, char *key);

void clearDictionaryContent(comp_dict_t *dict);
void destroyDictionary(comp_dict_t *dict);
void printDictionary(comp_dict_t dict);

int createDictionaty(comp_dict_t *dict, int numberOfLists);
comp_dict_item_t *insertKey(comp_dict_t *dict, char *key, int type, int line);
int updateKey(comp_dict_t dict, char *key, int newType);
int deleteKey(comp_dict_t *dict, char *key);
int getNumberOfKeys(comp_dict_t dict);

