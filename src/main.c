#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

int getLineNumber (void)
{
	/* deve ser implementada */
	return 0;
}

void linkedListTest(){
	//creates a new list and print it
	printf("Creating list...\n");
	comp_list_t *list;
	createList(&list);
	printList(list);

	//insert 20 randon values and print it
	printf("\nInserting 20 randon values...\n");
	int i;
	for(i = 0; i < 20; i++) insertTail(&list, rand()%100);
	printList(list);

	//check 1st, 2nd and 3rd values and print list
	printf("\nChecking and deleting 1st, 2nd and 3rd values...\n");
	int value = getFirst(list);
	delete(&list, 1);
	printf("first value: %d\n", value);
	value = getFirst(list);
	delete(&list, 1);
	printf("second value: %d\n", value);
	value = getFirst(list);
	delete(&list, 1);
	printf("third value: %d\n", value);
	printList(list);

	//updates 5, 10 and 15 nodes and print list
	printf("\nUpdating 5th, 10th and 15th nodes (new value = 999)...\n");
	update(list, 5, 999);
	update(list, 10, 999);
	update(list, 15, 999);
	update(list, 50, 999);//must be out of range
	printList(list);

	//delete 7, 9 and 11 nodes and print list
	printf("\nDeleting 7th, 9th and 11th nodes...\n");
	delete(&list, 7);
	delete(&list, 9);
	delete(&list, 11);
	delete(&list, 20);//must be out of range
	printList(list);
}

void dictionaryTest(){
    //creates dictionary with 20 lists and print it
    printf("\nCreating dictionary with 20 lists...\n");
    comp_dict_t dictionary;
    createDictionaty(&dictionary, 5);
    printDictionary(dictionary);

    //insert keys and print dictionary
    printf("\n");
    int quit = 0;
    char input[256];
    while(!quit){
        printf("Insert a key: ");
        gets(input);
        if(strcmp(input, "quit") == 0){
            quit = 1;
            break;
        }
        insertKey(&dictionary, input, rand()%100);
    }
    printDictionary(dictionary);

    //search for keys and print dictionary
    printf("\nSearch for a key: ");
    comp_dict_item_t *item;
    quit = 0;
    while(!quit){
        gets(input);
        if(strcmp(input, "quit") == 0){
            quit = 1;
            break;
        }
        item = searchKey(dictionary, input);
        if(item != NULL) printf("%s -> %d\n", item->key, item->value);
        else printf("not found\n");
    }
    printDictionary(dictionary);

    //update values and print dictionary
    printf("\nUpdate a key: ");
    quit = 0;
    while(!quit){
        gets(input);
        if(strcmp(input, "quit") == 0){
            quit = 1;
            break;
        }
        if(updateKey(dictionary, input, 999) != 0) printf("not found\n");
    }
    printDictionary(dictionary);

    //delete keys and print dictionary
    printf("\nDelete a key: ");
    quit = 0;
    while(!quit){
        gets(input);
        if(strcmp(input, "quit") == 0){
            quit = 1;
            break;
        }
        if(deleteKey(&dictionary, input) != 0) printf("not found\n");
    }
    printDictionary(dictionary);

    //clear content, print it, destroy it and print it again
    printf("\nRemoving content...\n");
    clearDictionaryContent(&dictionary);
    printDictionary(dictionary);
    printf("\nDestroying dictionary...\n");
    destroyDictionary(&dictionary);
    printDictionary(dictionary);
}

void graphTest(){
    comp_graph_t *grafo;
    createGraph(&grafo);
    printGraph(grafo);

    insertNode(&grafo, 0, 0);
    insertNode(&grafo, 1, 1);
    insertNode(&grafo, 2, 2);
    insertNode(&grafo, 3, 3);
    insertNode(&grafo, 4, 4);
    insertNode(&grafo, 5, 5);
    insertNode(&grafo, 6, 6);
    insertNode(&grafo, 7, 7);
    insertNode(&grafo, 8, 8);
    insertNode(&grafo, 9, 9);
    printGraph(grafo);

    insertEdge(grafo, 0, 1);
    insertEdge(grafo, 0, 2);
    insertEdge(grafo, 1, 3);
    insertEdge(grafo, 3, 6);
    insertEdge(grafo, 2, 3);
    insertEdge(grafo, 3, 4);
    insertEdge(grafo, 3, 5);
    insertEdge(grafo, 3, 6);
    insertEdge(grafo, 6, 7);
    insertEdge(grafo, 5, 7);
    insertEdge(grafo, 4, 7);
    insertEdge(grafo, 7, 8);
    insertEdge(grafo, 8, 9);
    insertEdge(grafo, 7, 8);
    insertEdge(grafo, 9, 9);
    insertEdge(grafo, 9, 0);
    printGraph(grafo);

    updateNode(grafo, 1, 999);
    updateNode(grafo, 3, 999);
    updateNode(grafo, 5, 999);
    updateNode(grafo, 7, 999);
    updateNode(grafo, 9, 999);
    printGraph(grafo);

    removeNodeEdges(grafo, 2);
    removeNodeEdges(grafo, 5);
    removeNodeEdges(grafo, 8);
    printGraph(grafo);

    removeNode(&grafo, 3);
    removeNode(&grafo, 6);
    printGraph(grafo);

    destroyGrapth(&grafo);
    printGraph(grafo);
}

int main (int argc, char **argv)
{
	int token = TOKEN_ERRO;
	//while (token = yylex()){
	//  printf ("token <%d> at %d\n", token, getLineNumber());
	//}

	linkedListTest();
	dictionaryTest();
	graphTest();

	return 0;
}
