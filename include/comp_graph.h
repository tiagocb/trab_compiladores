/**
 * @file   comp_graph.h
 * @brief  Estrutura de grafo.
 *
 * O grafo � representado por uma lista de adjac�ncia (cada nodo tem uma lista de arestas associada).
 * Cada nodo e cada aresta possui um ponteiro para algum tipo de dado.
 * O grafo � direcionado.
 */

#ifndef _COMP_GRAPH_H
#define _COMP_GRAPH_H

/**
 * @brief Estrutura da aresta.
 *
 * Cont�m a identifica��o do nodo destino, peso e ponteiro para a pr�xima aresta da lista.
 */
typedef struct _comp_graph_edge {
	int destinyNode;										/**< Identifica��o do nodo destino. */
	void *data; 												/**< Dados. */ 
	struct _comp_graph_edge *nextEdge;	/**< Ponteiro para a pr�xima aresta. */ 
} comp_graph_edge;

/**
 * @brief Estrutura do nodo.
 *
 * Cont�m a identifica��o do nodo, valor associado, lista encadeada de arestas e ponteiro para o pr�ximo nodo da lista.
 */
typedef struct _comp_graph_t {
	int nodeId;											/**< Chave do nodo. */ 
	void *data;											/**< Dados. */ 
	comp_graph_edge *edgeList;			/**< Lista de arestas. */ 
	struct _comp_graph_t *nextNode;	/**< Ponteiro para o pr�ximo nodo. */ 
} comp_graph_t;


//! Cria um grafo
void createGraph(comp_graph_t **graph);
//!  Insere um nodo no grafo.
int insertNode(comp_graph_t **graph, int nodeId, void *data);
//!  Insere uma aresta no grafo dado a chave do nodo de origem e do nodo destino.
int insertEdge(comp_graph_t *graph, int sourceNode, int destinyNode, void *data);
//!  Remove todas as arestas de um nodo.
int removeNodeEdges(comp_graph_t *graph, int nodeId);
//!  Remove um nodo do grafo
int removeNode(comp_graph_t **graph, int nodeId);
//!  Remove uma aresta do grafo
int removeEdge(comp_graph_t *graph, int sourceNodeId, int destinyNodeId);
//!  Obt�m um vetor com as identifica��es dos vizinhos de um nodo
int *getNeighbors(comp_graph_t *graph, int nodeId);
//!  Destr�i o grafo, libera toda a mem�ria associada a ele
void destroyGraph(comp_graph_t **graph);
//!  Retorna o n�mero de nodos do grafo
int countNodes (comp_graph_t *graph);
//!  Retorna o n�mero de arestas do grafo
int countEdges (comp_graph_t *graph);
//!  Testa se o grafo n�o cont�m nodos
int isEmpty(comp_graph_t *graph);
//!  Imprime o grafo
void printGraph(comp_graph_t *graph);

#endif
