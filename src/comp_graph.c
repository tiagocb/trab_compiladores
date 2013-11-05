#include <stdlib.h>
#include <stdio.h>
#include "comp_graph.h"

void createGraph (comp_graph_t **graph) {
	*graph = NULL;
}

int insertNode (comp_graph_t **graph, int nodeId, int value) {
	//check if nodeId is available
	comp_graph_t *ptAuxNode = *graph;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == nodeId) return 1;//nodeId already exists
		ptAuxNode = ptAuxNode->nextNode;
	}

	//create new node
	comp_graph_t *newNode;
	newNode = malloc(sizeof(comp_graph_t));
	if (newNode == NULL) return 2;//couldnt alloc
	newNode->edgeList = NULL;
	newNode->nextNode = NULL;
	newNode->nodeId = nodeId;
	newNode->value = value;

	//insert node
	if (*graph == NULL) {
		*graph = newNode;
	} else {
		ptAuxNode = *graph;
		while (ptAuxNode->nextNode != NULL) ptAuxNode = ptAuxNode->nextNode;
		ptAuxNode->nextNode = newNode;
	}
	return 0;
}

int insertEdge (comp_graph_t *graph, int sourceNode, int destinyNode) {
	//check if sourceNode and destinyNode exist
	comp_graph_t *ptAuxNode = graph;
	int sourceNodeFound = 0, destinyNodeFound = 0;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == sourceNode) sourceNodeFound = 1;
		if (ptAuxNode->nodeId == destinyNode) destinyNodeFound = 1;
		ptAuxNode = ptAuxNode->nextNode;
	}
	if (sourceNodeFound == 0 || destinyNodeFound == 0) return 1;//couldnt find nodes

	//create new edge
	comp_graph_edge *newEdge;
	newEdge = malloc(sizeof(comp_graph_edge));
	if (newEdge == NULL) return 2;//couldnt alloc
	newEdge->destinyNode = destinyNode;
	newEdge->nextEdge = NULL;

	ptAuxNode = graph;
	comp_graph_edge *ptAuxEdge;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == sourceNode) {
			ptAuxEdge = ptAuxNode->edgeList;
			if (ptAuxEdge == NULL) {
				ptAuxNode->edgeList = newEdge;
				return 0;
			}
			while (ptAuxEdge->nextEdge != NULL) ptAuxEdge = ptAuxEdge->nextEdge;
			ptAuxEdge->nextEdge = newEdge;
			return 0;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return 2;//shouldnt reach this line
}

int removeNodeEdges (comp_graph_t *graph, int nodeId) {
	comp_graph_t *ptAuxNode = graph;
	comp_graph_edge *ptAuxEdge, *ptAuxEdge2;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == nodeId) {
			ptAuxEdge = ptAuxNode->edgeList;
			while (ptAuxEdge != NULL) {
				ptAuxEdge2 = ptAuxEdge;
				ptAuxEdge = ptAuxEdge->nextEdge;
				free(ptAuxEdge2);
			}
			ptAuxNode->edgeList = NULL;
			return 0;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return 1;//couldnt find node
}

