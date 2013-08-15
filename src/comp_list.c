#include <stdlib.h>
#include <stdio.h>
#include "comp_list.h"

/* create list */
void createList(comp_list_t **list){
	*list = NULL;
}

/* insert node at the tail of the list */
int insertTail(comp_list_t **list, int value){
	comp_list_t *newNode;
	newNode = malloc(sizeof(comp_list_t));
	if(newNode == NULL) return 1;//couldnt alloc
	newNode->value = value;
	newNode->next = NULL;

	if(*list == NULL)
		*list = newNode;
	else{
		comp_list_t *ptAux = *list;
		while(ptAux->next != NULL) ptAux = ptAux->next;
		ptAux->next = newNode;
	}
	return 0;
}

/* delete node at the specified position */
int delete(comp_list_t **list, int position){
	if(*list == NULL) return 1;//cant delete
	if(position < 1) return 2;//invalid position

	comp_list_t *ptAux = *list;
	if(position == 1){
		*list = (*list)->next;
		free(ptAux);
	}
	else{
		int deleted = 0;
		int counter = 1;
		while(ptAux->next != NULL){
			counter++;
			if(counter == position){
				comp_list_t *targetNode = ptAux->next;
				ptAux->next = targetNode->next;
				free(targetNode);
				deleted = 1;
				break;
			}
			ptAux = ptAux->next;
		}
		if(deleted == 0) return 3;//position out of range
	}
	return 0;
}

/* count the number of nodes */
int count(comp_list_t *list){
	comp_list_t *ptAux = list;
	int counter = 0;
	while(ptAux != NULL){
		counter++;
		ptAux = ptAux->next;
	}
	return counter;
}

/* return first value of the list */
int getFirst(comp_list_t *list){
	if(list == NULL) return -1;//empty list
	return list->value;
}

/* return 1 if list is empty, 0 otherwise */
int isListEmpty(comp_list_t *list){
	return list == NULL;
}

/* update node at certain position */
int update(comp_list_t *list, int position, int newValue){
	if(list == NULL) return 1;//empty list
	if(position < 1) return 2;//invalid position

	comp_list_t *ptAux = list;
	int updated = 0;
	int counter = 1;
	while(ptAux != NULL){
		if(counter == position){
			ptAux->value = newValue;
			updated = 1;
			break;
		}
		counter++;
		ptAux = ptAux->next;
	}
	if(updated == 0) return 3;//position out of range
	return 0;
}

/* delete all list content */
void clearList(comp_list_t **list){
	comp_list_t *ptAux;
	while(*list != NULL){
		ptAux = *list;
		*list = (*list)->next;
		free(ptAux);
	}
	*list = NULL;
}

/* print list nodes */
void printList(comp_list_t *list){
	if(isListEmpty(list)){
		printf("empty list\n\n");
		return;
	}

	printf("Number of nodes: %d\n", count(list));
	
	comp_list_t *ptAux = list;
	int counter = 0;
	while(ptAux != NULL){
		printf("node %d\tvalue %d\n", counter, ptAux->value);
		counter++;
		ptAux = ptAux->next;
	}
}
