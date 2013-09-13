/**
 * @file   comp_graph.h
 * @brief  Estrutura de grafo.
 *
 * O grafo � representado por uma lista de adjac�ncia, ou seja, cada nodo tem uma lista de arestas associada.
 * Cada nodo tem um valor associado e cada aresta tem um peso associado.
 */

#ifndef _COMP_GRAPH_H
#define _COMP_GRAPH_H

/**
 * @brief Estrutura da aresta.
 *
 * Cont�m a chave do nodo destino, peso e ponteiro para a pr�xima aresta.
 */
typedef struct _comp_graph_edge {
	int destinyNode;			/**< Chave do nodo destino. */
	int weight; 				/**< Peso. */ 
	struct _comp_graph_edge *nextEdge;	/**< Ponteiro para a pr�xima aresta. */ 
} comp_graph_edge;

/**
 * @brief Estrutura do nodo.
 *
 * Cont�m a chave do nodo, valor associado, lista encadeada de arestas e ponteiro para o pr�ximo nodo.
 */
typedef struct _comp_graph_t {
	int nodeId;				/**< Chave do nodo. */ 
	int value;				/**< Valor associado. */ 
	comp_graph_edge *edgeList;		/**< Lista de arestas. */ 
	struct _comp_graph_t *nextNode;		/**< Ponteiro para o pr�ximo nodo. */ 
} comp_graph_t;


//!  Cria um grafo
void createGraph(comp_graph_t **graph);
//!  Destr�i o grafo, libera toda a mem�ria associada a ele
void destroyGrapth(comp_graph_t **graph);
//!  Imprime o grafo
void printGraph(comp_graph_t *graph);
//!  Testa se o grafo n�o cont�m nodos
int isEmpty(comp_graph_t *graph);
//!  Insere um nodo no grafo com uma chave �nica. Retorna 1 se a opera��o foi bem sucedida e 0 caso contr�rio
int insertNode(comp_graph_t **graph, int nodeId, int value);
//!  Insere uma aresta no grafo dado a chave do nodo de origem e do nodo destino. Retorna 1 se a opera��o foi bem sucedida e 0 caso contr�rio
int insertEdge(comp_graph_t *graph, int sourceNode, int destinyNode);
//!  Remove todas as arestas de um nodo. Retorna 1 se a opera��o foi bem sucedida e 0 caso contr�rio
int removeNodeEdges(comp_graph_t *graph, int nodeId);
//!  Remove um nodo do grafo. Retorna 1 se a opera��o foi bem sucedida e 0 caso contr�rio
int removeNode(comp_graph_t **graph, int nodeId);
//!  Remove uma aresta do grafo. Retorna 1 se a opera��o foi bem sucedida e 0 caso contr�rio
int removeEdge(comp_graph_t *graph, int sourceNodeId, int destinyNodeId);
//!  Atualiza o valor de um nodo. Retorna 1 se a opera��o foi bem sucedida e 0 caso contr�rio
int updateNode(comp_graph_t *graph, int nodeId, int newValue);
//!  Obt�m um vetor com as chaves dos vizinhos de um nodo
int *getNeighbors(comp_graph_t *graph, int nodeId);

#endif
