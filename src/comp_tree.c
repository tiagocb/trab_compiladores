#include <stdio.h>
#include <stdlib.h>
#include "comp_tree.h"

void createTree (comp_tree_t **tree) {
	*tree = NULL;
}

int countTreeNodes (comp_tree_t *tree) {
	if (tree == NULL) return 0;

	comp_tree_t *ptAux = tree;

	int counter = 0;
	while (ptAux != NULL) {
		counter += countTreeNodes(ptAux->child);
		counter++;
		ptAux = ptAux->brother;
	}

	return counter;
}

int countLeafs (comp_tree_t *tree) {
	if (tree == NULL) return 0;

	comp_tree_t *ptAux = tree;

	int counter = 0;
	while (ptAux != NULL) {
		if (ptAux->child == NULL) counter++;
		else counter += countLeafs(ptAux->child);
		ptAux = ptAux->brother;
	}

	return counter;
}

int countDepth (comp_tree_t *tree) {
	if (tree == NULL) return 0;

	comp_tree_t *ptAux = tree;

	int max = 0;
	while (ptAux != NULL) {
		int tmp = countDepth(ptAux->child);
		if (1 + tmp > max) max = 1 + tmp;
		ptAux = ptAux->brother;
	}
	return max;
}

int isTreeEmpty (comp_tree_t *tree) {
	if (tree == NULL) return 1;
	return 0;
}

void destroyTree (comp_tree_t **tree) {
	if (*tree == NULL) return;
	comp_tree_t *ptAux = *tree, *ptAux2;
	while (ptAux != NULL) {
		destroyTree(&(ptAux->child));
		ptAux2 = ptAux;
		ptAux = ptAux->brother;
		free(ptAux2);
	}
	*tree = NULL;
}

void printTree (comp_tree_t *tree) {
	if (isTreeEmpty(tree)){
		return;
	}

	comp_tree_t *ptAux = tree;
	while (ptAux != NULL) {
		printf("%d ", ptAux->value);
		printTree(ptAux->child);
		ptAux = ptAux->brother;
	}
}

void appendOnChildPointer(comp_tree_t *root, comp_tree_t *tree){
	if(tree == NULL) return;

	comp_tree_t *child = root->child;
	if(child == NULL){
		root->child = tree;
	}else{
		while(child->brother != NULL) child = child->brother;
		child->brother = tree;
	}

	comp_tree_t *newChild = tree;
	while(newChild != NULL){
		newChild->parent = root;
		newChild = newChild->brother;
	}
}

int insert(comp_tree_t **tree, int value){
	comp_tree_t *newNode;
	newNode = malloc(sizeof(comp_tree_t));
	if(newNode == NULL) return 1;//couldnt alloc
	newNode->value = value;
	newNode->tipoCoercao = IKS_COERCAO_NENHUMA;
	newNode->parent = NULL;
	newNode->child = NULL;
	newNode->brother = NULL;
	newNode->code = NULL;
	*tree = newNode;
	return 0;
}

