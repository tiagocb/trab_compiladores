#include <stdlib.h>
#include <stdio.h>
#include "comp_list.h"

void createList (comp_list_t **list) {
	*list = NULL;
}

int countListNodes(comp_list_t *list){
	int count = 0;
	comp_list_t *ptAux = list;
	while(ptAux != NULL){
		count++;
		ptAux = ptAux->next;
	}
	return count;
}

int insertHead (comp_list_t **list, void *data) {
	comp_list_t *newNode;
	newNode = malloc(sizeof(comp_list_t));
	if (newNode == NULL) return 1;//couldnt alloc
	newNode->data = data;
	newNode->next = *list;
	*list = newNode;
	return 0;
}

int insertTail (comp_list_t **list, void *data) {
	comp_list_t *newNode;
	newNode = malloc(sizeof(comp_list_t));
	if (newNode == NULL) return 1;//couldnt alloc
	newNode->data = data;
	newNode->next = NULL;

	if (*list == NULL)
		*list = newNode;
	else {
		comp_list_t *ptAux = *list;
		while (ptAux->next != NULL) ptAux = ptAux->next;
		ptAux->next = newNode;
	}
	return 0;
}

int delete (comp_list_t **list, int position) {
	if (*list == NULL) return 1;//cant delete
	if (position < 1) return 2;//invalid position

	comp_list_t *ptAux = *list;
	if (position == 1) {
		*list = (*list)->next;
		free(ptAux);
	} else {
		int deleted = 0;
		int counter = 1;
		while (ptAux->next != NULL) {
			counter++;
			if (counter == position) {
				comp_list_t *targetNode = ptAux->next;
				ptAux->next = targetNode->next;
				free(targetNode);
				deleted = 1;
				break;
			}
			ptAux = ptAux->next;
		}
		if (deleted == 0) return 3;//position out of range
	}
	return 0;
}

int count (comp_list_t *list) {
	comp_list_t *ptAux = list;
	int counter = 0;
	while (ptAux != NULL) {
		counter++;
		ptAux = ptAux->next;
	}
	return counter;
}

void *getFirst(comp_list_t *list) {
	if (list == NULL) return NULL;//empty list
	return list->data;
}

int isListEmpty (comp_list_t *list) {
	return list == NULL;
}

void clearList (comp_list_t **list) {
	comp_list_t *ptAux;
	while (*list != NULL) {
		ptAux = *list;
		*list = (*list)->next;
		free(ptAux);
	}
	*list = NULL;
}

void printList (comp_list_t *list) {
	if (isListEmpty(list)) {
		printf("empty list\n");
		return;
	}

	comp_list_t *ptAux = list;
	int counter = 0;
	while (ptAux != NULL) {
		printf("[%d] data %p\n", counter, ptAux->data);
		counter++;
		ptAux = ptAux->next;
	}
}
