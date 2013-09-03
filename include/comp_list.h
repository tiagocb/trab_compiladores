/*
  comp_list.h

  Contém estrutura de uma lista encadeada e operações de:
  - Criar lista
  - Inserir no fim da lista
  - Deletar um elemento dada a sua posição
  - Obter o primeiro elemento
  - Testar se está vazia
  - Atualizar um elemento dada a sua posição
  - Destruir a lista
  - Imprimir o seu conteúdo
*/

typedef struct _comp_list_node comp_list_t;
struct _comp_list_node {
	int value;
	comp_list_t *next;
};

void createList		(comp_list_t **list);
void clearList		(comp_list_t **list);
void printList		(comp_list_t *list);

int insertTail		(comp_list_t **list, int value);
int delete			(comp_list_t **list, int position);
int getFirst		(comp_list_t *list);
int isListEmpty		(comp_list_t *list);
int update			(comp_list_t *list, int position, int newValue);
