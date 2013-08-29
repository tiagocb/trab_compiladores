#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp_dict.h"

/* Create a new dictionary with a given number of lists */
int createDictionaty(comp_dict_t *dict, int numberOfLists){
    if(numberOfLists < 1) return 1;//invalid list number
    dict->numberOfLists = numberOfLists;
    dict->table = malloc(sizeof(comp_dict_node_t *) * numberOfLists);
    int i;
    for(i = 0; i < numberOfLists; i++)
        dict->table[i] = NULL;

    dict->numberOfElements = 0;
    return 0;
}

unsigned int hashFunction(int numberOfLists, char *key){
    unsigned int hashValue = 0;

    for(; *key != '\0'; key++)
        hashValue = *key + (hashValue << 5) - hashValue;

    return hashValue % numberOfLists;
}

/* Search for a key in the dictionary and returns the item if it was found */
comp_dict_item_t *searchKey(comp_dict_t dict, char *key){
    comp_dict_node_t *node;
    unsigned int hashValue = hashFunction(dict.numberOfLists, key);

    for(node = dict.table[hashValue]; node != NULL; node = node->next)
        if(strcmp(key, node->item->key) == 0)
            return node->item;

    return NULL;
}

/* Insert key (if it doesnt exist) with its associated value */
int insertKey(comp_dict_t *dict, char *key, int value, int type){
    if(searchKey(*dict, key) != NULL)
        return 1;//key already exists

    comp_dict_node_t *newNode;
    unsigned int hashValue = hashFunction(dict->numberOfLists, key);

    newNode = malloc(sizeof(comp_dict_node_t));
    if(newNode == NULL) return 2;//couldnt alloc
    newNode->item = malloc(sizeof(comp_dict_item_t));
    if(newNode->item == NULL) return 3;//couldnt alloc

    newNode->item->key = strdup(key);
    newNode->item->value = value;
    newNode->item->type = type;
    newNode->next = dict->table[hashValue];
    dict->table[hashValue] = newNode;
    dict->numberOfElements++;
    return 0;
}

/* Updates the value associated with the key */
int updateKey(comp_dict_t dict, char *key, int newValue){
    comp_dict_node_t *node;
    unsigned int hashValue = hashFunction(dict.numberOfLists, key);

    int existingKey = 0;
    for(node = dict.table[hashValue]; node != NULL; node = node->next)
        if(strcmp(key, node->item->key) == 0){
            node->item->value = newValue;
            existingKey = 1;
            break;
        }

    if(existingKey == 0) return 1;//not found
    return 0;
}

/* Deletes the node associated with the key */
int deleteKey(comp_dict_t *dict, char *key){
    comp_dict_node_t *node, *tempNode;
    unsigned int hashValue = hashFunction(dict->numberOfLists, key);

    node = dict->table[hashValue];
    if(node == NULL) return 1;//invalid key
    if(strcmp(key, node->item->key) == 0){
        tempNode = node;
        dict->table[hashValue] = node->next;
        free(tempNode);
        dict->numberOfElements--;
        return 0;
    }

    while(node->next != NULL){
        if(strcmp(key, node->next->item->key) == 0){
            tempNode = node->next;
            node->next = node->next->next;
            free(tempNode);
            dict->numberOfElements--;
            return;
        }
        node = node->next;
    }
    return 1;//invalid key
}

int getNumberOfKeys(comp_dict_t dict){
    return dict.numberOfElements;
}

/* Remove all items in the table */
void clearDictionaryContent(comp_dict_t *dict){
    comp_dict_node_t *node, *tempNode;
    int i;
    for(i = 0; i < dict->numberOfLists; i++){
        node = dict->table[i];
        while(node != NULL){
            tempNode = node;
            node = node->next;
            free(tempNode->item->key);
            free(tempNode->item);
            free(tempNode);
        }
        dict->table[i] = NULL;
    }

    dict->numberOfElements = 0;
}

/* Destroy dictionaty table */
void destroyDictionary(comp_dict_t *dict){
    clearDictionaryContent(dict);
    free(dict->table);
    dict->numberOfLists = 0;
    dict->table = NULL;
}

/* Print dictionary content */
void printDictionary(comp_dict_t dict){
    if(dict.table == NULL){
        printf("Tabela nao inicializada\n");
        return;
    }

    printf("Number of nodes: %d\n", dict.numberOfElements);
    comp_dict_node_t *node;
    int i;
    for(i = 0; i < dict.numberOfLists; i++){
        node = dict.table[i];
        printf("List %d:\n", i);
        if(node == NULL) printf("\tEmpty\n");

        int counter = 0;
        while(node != NULL){
            printf("\tnode %d: %s -> (%d, %d)\n", counter, node->item->key, node->item->value, node->item->type);
            node = node->next;
            counter++;
        }
    }
}
