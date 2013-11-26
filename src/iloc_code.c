#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iloc_code.h"

int insert(iloc_code **code, char *format, ...){
	va_list ap;
	char buffer[100];
	va_start(ap, format);

	vsnprintf(buffer, 100, format, ap);
	va_end(ap);

	iloc_code *newOperation = malloc(sizeof(iloc_code));
	if(newOperation == NULL) return 1;//couldnt alloc
	newOperation->operation = strdup(buffer);
	newOperation->next = NULL;

	if(*code == NULL) *code = newOperation;
	else {
		iloc_code *ptAux = *code;
		while(ptAux->next != NULL) ptAux = ptAux->next;
		ptAux->next = newOperation;
	}
	return 0;
}

void concatCode(iloc_code **code1, iloc_code **code2){
	if(*code2 == NULL) return;
	if(*code1 == NULL){
		*code1 = *code2;
		return;
	}

	iloc_code *ptAux = *code1;
	while(ptAux->next != NULL) ptAux = ptAux->next;
	ptAux->next = *code2;
}

int printCode(iloc_code *code){
	iloc_code *ptAux = code;
	while(ptAux != NULL){
		printf("%s\n", ptAux->operation);
		ptAux = ptAux->next;
	}
}
