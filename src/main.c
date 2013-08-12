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

int main (int argc, char **argv)
{
	int token = TOKEN_ERRO;
	//while (token = yylex()){
	//  printf ("token <%d> at %d\n", token, getLineNumber());
	//}

	linkedListTest();

	return 0;
}
