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

void treeTest(){
	comp_tree_t *tree;

	//Create tree and print info
	createTree(&tree);
	printf("number of nodes: %d\n", countNodes(tree));
	printf("number of leafs: %d\n", countLeafs(tree));
	printf("depth: %d\n", countDepth(tree));
	printf("is empty: %d\n", isTreeEmpty(tree));
	printf("\n");


	/*
									1
		  			2      				3				4
		         5         6         7         8                9                       10             11
	             12      13              14     15 16 17    18  19  20  21  22                          23     24
		   25  26    27            28 29          30               31 32                            33
	               34                35 36                                                     37 38 39 40 41 42 43
                                            44
                                            45
                                            46
                                        47      48
                                             49    50

		resultado esperado 1 2 5 12 25 26 34 13 27 6 7 14 28 35 36 44 45 46 47 48 49 50 29 8 15 16 17 30 3 9 18 19 20 21 31 32 22
                4 10 11 23 33 37 38 39 40 41 42 43 24

	*/
	insert(&tree, 0, 1, 0);insert(&tree, 0, 2, 1);insert(&tree, 0, 3, 1);insert(&tree, 0, 4, 1);
	insert(&tree, 0, 5, 2);insert(&tree, 0, 6, 2);insert(&tree, 0, 7, 2);insert(&tree, 0, 8, 2);
	insert(&tree, 0, 9, 3);insert(&tree, 0, 10, 4);insert(&tree, 0, 11, 4);insert(&tree, 0, 12, 5);
	insert(&tree, 0, 13, 5);insert(&tree, 0, 14, 7);insert(&tree, 0, 15, 8);insert(&tree, 0, 16, 8);
	insert(&tree, 0, 17, 8);insert(&tree, 0, 18, 9);insert(&tree, 0, 19, 9);insert(&tree, 0, 20, 9);
	insert(&tree, 0, 21, 9);insert(&tree, 0, 22, 9);insert(&tree, 0, 23, 11);insert(&tree, 0, 24, 11);
	insert(&tree, 0, 25, 12);insert(&tree, 0, 26, 12);insert(&tree, 0, 27, 13);insert(&tree, 0, 28, 14);
	insert(&tree, 0, 29, 14);insert(&tree, 0, 30, 17);insert(&tree, 0, 31, 21);insert(&tree, 0, 32, 21);
	insert(&tree, 0, 33, 23);insert(&tree, 0, 34, 26);insert(&tree, 0, 35, 28);insert(&tree, 0, 36, 28);
	insert(&tree, 0, 37, 33);insert(&tree, 0, 38, 33);insert(&tree, 0, 39, 33);insert(&tree, 0, 40, 33);
	insert(&tree, 0, 41, 33);insert(&tree, 0, 42, 33);insert(&tree, 0, 43, 33);insert(&tree, 0, 44, 36);
	insert(&tree, 0, 45, 44);insert(&tree, 0, 46, 45);insert(&tree, 0, 47, 46);insert(&tree, 0, 48, 46);
	insert(&tree, 0, 49, 48);insert(&tree, 0, 50, 48);
	printTree(tree);
	printf("\nnumber of nodes: %d\n", countNodes(tree));
	printf("number of leafs: %d\n", countLeafs(tree));
	printf("depth: %d\n", countDepth(tree));
	printf("is empty: %d\n", isTreeEmpty(tree));
	printf("\n");

	printf("contains key 0: %d\n", containsKey(tree, 0));
	printf("contains key 1: %d\n", containsKey(tree, 1));
	printf("contains key 5: %d\n", containsKey(tree, 5));
	printf("contains key 14: %d\n", containsKey(tree, 14));
	printf("contains key 24: %d\n", containsKey(tree, 24));
	printf("contains key 34: %d\n", containsKey(tree, 34));
	printf("contains key 41: %d\n", containsKey(tree, 41));
	printf("contains key 49: %d\n", containsKey(tree, 49));
	printf("contains key 50: %d\n", containsKey(tree, 50));
	printf("contains key 51: %d\n", containsKey(tree, 51));

	updateValue(tree, 4, 999);
	updateValue(tree, 16, 999);
	updateValue(tree, 32, 999);
	printTree(tree);
	printf("\n\n\n");

	/*
									1
		  			2      				3				4
		         5         6         7         8                                               10
	             12      13                     15 16 17
		   25  26    27                           30
	               34


		resultado esperado 1 2 5 12 25 26 34 13 27 6 7 8 15 16 17 30 3 4 10

	*/
	removeNode(&tree, 14);
	removeNode(&tree, 9);
	removeNode(&tree, 11);
	printTree(tree);
	printf("\nnumber of nodes: %d\n", countNodes(tree));
	printf("number of leafs: %d\n", countLeafs(tree));
	printf("depth: %d\n", countDepth(tree));
	printf("is empty: %d\n", isTreeEmpty(tree));
	printf("\n");
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
	treeTest();

	return 0;
}
