#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp_dict.h"

int createDictionaty(comp_dict_t *dict, int numberOfLists, comp_dict_t *parent) {
	if (numberOfLists < 1) return 1;//invalid list number
	dict->numberOfLists = numberOfLists;
	dict->table = malloc(sizeof(comp_dict_node_t *) * numberOfLists);
	int i;
	for (i = 0; i < numberOfLists; i++)
		dict->table[i] = NULL;

	dict->numberOfElements = 0;
	dict->parent = parent;
	return 0;
}

unsigned int hashFunction (int numberOfLists, char *key) {
	unsigned int hashValue = 0;

	for (; *key != '\0'; key++)
		hashValue = *key + (hashValue << 5) - hashValue;

	return hashValue % numberOfLists;
}

comp_dict_item_t *searchKey (comp_dict_t dict, char *key) {
	comp_dict_node_t *node;
	unsigned int hashValue = hashFunction(dict.numberOfLists, key);

	for (node = dict.table[hashValue]; node != NULL; node = node->next)
		if (strcmp(key, node->item->key) == 0)
			return node->item;

	return NULL;
}

comp_dict_item_t *insertKey(comp_dict_t *dict, char *key, int valueType, int line) {	
	comp_dict_item_t *item;
	item = searchKey(*dict, key);
	if (item != NULL){
		return item;//key already exists
	}

	comp_dict_node_t *newNode;
	unsigned int hashValue = hashFunction(dict->numberOfLists, key);

	newNode = malloc(sizeof(comp_dict_node_t));
	if (newNode == NULL) return NULL;//couldnt alloc
	newNode->item = malloc(sizeof(comp_dict_item_t));
	if (newNode->item == NULL) return NULL;//couldnt alloc

	newNode->item->key = strdup(key);//store key
	newNode->item->valueType = valueType;//store value type

	//initialize value (only literals have a valid value) and number of bytes
	if(valueType == IKS_STRING){ //cut string's double quotes 
		newNode->item->stringValue = strdup(key);
		newNode->item->stringValue[strlen(key) - 1] = '\0';
		newNode->item->stringValue = newNode->item->stringValue + 1;
		newNode->item->numBytes = 4;
	}
	if(valueType == IKS_CHAR){
		newNode->item->charValue = key[1];//cut char's single quotes
		newNode->item->numBytes = 1;
	}
	if(valueType == IKS_INT){
		newNode->item->intValue = atoi(key);
		newNode->item->numBytes = 4;
	}
	if(valueType == IKS_FLOAT){
		newNode->item->floatValue = atof(key);
		newNode->item->numBytes = 8;
	}
	if(valueType == IKS_BOOL){
		newNode->item->boolValue = (strcmp(key, "true") == 0 ? 1: 0);
		newNode->item->numBytes = 1;
	}
	if(valueType == IKS_UNDEFINED){
		newNode->item->floatValue = 0;
		newNode->item->numBytes = 0;
	}

	newNode->item->nodeType = IKS_UNDEFINED_ITEM;//initialize node type
	newNode->item->line = line;//store line
	newNode->item->functionSymbolTable = NULL;//initialize function symbol table
	newNode->item->parametersList = NULL;//initialize parameters list
	createList(&(newNode->item->dimensionList));//initialize dimensions list
	createList(&(newNode->item->localVars));//initialize local vars list

	newNode->next = dict->table[hashValue];
	dict->table[hashValue] = newNode;
	dict->numberOfElements++;
	return newNode->item;
}

int updateKey(comp_dict_t dict, char *key, int newValueType) {
	comp_dict_node_t *node;
	unsigned int hashValue = hashFunction(dict.numberOfLists, key);

	int existingKey = 0;
	for (node = dict.table[hashValue]; node != NULL; node = node->next)
		if (strcmp(key, node->item->key) == 0) {
			node->item->valueType = newValueType;
			existingKey = 1;
			break;
		}

	if (existingKey == 0) return 1;//not found
	return 0;
}

int deleteKey (comp_dict_t *dict, char *key) {
	comp_dict_node_t *node, *tempNode;
	unsigned int hashValue = hashFunction(dict->numberOfLists, key);

	node = dict->table[hashValue];
	if (node == NULL) return 1;//invalid key
	if (strcmp(key, node->item->key) == 0) {
		tempNode = node;
		dict->table[hashValue] = node->next;
		if(tempNode->item->valueType == IKS_STRING)
			free(tempNode->item->stringValue);
		free(tempNode->item->key);
		free(tempNode->item);
		free(tempNode);
		dict->numberOfElements--;
		return 0;
	}

	while (node->next != NULL) {
		if (strcmp(key, node->next->item->key) == 0) {
			tempNode = node->next;
			node->next = node->next->next;
			if(tempNode->item->valueType == IKS_STRING)
				free(tempNode->item->stringValue);
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
			dict->numberOfElements--;
			return;
		}
		node = node->next;
	}
	return 1;//invalid key
}

int getNumberOfKeys (comp_dict_t dict) {
	return dict.numberOfElements;
}

void clearDictionaryContent (comp_dict_t *dict) {
	comp_dict_node_t *node, *tempNode;
	int i;
	for (i = 0; i < dict->numberOfLists; i++) {
		node = dict->table[i];
		while (node != NULL){
			tempNode = node;
			node = node->next;
			if(tempNode->item->valueType == IKS_STRING)
				free(tempNode->item->stringValue);
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
		}
		dict->table[i] = NULL;
	}

	dict->numberOfElements = 0;
}

void destroyDictionary (comp_dict_t *dict) {
	clearDictionaryContent(dict);
	free(dict->table);
	dict->numberOfLists = 0;
	dict->table = NULL;
}

void printDictionary (comp_dict_t dict) {
	if (dict.table == NULL) {
		printf("Tabela nao inicializada\n");
		return;
	}

	printf("Number of nodes: %d\n", dict.numberOfElements);
	comp_dict_node_t *node;
	int i;
	for (i = 0; i < dict.numberOfLists; i++) {
		node = dict.table[i];
		printf("List %d:\n", i);
		if (node == NULL) printf("\tEmpty\n");

		int counter = 0;
		while (node != NULL) {
			printf("\tnode %d: %s -> (", counter, node->item->key);
			switch(node->item->nodeType){
				case IKS_VARIABLE_ITEM: printf("VARIABLE, "); break;
				case IKS_VECTOR_ITEM: printf("VECTOR, "); break;
				case IKS_LITERAL_ITEM: printf("LITERAL, "); break;
				case IKS_FUNCTION_ITEM: printf("FUNCTION, "); break;
				case IKS_UNDEFINED_ITEM: printf("UNDEFINED, "); break;
			}

			if(node->item->nodeType == IKS_LITERAL_ITEM){
				switch(node->item->valueType){
					case IKS_INT: printf("int, value: %d, ", node->item->intValue); break;
					case IKS_FLOAT: printf("float, value: %f, ", node->item->floatValue); break;
					case IKS_CHAR: printf("char, value: %c, ", node->item->charValue); break;
					case IKS_STRING: printf("string, value: %s, ", node->item->stringValue); break;
					case IKS_BOOL: printf("bool, value: %d, ", node->item->boolValue); break;
					case IKS_UNDEFINED: printf("undefined, value: %d, ", node->item->intValue); break;
				}
			}
	
			if(node->item->nodeType == IKS_VECTOR_ITEM){
				printf("dim list:");
				printList(node->item->dimensionList);
				printf(", ");
			}

			printf("line: %d)\n", node->item->line);
			
			node = node->next;
			counter++;
		}
	}
}
