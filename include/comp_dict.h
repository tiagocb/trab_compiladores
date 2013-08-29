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

typedef struct{
    char *key;
    int value;
    int type;
} comp_dict_item_t;

typedef struct _comp_dict_node_t{
    comp_dict_item_t *item;
    struct _comp_dict_node_t *next;
} comp_dict_node_t;

typedef struct{
    int numberOfLists;
    int numberOfElements;
    comp_dict_node_t **table;
} comp_dict_t;

int createDictionaty(comp_dict_t *dict, int numberOfLists);
unsigned int hashFunction(int numberOfLists, char *key);
comp_dict_item_t *searchKey(comp_dict_t dict, char *key);
int insertKey(comp_dict_t *dict, char *key, int value, int type);
int updateKey(comp_dict_t dict, char *key, int newValue);
int deleteKey(comp_dict_t *dict, char *key);
int getNumberOfKeys(comp_dict_t dict);
void clearDictionaryContent(comp_dict_t *dict);
void destroyDictionary(comp_dict_t *dict);
void printDictionary(comp_dict_t dict);