int removeNode (comp_graph_t **graph, int nodeId) {
	//remove incident edges of the node
	comp_graph_t *ptAuxNode = *graph;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId != nodeId)
			removeEdge(*graph, ptAuxNode->nodeId, nodeId);
		ptAuxNode = ptAuxNode->nextNode;
	}

	//remove all node's edges
	removeNodeEdges(*graph, nodeId);

	//remove node
	ptAuxNode = *graph;
	comp_graph_t *ptAuxNode2;
	if (ptAuxNode->nodeId == nodeId) {
		*graph = ptAuxNode->nextNode;
		free(ptAuxNode);
		return 0;
	}

	while (ptAuxNode->nextNode != NULL) {
		if (ptAuxNode->nextNode->nodeId == nodeId) {
			ptAuxNode2 = ptAuxNode->nextNode;
			ptAuxNode->nextNode = ptAuxNode->nextNode->nextNode;
			free(ptAuxNode2);
			return 0;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return 1;//couldnt find node
}

int removeEdge (comp_graph_t *graph, int sourceNodeId, int destinyNodeId) {
	comp_graph_t *ptAuxNode = graph;
	comp_graph_edge *ptAuxEdge, *ptAuxEdge2;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == sourceNodeId) {
			ptAuxEdge = ptAuxNode->edgeList;

			if (ptAuxEdge == NULL) return 1;//couldnt find edge
			if (ptAuxEdge->destinyNode == destinyNodeId) {
				ptAuxNode->edgeList = ptAuxEdge->nextEdge;
				free(ptAuxEdge);
				return 0;
			}

			while (ptAuxEdge->nextEdge != NULL) {
				if (ptAuxEdge->nextEdge->destinyNode == destinyNodeId) {
					ptAuxEdge2 = ptAuxEdge->nextEdge;
					ptAuxEdge->nextEdge = ptAuxEdge->nextEdge->nextEdge;
					free(ptAuxEdge2);
					return 0;
				}
				ptAuxEdge = ptAuxEdge->nextEdge;
			}
			return 1;//couldnt find edge

		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return 2;//couldnt find node
}

int updateNode (comp_graph_t *graph, int nodeId, int newValue) {
	comp_graph_t *ptAuxNode = graph;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == nodeId) {
			ptAuxNode->value = newValue;
			return 0;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return 1;//couldnt find node
}

int *getNeighbors (comp_graph_t *graph, int nodeId) {
	int *neighbors;

	comp_graph_t *ptAuxNode = graph;
	comp_graph_edge *ptAuxEdge;
	while (ptAuxNode != NULL) {
		if (ptAuxNode->nodeId == nodeId) {
			int counter = 0;
			ptAuxEdge = ptAuxNode->edgeList;
			while (ptAuxEdge != NULL) {
				counter++;
				ptAuxEdge = ptAuxEdge->nextEdge;
			}
			neighbors = malloc((sizeof(int) * counter));
			if (neighbors == NULL && counter > 0) return NULL;//couldnt alloc 
			if (counter == 0) return NULL;//no neighbors

			int index = 0;
			ptAuxEdge = ptAuxNode->edgeList;
			while (ptAuxEdge != NULL) {
				neighbors[index++] = ptAuxEdge->destinyNode;
				ptAuxEdge = ptAuxEdge->nextEdge;
			}
			return neighbors;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return NULL;//nodeId not found
}

void destroyGraph (comp_graph_t **graph) {
	comp_graph_t *ptAuxNode = *graph, *ptAuxNode2;
	comp_graph_edge *ptAuxEdge, *ptAuxEdge2;
	while (ptAuxNode != NULL) {
		ptAuxEdge = ptAuxNode->edgeList;
		while (ptAuxEdge != NULL) {
			ptAuxEdge2 = ptAuxEdge;
			ptAuxEdge = ptAuxEdge->nextEdge;
			free(ptAuxEdge2);
		}
		ptAuxNode2 = ptAuxNode;
		ptAuxNode = ptAuxNode->nextNode;
		free(ptAuxNode2);
	}
	*graph = NULL;
}

int countNodes (comp_graph_t *graph) {
	int counter = 0;
	comp_graph_t *ptAuxNode = graph;
	while (ptAuxNode != NULL) {
		counter++;
		ptAuxNode = ptAuxNode->nextNode;
	}
	return counter;
}

int countEdges (comp_graph_t *graph) {
	int counter = 0;
	comp_graph_t *ptAuxNode = graph;
	comp_graph_edge *ptAuxEdge;
	while (ptAuxNode != NULL) {
		ptAuxEdge = ptAuxNode->edgeList;
		while (ptAuxEdge != NULL) {
			counter++;
			ptAuxEdge = ptAuxEdge->nextEdge;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	return counter;
}

int isEmpty (comp_graph_t *graph) {
	return countNodes(graph) == 0;
}

void printGraph (comp_graph_t *graph) {
	if (isEmpty(graph)) {
		printf("empty graph\n\n\n");
		return;
	}
	comp_graph_t *ptAuxNode = graph;
	comp_graph_edge *ptAuxEdge;
	while (ptAuxNode != NULL) {
		printf("%d: %d\n", ptAuxNode->nodeId, ptAuxNode->value);
		ptAuxEdge = ptAuxNode->edgeList;
		while (ptAuxEdge != NULL) {
			printf("\t%d -> %d\n", ptAuxNode->nodeId, ptAuxEdge->destinyNode);
			ptAuxEdge = ptAuxEdge->nextEdge;
		}
		ptAuxNode = ptAuxNode->nextNode;
	}
	printf("%d nodes\t %d edges\n\n\n", countNodes(graph), countEdges(graph));
}
