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

int containsValue (comp_tree_t *tree, int value) {
	if (tree == NULL) return 0;

	comp_tree_t *ptAux = tree;

	int valueFound = 0;
	while (ptAux != NULL) {
		if(ptAux->value == value) return 1;
		valueFound = containsValue(ptAux->child, value) | valueFound;
		ptAux = ptAux->brother;
	}

	return valueFound;
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

/* Check if tree is empty */
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

/* Check if tree is empty */
int isTreeEmpty (comp_tree_t *tree) {
	if (tree == NULL) return 1;
	return 0;
}

comp_tree_t *getKeyNode (comp_tree_t *tree, int key) {
	if (tree == NULL) return NULL;

	comp_tree_t *ptAux = tree;

	while (ptAux != NULL) {
		if (ptAux->key == key) return ptAux;
		comp_tree_t *ptTmp = getKeyNode(ptAux->child, key);
		if (ptTmp != NULL) return ptTmp;
		ptAux = ptAux->brother;
	}

	return NULL;
}

int countChild (comp_tree_t *tree, int key) {
	comp_tree_t *node = getKeyNode(tree, key);
	if (node == NULL) return -1;
	if (node->child == NULL) return 0;

	int counter = 0;
	comp_tree_t *child = node->child;
	while(child != NULL){
		child = child->brother;
		counter++;
	}

	return counter;
}

/* Check if tree contains key */
int containsKey (comp_tree_t *tree, int key) {
	if (tree == NULL) return 0;

	comp_tree_t *ptAux = tree;

	int keyFound = 0;
	while (ptAux != NULL) {
		if (ptAux->key == key) return 1;
		keyFound = containsKey(ptAux->child, key) | keyFound;
		ptAux = ptAux->brother;
	}

	return keyFound;
}

/* Insert node with unique key */
int insert (comp_tree_t **tree, int value, int key, int parentKey) {
	//if parent node has value = 0, and tree is empty, creates root node
	if (parentKey == 0 && *tree == NULL) {
		comp_tree_t *newNode;
		newNode = malloc(sizeof(comp_tree_t));
		if (newNode == NULL) return 2;//couldnt alloc
		newNode->key = key;
		newNode->value = value;
		newNode->parent = NULL;
		newNode->child = NULL;
		newNode->brother = NULL;
		*tree = newNode;
		return 0;
	}

	//check if key already exists
	if (containsKey(*tree, key)) return 1;//key already exists

	//get parent node pointer
	comp_tree_t *parentNode = getKeyNode(*tree, parentKey);
	if (parentNode == NULL) return 2;//couldnt find parent

	//create node
	comp_tree_t *newNode;
	newNode = malloc(sizeof(comp_tree_t));
	if (newNode == NULL) return 3;//couldnt alloc
	newNode->key = key;
	newNode->value = value;
	newNode->parent = parentNode;
	newNode->child = NULL;
	newNode->brother = NULL;

	//insert node
	comp_tree_t *ptAux = parentNode->child;
	if (ptAux == NULL) {
		parentNode->child = newNode;
		return 0;
	}
	while (ptAux->brother != NULL) ptAux = ptAux->brother;
	ptAux->brother = newNode;
	return 0;
}

/* Update some node's value */
int updateValue (comp_tree_t *tree, int key, int newValue) {
	if (tree == NULL) return 1;

	comp_tree_t *ptAux = tree;

	while (ptAux != NULL) {
		if (ptAux->key == key) {
			ptAux->value = newValue;
			return 0;
		}
		if (updateValue(ptAux->child, key, newValue) == 0) return 0;
		ptAux = ptAux->brother;
	}
	return 1;
}

/* Destroy tree */
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

/* Remove node and all his child */
int removeTreeNode (comp_tree_t **tree, int keyNode) {
	if (*tree == NULL) return 1;

	comp_tree_t *ptAux = *tree, *ptAux2 = NULL, *ptParent;
	while (ptAux != NULL) {
		if (ptAux->key == keyNode) {
			if (ptAux->parent == NULL) return 0;
			ptParent = ptAux->parent;
			if (ptAux2 == NULL) ptParent->child = ptAux->brother;
			else ptAux2->brother = ptAux->brother;
			destroyTree(&(ptAux->child));
			free(ptAux);
			return 0;
		}
		if (removeTreeNode(&(ptAux->child), keyNode) == 0) return 0;
		ptAux2 = ptAux;
		ptAux = ptAux->brother;
	}

	return 1;
}

/* Print tree */
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

void appendOnBrotherPointer(comp_tree_t *root, comp_tree_t *tree){
	if(tree == NULL) return;

	comp_tree_t *brother = root->brother;
	if(brother == NULL){
		root->brother = tree;
	}else{
		while(brother->brother != NULL) brother = brother->brother;
		brother->brother = tree;
	}
}





