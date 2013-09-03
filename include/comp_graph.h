/*
  comp_graph.h

  Cont�m estrutura de um grafo e opera��es de:
  - Criar grafo
  - Obter n�mero de nodos
  - Obter n�mero de arestas
  - Testar se est� vazio
  - Inserir novo nodo
  - Inserir nova aresta
  - Remover todas as arestas de um nodo
  - Remover nodo
  - Remover aresta
  - Atualizar valor de nodo
  - Obter vizinhos de um nodo
  - Destruir grafo
  - Imprimir o seu conte�do
*/

typedef struct _comp_graph_edge {
	int destinyNode;
	struct _comp_graph_edge *nextEdge;
} comp_graph_edge;

typedef struct _comp_graph_t {
	int nodeId;
	int value;
	comp_graph_edge *edgeList;
	struct _comp_graph_t *nextNode;
} comp_graph_t;


void createGraph		(comp_graph_t **graph);
void destroyGrapth		(comp_graph_t **graph);
void printGraph			(comp_graph_t *graph);

int isEmpty				(comp_graph_t *graph);
int insertNode			(comp_graph_t **graph, int nodeId, int value);
int insertEdge			(comp_graph_t *graph, int sourceNode, int destinyNode);
int removeNodeEdges		(comp_graph_t *graph, int nodeId);
int removeNode			(comp_graph_t **graph, int nodeId);
int removeEdge			(comp_graph_t *graph, int sourceNodeId, int destinyNodeId);
int updateNode			(comp_graph_t *graph, int nodeId, int newValue);
int *getNeighbors		(comp_graph_t *graph, int nodeId);
