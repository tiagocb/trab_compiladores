#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

int _gst_createTable(_gst_hash_table *ht, int numberOfLists){
	if(numberOfLists < 1) return 1;//invalid list number
	ht->numberOfLists = numberOfLists;
	ht->table = malloc(sizeof(_gst_hash_table_node *) * numberOfLists);
	int i;
	for(i = 0; i < numberOfLists; i++)
		ht->table[i] = NULL;

	ht->numberOfElements = 0;
	return 0;
}

unsigned int _gst_hashFunction(int numberOfLists, char *key){
	if(key == NULL) return -1;

	unsigned int hashValue = 0;
	for(; *key != '\0'; key++)
		hashValue = *key + (hashValue << 5) - hashValue;

	return hashValue % numberOfLists;
}

_gst_hash_table_item *_gst_searchKey(_gst_hash_table ht, char *key){
	if(key == NULL) return NULL;

	_gst_hash_table_node *node;
	unsigned int hashValue = _gst_hashFunction(ht.numberOfLists, key);

	for(node = ht.table[hashValue]; node != NULL; node = node->next)
		if(strcmp(key, node->item->key) == 0)
			return node->item;

	return NULL;
}

int _gst_insertKey(_gst_hash_table *ht, char *key, void *data){
	if(key == NULL) return 1;

	_gst_hash_table_item *item;
	item = _gst_searchKey(*ht, key);
	if(item != NULL){
		return 1;//key already exists
	}

	_gst_hash_table_node *newNode;
	unsigned int hashValue = _gst_hashFunction(ht->numberOfLists, key);

	newNode = malloc(sizeof(_gst_hash_table_node));
	if(newNode == NULL) return 1;//couldnt alloc
	newNode->item = malloc(sizeof(_gst_hash_table_item));
	if(newNode->item == NULL) return 1;//couldnt alloc

	newNode->item->key = strdup(key);//store key
	newNode->item->data = data;//store data

	newNode->next = ht->table[hashValue];
	ht->table[hashValue] = newNode;
	ht->numberOfElements++;
	return 0;
}

int _gst_updateKey(_gst_hash_table ht, char *key, void *data){
	if(key == NULL) return 1;

	_gst_hash_table_node *node;
	unsigned int hashValue = _gst_hashFunction(ht.numberOfLists, key);

	int existingKey = 0;
	for(node = ht.table[hashValue]; node != NULL; node = node->next)
		if(strcmp(key, node->item->key) == 0){
			node->item->data = data;
			existingKey = 1;
			break;
		}

	if(existingKey == 0) return 1;//not found
	return 0;
}

void *_gst_deleteKey(_gst_hash_table *ht, char *key){
	if(key == NULL) return NULL;

	_gst_hash_table_node *node, *tempNode;
	unsigned int hashValue = _gst_hashFunction(ht->numberOfLists, key);
	void *data;
	node = ht->table[hashValue];
	if(node == NULL) return NULL;//invalid key
	
	//if key is list header
	if(strcmp(key, node->item->key) == 0){
		tempNode = node;
		ht->table[hashValue] = node->next;
		data = tempNode->item->data;
		free(tempNode->item->key);
		free(tempNode->item);
		free(tempNode);
		ht->numberOfElements--;
		return data;
	}

	while(node->next != NULL){
		if(strcmp(key, node->next->item->key) == 0){
			tempNode = node->next;
			node->next = node->next->next;
			data = tempNode->item->data;
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
			ht->numberOfElements--;
			return data;
		}
		node = node->next;
	}
	return NULL;//invalid key
}

void *_gst_getData(_gst_hash_table ht, char *key){
	if(key == NULL) return NULL;

	_gst_hash_table_node *node;
	unsigned int hashValue = _gst_hashFunction(ht.numberOfLists, key);

	for(node = ht.table[hashValue]; node != NULL; node = node->next)
		if(strcmp(key, node->item->key) == 0){
			return node->item->data;
		}

	return NULL;
}

int _gst_getNumberOfKeys(_gst_hash_table ht){
	return ht.numberOfElements;
}

void _gst_clearTableContent(_gst_hash_table *ht){
	_gst_hash_table_node *node, *tempNode;
	int i;
	for(i = 0; i < ht->numberOfLists; i++) {
		node = ht->table[i];
		while(node != NULL){
			tempNode = node;
			node = node->next;
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
		}
		ht->table[i] = NULL;
	}

	ht->numberOfElements = 0;
}

void _gst_destroyTable(_gst_hash_table *ht){
	_gst_clearTableContent(ht);
	free(ht->table);
	ht->numberOfLists = 0;
	ht->table = NULL;
}

void _gst_printTable(_gst_hash_table ht){
	if (ht.table == NULL) {
		printf("Tabela nao inicializada\n");
		return;
	}

	printf("Number of nodes: %d\n", ht.numberOfElements);
	_gst_hash_table_node *node;
	int i;
	for(i = 0; i < ht.numberOfLists; i++){
		node = ht.table[i];
		printf("List %d:\n", i);
		if(node == NULL) printf("\tEmpty\n");

		int counter = 0;
		while(node != NULL){
			printf("\tnode %d: %s -> %p\n", counter, node->item->key, node->item->data);
			node = node->next;
			counter++;
		}
	}
}
