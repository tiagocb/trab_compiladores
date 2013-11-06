#include <stdio.h>
#include <stdlib.h>
#include "iloc_code.h"

int insertOperation(iloc_code **code, int label, int op, int op1, int op2, int op3){
	iloc_code *newOperation = malloc(sizeof(iloc_code));
	if(newOperation == NULL) return 1;//couldnt alloc
	newOperation->operator = op;
	newOperation->operand1 = op1;
	newOperation->operand2 = op2;
	newOperation->operand3 = op3;
	newOperation->label = label;
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
		if(ptAux->label != ILOC_NO_LABEL){
			printf("L%d: ", ptAux->label);
		}

		switch(ptAux->operator){
			case ILOC_NOP: printf("nop\n"); break;
			case ILOC_ADD: printf("add\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_SUB: printf("sub\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_MULT: printf("mult\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_DIV: printf("div\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_ADDI: printf("addI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_SUBI: printf("subI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_RSUBI: printf("rsubI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_MULTI: printf("multI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_DIVI: printf("divI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_RDIVI: printf("rdivI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_LSHIFT: printf("lshift\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_LSHIFTI: printf("lshiftI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_RSHIFT: printf("rshift\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_RSHIFTI: printf("rshiftI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_AND: printf("and\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_ANDI: printf("andI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_OR: printf("or\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_ORI: printf("orI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_XOR: printf("xor\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_XORI: printf("xorI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_LOADI: printf("loadI\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_LOAD: printf("load\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_LOADAI: printf("loadAI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_LOADAO: printf("loadAO\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CLOAD: printf("cload\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_CLOADAI: printf("cloadAI\tr%d,\t%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CLOADAO: printf("cloadAO\tr%d,\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_STORE: printf("store\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_STOREAI: printf("storeAI\tr%d\t=>\tr%d,\t%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_STOREAO: printf("storeAO\tr%d\t=>\tr%d,\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CSTORE: printf("cstore\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_CSTOREAI: printf("cstoreAI\tr%d\t=>\tr%d,\t%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CSTOREAO: printf("cstoreAO\tr%d\t=>\tr%d,\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_I2I: printf("i2i\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_C2C: printf("c2c\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_C2I: printf("c2i\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_I2C: printf("i2c\tr%d\t=>\tr%d\n", ptAux->operand1,  ptAux->operand2); break;
			case ILOC_JUMPI: printf("jumpI\t->\tL%d\n", ptAux->operand1); break;
			case ILOC_JUMP: printf("jump\t->\tr%d\n", ptAux->operand1); break;
			case ILOC_CBR: printf("cbr\tr%d\t->\tL%d,\tL%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CMPLT: printf("cmp_LT\tr%d,\tr%d\t->\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CMPLE: printf("cmp_LE\tr%d,\tr%d\t->\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CMPEQ: printf("cmp_EQ\tr%d,\tr%d\t->\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CMPGE: printf("cmp_GE\tr%d,\tr%d\t->\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CMPGT: printf("cmp_GT\tr%d,\tr%d\t->\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
			case ILOC_CMPNE: printf("cmp_NE\tr%d,\tr%d\t->\tr%d\n", ptAux->operand1,  ptAux->operand2, ptAux->operand3); break;
		}

		ptAux = ptAux->next;
	}
}