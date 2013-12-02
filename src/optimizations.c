#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optimizations.h"

opt_iloc_code *code = NULL;

void opt_iloc_code_print(){
	opt_iloc_code *ptAux = code;
	while(ptAux != NULL){
	
		//imprime rótulo
		if(ptAux->instruction.label != NULL) printf("%s: ", ptAux->instruction.label);
		
		//imprime nome da operação
		printf("%s ", ptAux->instruction.op);
		
		if(strcmp(ptAux->instruction.op, "nop") == 0){}
		
		else if(strcmp(ptAux->instruction.op, "add") == 0
						|| strcmp(ptAux->instruction.op, "sub") == 0
						|| strcmp(ptAux->instruction.op, "mult") == 0
						|| strcmp(ptAux->instruction.op, "div") == 0
						|| strcmp(ptAux->instruction.op, "addI") == 0
						|| strcmp(ptAux->instruction.op, "subI") == 0
						|| strcmp(ptAux->instruction.op, "rsubI") == 0
						|| strcmp(ptAux->instruction.op, "multI") == 0
						|| strcmp(ptAux->instruction.op, "divI") == 0
						|| strcmp(ptAux->instruction.op, "rdivI") == 0
						|| strcmp(ptAux->instruction.op, "and") == 0
						|| strcmp(ptAux->instruction.op, "andI") == 0
						|| strcmp(ptAux->instruction.op, "or") == 0
						|| strcmp(ptAux->instruction.op, "orI") == 0
						|| strcmp(ptAux->instruction.op, "xor") == 0
						|| strcmp(ptAux->instruction.op, "xorI") == 0
						|| strcmp(ptAux->instruction.op, "lshift") == 0
						|| strcmp(ptAux->instruction.op, "lshiftI") == 0
						|| strcmp(ptAux->instruction.op, "rshift") == 0
						|| strcmp(ptAux->instruction.op, "rshiftI") == 0
						|| strcmp(ptAux->instruction.op, "loadAI") == 0
						|| strcmp(ptAux->instruction.op, "loadAO") == 0
						|| strcmp(ptAux->instruction.op, "cloadAI") == 0
						|| strcmp(ptAux->instruction.op, "cloadAO") == 0){
						
			printf("%s, %s => %s", ptAux->instruction.op1, ptAux->instruction.op2, ptAux->instruction.op3);
		}

		else if(strcmp(ptAux->instruction.op, "storeAI") == 0
						|| strcmp(ptAux->instruction.op, "storeAO") == 0
						|| strcmp(ptAux->instruction.op, "cstoreAI") == 0
						|| strcmp(ptAux->instruction.op, "cstoreAO") == 0){
						
			printf("%s => %s, %s", ptAux->instruction.op1, ptAux->instruction.op2, ptAux->instruction.op3);
		}

		else if(strcmp(ptAux->instruction.op, "loadI") == 0
						|| strcmp(ptAux->instruction.op, "load") == 0
						|| strcmp(ptAux->instruction.op, "store") == 0
						|| strcmp(ptAux->instruction.op, "cstore") == 0
						|| strcmp(ptAux->instruction.op, "i2i") == 0
						|| strcmp(ptAux->instruction.op, "c2c") == 0
						|| strcmp(ptAux->instruction.op, "c2i") == 0
						|| strcmp(ptAux->instruction.op, "i2c") == 0){
						
			printf("%s => %s", ptAux->instruction.op1, ptAux->instruction.op2);
		}
		
		else if(strcmp(ptAux->instruction.op, "inc") == 0
						|| strcmp(ptAux->instruction.op, "dec") == 0){
						
			printf("%s", ptAux->instruction.op1);
		}
		
		else if(strcmp(ptAux->instruction.op, "jumpI") == 0
						|| strcmp(ptAux->instruction.op, "jump") == 0){
						
			printf("-> %s", ptAux->instruction.op1);
		}
		
		else if(strcmp(ptAux->instruction.op, "cbr") == 0){
						
			printf("%s -> %s, %s", ptAux->instruction.op1, ptAux->instruction.op2, ptAux->instruction.op3);
		}
		
		else if(strcmp(ptAux->instruction.op, "cmp_LT") == 0
						|| strcmp(ptAux->instruction.op, "cmp_LE") == 0
						|| strcmp(ptAux->instruction.op, "cmp_EQ") == 0
						|| strcmp(ptAux->instruction.op, "cmp_GE") == 0
						|| strcmp(ptAux->instruction.op, "cmp_GT") == 0
						|| strcmp(ptAux->instruction.op, "cmp_NE") == 0){
						
			printf("%s, %s -> %s", ptAux->instruction.op1, ptAux->instruction.op2, ptAux->instruction.op3);
		}
		
		printf("\n");

		ptAux = ptAux->next;
	}
}

void opt_iloc_add_instruction(char *instruction){

	void replace(char *buff, char old, char new){
		  char *ptr;        
		  while(1){
		      ptr = strchr(buff, old);
		      if(ptr == NULL) break; 
		      buff[(int)(ptr - buff)] = new;
		  }        
	}

	void replace_seq(char *buff, char old1, char old2, char new){
			int n = strlen(buff);
			int i;
			for(i = 0; i < n - 1; i++){
				if(buff[i] == old1 && buff[i+1] == old2){
					buff[i] = new;
					buff[i+1] = new;
				}
			}      
	}

	int count_chars(char *buff, char target){
		int counter = 0;
		int i;
		int n = strlen(buff);
		for(i = 0; i < n; i++){
			if(buff[i] == target) counter++;
		}
		return counter;      
	}

	void remove_extra_spaces(char *buff){
		int readingSpaces = 0;
		int stringIndex = 0;
		int lastValidCharPos = -1;
		int i;
		int n = strlen(buff);
		for(i = 0; i < n; i++){
			if(buff[i] != ' '){
				lastValidCharPos = stringIndex;
				buff[stringIndex++] = buff[i];
				readingSpaces = 0;
			}
			else if(buff[i] == ' ' && readingSpaces == 0){
				buff[stringIndex++] = buff[i];
				readingSpaces = 1;
			}
		}
		buff[lastValidCharPos + 1] = '\0';
		
	}

	//verifica se a função possui um rótulo
	int labeled = (count_chars(instruction, ':') > 0 ? 1 : 0);

	//Modifica a string para trabalhar melhor com ela
	replace(instruction, '\n', ' ');
	replace(instruction, '\t', ' ');
	replace(instruction, '=', ' ');
	replace_seq(instruction, '-', '>', ' ');
	replace(instruction, ':', ' ');
	replace(instruction, ',', ' ');
	replace(instruction, '>', ' ');
	replace(instruction, '/', '\0');

	//Remove espacos extra
	remove_extra_spaces(instruction);
	
	if(strlen(instruction) <= 1) return;

	opt_iloc_code *newInstruction = malloc(sizeof(opt_iloc_code));
	newInstruction->instruction.label = NULL;
	newInstruction->instruction.op = NULL;
	newInstruction->instruction.op1 = NULL;
	newInstruction->instruction.op2 = NULL;
	newInstruction->instruction.op3 = NULL;
	newInstruction->next = NULL;

	//get label and op
	char *result;
	if(labeled){
		result = strtok(instruction, " ");
		newInstruction->instruction.label = strdup(result);
		result = strtok(NULL, " ");
		newInstruction->instruction.op = strdup(result);
	}
	else{
		result = strtok(instruction, " ");
		newInstruction->instruction.op = strdup(result);
	}

	//get operands
	if(is_from_class(newInstruction->instruction.op, one_operand_instructions, num_one_operand_instructions) == 1){
		result = strtok(NULL, " ");
		newInstruction->instruction.op1 = strdup(result);
	}
	else if(is_from_class(newInstruction->instruction.op, two_operands_instructions, num_two_operands_instructions) == 1){
		result = strtok(NULL, " ");
		newInstruction->instruction.op1 = strdup(result);
		result = strtok(NULL, " ");
		newInstruction->instruction.op2 = strdup(result);
	}
	else if(is_from_class(newInstruction->instruction.op, three_operands_instructions, num_three_operands_instructions) == 1){
		result = strtok(NULL, " ");
		newInstruction->instruction.op1 = strdup(result);
		result = strtok(NULL, " ");
		newInstruction->instruction.op2 = strdup(result);
		result = strtok(NULL, " ");
		newInstruction->instruction.op3 = strdup(result);
	}

	if(code == NULL) code = newInstruction;
	else {
		opt_iloc_code *ptAux = code;
		while(ptAux->next != NULL) ptAux = ptAux->next;
		ptAux->next = newInstruction;
	}

	return;
}

/*
 * Otimizações de controle de fluxo
 * Em uma passagem pelo código, cria uma tabela com os rótulos associados às instruções que eles apontam.
 * Em uma segunda passagem pelo código, analiza todos os jumps incodicionais para rótulos (jumpI).
 * Para cada um deles, verifica se a instrução que possui o rótulo do jumpI também é um desvio incondicional.
 * Caso for, continua o processo até achar uma instrução que não é um desvio incondicional.
 * Substitui o rótulo do jumpI original pelo rótulo da instrução que não é um desvio incondicional.
 */
void control_flow_optimizations(){
	//Cria a tabela de rótulos
	_gst_hash_table ht;
	_gst_createTable(&ht, 2);
	
	//Varre o código preenchendo a tabela de rótulos
	opt_iloc_code *ptAux = code;
	while(ptAux != NULL){
	
		//Se tem rótulo, insere o rótulo e o ponteiro para a instrução na tabela
		if(ptAux->instruction.label != NULL)
			_gst_insertKey(&ht, ptAux->instruction.label, ptAux);
			
		ptAux = ptAux->next;
	}
	
	//Varre o código, analizando os desvios incondicionais do tipo jumpI e eventualmente troca o rótulo do desvio
	ptAux = code;
	while(ptAux != NULL){
	
		if(strcmp(ptAux->instruction.op, "jumpI") == 0){
		
			//Percorre as instruções até achar uma que não é um desvio incondicional
			opt_iloc_code *tmp = ptAux;
			char *currentLabel;
			int counter = 0;
			do{
				counter++;
				currentLabel = tmp->instruction.op1;
				tmp = (opt_iloc_code *)_gst_getData(ht, currentLabel);
			}while(strcmp(tmp->instruction.op, "jumpI") == 0);
			
			//Substitui o rótulo da instrução original
			if(counter > 1){
				free(ptAux->instruction.op1);
				ptAux->instruction.op1 = strdup(currentLabel);
			}
		}
		ptAux = ptAux->next;
	}

	//destrói a tabela
	_gst_hash_table_node *node, *tempNode;
	int i;
	for(i = 0; i < ht.numberOfLists; i++) {
		node = ht.table[i];
		while(node != NULL){
			tempNode = node;
			node = node->next;
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
		}
		ht.table[i] = NULL;
	}
	ht.numberOfElements = 0;
	free(ht.table);
	ht.numberOfLists = 0;
	ht.table = NULL;
}

/* Simplificações algébricas
 * Varre o código, substituindo algumas instruções por outras mais simples e mais eficientes.
 * Instruções que podem ser convertidas:
 * addI x, 0 => y                ->                i2i x => y
 * subI x, 0 => y                ->                i2i x => y
 * multI x, 1 => y               ->                i2i x => y
 * divI x, 1 => y                ->                i2i x => y
 * divI x, 2^i => y              ->                rshiftI x, i => y
 * multI x, 2^i => y             ->                lshiftI x, i => y
 */
void algebric_simplifications(){

	//Varre o código, substituindo as instruções que podem ser convertidas em outras mais simples
	opt_iloc_code *ptAux = code;
	while(ptAux != NULL){

		//addI x, 0 => y
		//i2i x => y
		if(strcmp(ptAux->instruction.op, "addI") == 0 && strcmp(ptAux->instruction.op2, "0") == 0){
			free(ptAux->instruction.op);
			char *op = "i2i";
			ptAux->instruction.op = strdup(op);
			
			free(ptAux->instruction.op2);
			ptAux->instruction.op2 = ptAux->instruction.op3;
			ptAux->instruction.op3 = NULL;
		}
		
		//subI x, 0 => y
		//i2i x => y
		if(strcmp(ptAux->instruction.op, "subI") == 0 && strcmp(ptAux->instruction.op2, "0") == 0){
			free(ptAux->instruction.op);
			char *op = "i2i";
			ptAux->instruction.op = strdup(op);
			
			free(ptAux->instruction.op2);
			ptAux->instruction.op2 = ptAux->instruction.op3;
			ptAux->instruction.op3 = NULL;
		}
		
		//multI x, 1 => y
		//i2i x => y
		if(strcmp(ptAux->instruction.op, "multI") == 0 && strcmp(ptAux->instruction.op2, "1") == 0){
			free(ptAux->instruction.op);
			char *op = "i2i";
			ptAux->instruction.op = strdup(op);
			
			free(ptAux->instruction.op2);
			ptAux->instruction.op2 = ptAux->instruction.op3;
			ptAux->instruction.op3 = NULL;
		}
		
		//divI x, 1 => y
		//i2i x => y
		if(strcmp(ptAux->instruction.op, "divI") == 0 && strcmp(ptAux->instruction.op2, "1") == 0){
			free(ptAux->instruction.op);
			char *op = "i2i";
			ptAux->instruction.op = strdup(op);
			
			free(ptAux->instruction.op2);
			ptAux->instruction.op2 = ptAux->instruction.op3;
			ptAux->instruction.op3 = NULL;
		}
		
		//divI x, 2^i => y
		//rshiftI x, i => y
		if(strcmp(ptAux->instruction.op, "divI") == 0){
			int constant = atoi(ptAux->instruction.op2);
			int test_num = 2;
			
			int counter = 1;
			while(test_num <= constant){
				if(test_num == constant){
					free(ptAux->instruction.op);
					char *op = "rshiftI";
					ptAux->instruction.op = strdup(op);
					
					free(ptAux->instruction.op2);
					char op2[10];
					sprintf(op2, "%d", counter);
					ptAux->instruction.op2 = strdup(op2);
					break;
				}
				
				counter++;
				test_num = test_num * 2;
			}
		}
		
		//multI x, 2^i => y
		//lshiftI x, i => y
		if(strcmp(ptAux->instruction.op, "multI") == 0){
			int constant = atoi(ptAux->instruction.op2);
			int test_num = 2;
			
			int counter = 1;
			while(test_num <= constant){
				if(test_num == constant){
					free(ptAux->instruction.op);
					char *op = "lshiftI";
					ptAux->instruction.op = strdup(op);
					
					free(ptAux->instruction.op2);
					char op2[10];
					sprintf(op2, "%d", counter);
					ptAux->instruction.op2 = strdup(op2);
					break;
				}
				
				counter++;
				test_num = test_num * 2;
			}
		}
		
		ptAux = ptAux->next;
	}
}

/* Uso da linguagem da máquina
 * Varre o código, substituindo as instruções addI x, 1 => x por inc x
 * e instruções subI x, 1 => x por dec x
 */
void use_machine_language(){
	
	void change_to_inc(opt_iloc_code *code){
		free(code->instruction.op);
		char *op = "inc";
		code->instruction.op = strdup(op);

		free(code->instruction.op1);
		code->instruction.op1 = code->instruction.op3;
		code->instruction.op3 = NULL;
		free(code->instruction.op2);
		code->instruction.op2 = NULL;
	}

	void change_to_dec(opt_iloc_code *code){
		free(code->instruction.op);
		char *op = "dec";
		code->instruction.op = strdup(op);

		free(code->instruction.op1);
		code->instruction.op1 = code->instruction.op3;
		code->instruction.op3 = NULL;
		free(code->instruction.op2);
		code->instruction.op2 = NULL;
	}

	//Varre o código procurando pelas instruções que podem ser substituidas
	opt_iloc_code *ptAux = code;
	while(ptAux != NULL){

		if(strcmp(ptAux->instruction.op, "addI") == 0 && strcmp(ptAux->instruction.op2, "1") == 0 && strcmp(ptAux->instruction.op1, ptAux->instruction.op3) == 0)
			change_to_inc(ptAux);
		if(strcmp(ptAux->instruction.op, "subI") == 0 && strcmp(ptAux->instruction.op2, "1") == 0 && strcmp(ptAux->instruction.op1, ptAux->instruction.op3) == 0)
			change_to_dec(ptAux);
		
		ptAux = ptAux->next;
	}
}

/* Propagação de copias
 * Ao copiar o valor de um registrador para outro,
 * o valor copiado pode ser utiizado no lugar da cópia enquando não houver uma escrita em algum deles.
 * O algortimo armazena em uma tabela a cópia e o copiado, caso encontrar uma leitura de um valor cópia, substitui pelo copiado.
 * Ao encontrar uma escrita em algum copiado ou cópia, remove a sua entrada da tabela.
 * Ao encontrar uma instrução rotulada, esvazia a tabela.
 */
void propagate_copies(){

	//Cria a tabela
	_gst_hash_table ht;
	_gst_createTable(&ht, 1);

	//Cria a tabela
	_gst_hash_table tmp_ht;
	_gst_createTable(&tmp_ht, 1);

	//define o valor de um registrador na tabela
	void set_register_value(_gst_hash_table *ht, char* key, char *value){
		if(_gst_searchKey(*ht, key) == NULL){
			char *valuePointer = strdup(value);
			_gst_insertKey(ht, key, valuePointer);
		}
	}

	//esvazia a tabela
	void empty_table(_gst_hash_table *ht){
		_gst_hash_table_node *node, *tempNode;
		int i;
		for(i = 0; i < ht->numberOfLists; i++) {
			node = ht->table[i];
			while(node != NULL){
				tempNode = node;
				node = node->next;
				free(tempNode->item->data);
				free(tempNode->item->key);
				free(tempNode->item);
				free(tempNode);
			}
			ht->table[i] = NULL;
		}
		ht->numberOfElements = 0;
	}

	int writeInstruction = 0;
	opt_iloc_code *ptAux = code;
	while(ptAux != NULL){

		//Se é uma instrução com rótulo, esvazia a tabela
		if(ptAux->instruction.label != NULL){
			empty_table(&ht);
			empty_table(&tmp_ht);
		}

		//Se é uma escrita em algum elemento da tabela, remove a entrada deste elemento
		if(is_from_class(ptAux->instruction.op, RW_instructions, num_RW_instructions) == 1){
			if(is_from_class(ptAux->instruction.op, three_operands_instructions, num_three_operands_instructions) == 1){
				char *reg1 = (char *)_gst_deleteKey(&tmp_ht, ptAux->instruction.op3);
				if(reg1 != NULL){
					char *reg2 = _gst_deleteKey(&ht, reg1);
					free(reg1);
					free(reg2);
					writeInstruction = 1;
				}
				else{
					char *reg2 = (char *)_gst_deleteKey(&ht, ptAux->instruction.op3);
					if(reg2 != NULL){
						char *reg1 = _gst_deleteKey(&tmp_ht, reg2);
						free(reg1);
						free(reg2);
						writeInstruction = 1;
					}
				}
			}

			if(is_from_class(ptAux->instruction.op, two_operands_instructions, num_two_operands_instructions) == 1){
				char *reg1 = (char *)_gst_deleteKey(&tmp_ht, ptAux->instruction.op2);
				if(reg1 != NULL){
					char *reg2 = _gst_deleteKey(&ht, reg1);
					free(reg1);
					free(reg2);
					writeInstruction = 1;
				}
				else{
					char *reg2 = (char *)_gst_deleteKey(&ht, ptAux->instruction.op2);
					if(reg2 != NULL){
						char *reg1 = _gst_deleteKey(&tmp_ht, reg2);
						free(reg1);
						free(reg2);
						writeInstruction = 1;
					}
				}
			}

			if(is_from_class(ptAux->instruction.op, one_operand_instructions, num_one_operand_instructions) == 1){
				char *reg1 = (char *)_gst_deleteKey(&tmp_ht, ptAux->instruction.op1);
				if(reg1 != NULL){
					char *reg2 = _gst_deleteKey(&ht, reg1);
					free(reg1);
					free(reg2);
					writeInstruction = 1;
				}
				else{
					char *reg2 = (char *)_gst_deleteKey(&ht, ptAux->instruction.op1);
					if(reg2 != NULL){
						char *reg1 = _gst_deleteKey(&tmp_ht, reg2);
						free(reg1);
						free(reg2);
						writeInstruction = 1;
					}
				}
			}
		}
		
		if(writeInstruction == 0){
			char *op1 = (char *)_gst_getData(ht, ptAux->instruction.op1);
			if(op1 != NULL){
				free(ptAux->instruction.op1);
				ptAux->instruction.op1 = strdup(op1);
			}

			char *op2 = (char *)_gst_getData(ht, ptAux->instruction.op2);
			if(op2 != NULL){
				free(ptAux->instruction.op2);
				ptAux->instruction.op2 = strdup(op2);
			}

			char *op3 = (char *)_gst_getData(ht, ptAux->instruction.op3);
			if(op3 != NULL){
				free(ptAux->instruction.op3);
				ptAux->instruction.op3 = strdup(op3);
			}

			//Se é uma cópia de registradores, insere a cópia na tabela como chave e o copiado como valor
			if(strcmp(ptAux->instruction.op, "i2i") == 0){
				set_register_value(&ht, ptAux->instruction.op2, ptAux->instruction.op1);
				set_register_value(&tmp_ht, ptAux->instruction.op1, ptAux->instruction.op2);
			}
		}
		writeInstruction = 0;

		ptAux = ptAux->next;
	}

	//destrói a tabela
	_gst_hash_table_node *node, *tempNode;
	int i;
	for(i = 0; i < ht.numberOfLists; i++) {
		node = ht.table[i];
		while(node != NULL){
			tempNode = node;
			node = node->next;
			free(tempNode->item->data);
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
		}
	}
	free(ht.table);


	//destrói a tabela
	for(i = 0; i < tmp_ht.numberOfLists; i++) {
		node = tmp_ht.table[i];
		while(node != NULL){
			tempNode = node;
			node = node->next;
			free(tempNode->item->data);
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
		}
	}
	free(tmp_ht.table);
}

/* Remoção de instruções redundantes e avaliação de operações com constantes
 */
void remove_redundant_instructions_and_evaluate_constant_operations(){

	typedef struct {
		char *value;
	} table_element;
	
	void free_table_element(table_element *element){
		free(element->value);
		free(element);
	}
	
	table_element *new_table_element(char *value){
		table_element *element = malloc(sizeof(table_element));
		if(element == NULL) return NULL;
		element->value = strdup(value);
		return element;
	}
	
	//cria a tabela de (registradores + value + read flag + last write instruction)
	_gst_hash_table ht;
	_gst_createTable(&ht, 2);
	
	//obtém um registrador na tabela
	table_element *get_register(char* key){
		if(_gst_searchKey(ht, key) != NULL){
			table_element *element = (table_element *)_gst_getData(ht, key);
			return element;
		}
		return NULL;
	}

	//insere um registrador na tabela
	void put_register(char* key, char *value){
		if(_gst_searchKey(ht, key) != NULL){
			table_element *tmpElement = (table_element *)_gst_getData(ht, key);
			free_table_element(tmpElement);
			
			table_element *element = new_table_element(value);
			_gst_updateKey(ht, key, element);
		}
		else{
			table_element *element = new_table_element(value);
			_gst_insertKey(&ht, key, element);
		}
	}
	
	//remove todos os itens da tabela
	void clear_table_content(){
		_gst_hash_table_node *node, *tempNode;
		int i;
		for(i = 0; i < ht.numberOfLists; i++) {
			node = ht.table[i];
			while(node != NULL){
				tempNode = node;
				node = node->next;
				free_table_element(tempNode->item->data);
				free(tempNode->item->key);
				free(tempNode->item);
				free(tempNode);
			}
			ht.table[i] = NULL;
		}
		ht.numberOfElements = 0;
	}
	
	//destrói a tabela
	void destroy_table(){
		_gst_hash_table_node *node, *tempNode;
		int i;
		for(i = 0; i < ht.numberOfLists; i++) {
			node = ht.table[i];
			while(node != NULL){
				tempNode = node;
				node = node->next;
				free_table_element(tempNode->item->data);
				free(tempNode->item->key);
				free(tempNode->item);
				free(tempNode);
			}
			ht.table[i] = NULL;
		}

		ht.numberOfElements = 0;
		free(ht.table);
		ht.numberOfLists = 0;
		ht.table = NULL;
	}
	
	//muda uma instrução qualquer para um loadI de um valor em um registrador
	void change_to_loadi(opt_iloc_code *code, char *reg, int value){
		char *op = malloc(sizeof(char) * 10);
		sprintf(op, "loadI");
		free(code->instruction.op);
		code->instruction.op = op;
		
		char *constant = malloc(sizeof(char) * 10);
		sprintf(constant, "%d", value);
		
		char *new_register = malloc(sizeof(char) * 10);
		sprintf(new_register, "%s", reg);
		
		free(code->instruction.op1);
		free(code->instruction.op2);
		free(code->instruction.op3);
		code->instruction.op3 = NULL;
		code->instruction.op2 = new_register;
		code->instruction.op1 = constant;
	}
	
	//muda uma instrução qualquer para um nop
	void change_to_nop(opt_iloc_code *code){
		char *op = malloc(sizeof(char) * 10);
		sprintf(op, "nop");
		free(code->instruction.op);
		code->instruction.op = op;
		
		free(code->instruction.op1);
		code->instruction.op1 = NULL;
		free(code->instruction.op2);
		code->instruction.op2 = NULL;
		free(code->instruction.op3);
		code->instruction.op3 = NULL;
	}
	
	//varre todas as instruções, substituindo as instruções de escrita em que se conhece os valores dos operandos por um loadI
	//ou quando a escrita é de uma valor que já está escrito no registrador, remove a instrução
	opt_iloc_code *ptAux = code;
	while(ptAux != NULL){
	
		//loadI
		if(strcmp(ptAux->instruction.op, "loadI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element = get_register(ptAux->instruction.op2);
			
			//verifica se o valor escrita e o que já está no registrador são iguais
			if(element != NULL && strcmp(element->value, ptAux->instruction.op1) == 0){
				//se são iguais, muda a instrução para um nop
				change_to_nop(ptAux);
			}
			else{
				//se não são iguais, insere o novo valor na tabela
				put_register(ptAux->instruction.op2, ptAux->instruction.op1);
				//muda a instrução para loadI
				change_to_loadi(ptAux, ptAux->instruction.op2, atoi(ptAux->instruction.op1));
			}
		}

		//i2i
		if(strcmp(ptAux->instruction.op, "i2i") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(element1->value, element2->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op2, element2->value);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op2, atoi(element2->value));
				}
			}
		}
		
		//add
		if(strcmp(ptAux->instruction.op, "add") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) + atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//sub
		if(strcmp(ptAux->instruction.op, "sub") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) - atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//mult
		if(strcmp(ptAux->instruction.op, "mult") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) * atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//div
		if(strcmp(ptAux->instruction.op, "div") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) / atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//and
		if(strcmp(ptAux->instruction.op, "and") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) & atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//or
		if(strcmp(ptAux->instruction.op, "or") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) | atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//xor
		if(strcmp(ptAux->instruction.op, "xor") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) ^ atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//lshift
		if(strcmp(ptAux->instruction.op, "lshift") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) << atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//rshift
		if(strcmp(ptAux->instruction.op, "rshift") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura1 da tabela
			table_element *element2 = get_register(ptAux->instruction.op2);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element3 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL && element3 != NULL){
				int value = atoi(element2->value) >> atoi(element3->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//inc
		if(strcmp(ptAux->instruction.op, "inc") == 0){
		
			//obtém o registrador de escrita e leitura da tabela
			table_element *element = get_register(ptAux->instruction.op1);
			
			if(element != NULL){
				int value = atoi(element->value) + 1;
				char result[10];
				sprintf(result, "%d", value);
			
				//insere o novo valor na tabela
				put_register(ptAux->instruction.op1, result);
				//muda a instrução para loadI
				change_to_loadi(ptAux, ptAux->instruction.op1, atoi(result));
			}
		}
		
		//dec
		if(strcmp(ptAux->instruction.op, "dec") == 0){
		
			//obtém o registrador de escrita e leitura da tabela
			table_element *element = get_register(ptAux->instruction.op1);
			
			if(element != NULL){
				int value = atoi(element->value) - 1;
				char result[10];
				sprintf(result, "%d", value);
			
				//insere o novo valor na tabela
				put_register(ptAux->instruction.op1, result);
				//muda a instrução para loadI
				change_to_loadi(ptAux, ptAux->instruction.op1, atoi(result));
			}
		}

		//addI
		if(strcmp(ptAux->instruction.op, "addI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) + atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//subI
		if(strcmp(ptAux->instruction.op, "subI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) - atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//multI
		if(strcmp(ptAux->instruction.op, "multI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) * atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//divI
		if(strcmp(ptAux->instruction.op, "divI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) / atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//rsubI
		if(strcmp(ptAux->instruction.op, "rsubI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(ptAux->instruction.op2) - atoi(element2->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//rdivI
		if(strcmp(ptAux->instruction.op, "rdivI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(ptAux->instruction.op2) / atoi(element2->value);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//lshiftI
		if(strcmp(ptAux->instruction.op, "lshiftI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) << atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//rshiftI
		if(strcmp(ptAux->instruction.op, "rshiftI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) >> atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//andI
		if(strcmp(ptAux->instruction.op, "andI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) & atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//orI
		if(strcmp(ptAux->instruction.op, "orI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) | atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		//xorI
		if(strcmp(ptAux->instruction.op, "xorI") == 0){
		
			//obtém o registrador de escrita da tabela
			table_element *element1 = get_register(ptAux->instruction.op3);
			
			//obtém o registrador de leitura2 da tabela
			table_element *element2 = get_register(ptAux->instruction.op1);
			
			if(element2 != NULL){
				int value = atoi(element2->value) ^ atoi(ptAux->instruction.op2);
				char result[10];
				sprintf(result, "%d", value);
			
				//verifica se o valor escrita e o que já está no registrador são iguais
				if(element1 != NULL && strcmp(result, element1->value) == 0){
					//se são iguais, muda a instrução para um nop
					change_to_nop(ptAux);
				}
				else{
					//se não são iguais, insere o novo valor na tabela
					put_register(ptAux->instruction.op3, result);
					//muda a instrução para loadI
					change_to_loadi(ptAux, ptAux->instruction.op3, atoi(result));
				}
			}
		}
		
		ptAux = ptAux->next;
	}
	
	clear_table_content();
	
	typedef struct {
		opt_iloc_code *instruction;
		int num_uses;
	} table_element_2;
	
	void free_table_element_2(table_element_2 *element){
		free(element);
	}
	
	table_element_2 *new_table_element_2(opt_iloc_code *instruction, int uses){
		table_element_2 *element = malloc(sizeof(table_element_2));
		if(element == NULL) return NULL;
		element->instruction = instruction;
		element->num_uses = uses;
		return element;
	}
	
	//obtém um registrador na tabela
	table_element_2 *get_register_2(char* key){
		if(_gst_searchKey(ht, key) != NULL){
			table_element_2 *element = (table_element_2 *)_gst_getData(ht, key);
			return element;
		}
		return NULL;
	}

	//insere um registrador na tabela
	void put_register_2(char* key, opt_iloc_code *instruction, int uses){
		if(_gst_searchKey(ht, key) != NULL){
			table_element_2 *tmpElement = (table_element_2 *)_gst_getData(ht, key);
			free_table_element_2(tmpElement);
			
			table_element_2 *element = new_table_element_2(instruction, uses);
			_gst_updateKey(ht, key, element);
		}
		else{
			table_element_2 *element = new_table_element_2(instruction, uses);
			_gst_insertKey(&ht, key, element);
		}
	}
	
	//varre todas as instruções, inserindo todos os registradores que são escritos utilizando a instrução loadI em uma tabela e incrementando a cada uso
	ptAux = code;
	while(ptAux != NULL){
		if(strcmp(ptAux->instruction.op, "loadI") == 0){
			table_element_2 *element = get_register_2(ptAux->instruction.op2);
			if(element != NULL){
				put_register_2(ptAux->instruction.op2, element->instruction, element->num_uses + 1);
			}
			else{
				put_register_2(ptAux->instruction.op2, ptAux, 0);
			}
		}
		
		else{
			table_element_2 *element = get_register_2(ptAux->instruction.op1);
			if(element != NULL){
				put_register_2(ptAux->instruction.op1, element->instruction, element->num_uses + 1);
			}
			
			element = get_register_2(ptAux->instruction.op2);
			if(element != NULL){
				put_register_2(ptAux->instruction.op2, element->instruction, element->num_uses + 1);
			}
			
			element = get_register_2(ptAux->instruction.op3);
			if(element != NULL){
				put_register_2(ptAux->instruction.op3, element->instruction, element->num_uses + 1);
			}
		}

		ptAux = ptAux->next;
	}
	
	//as instruções loadI de todos os registradores da tabela com nenhum uso são transformadas em nop
	_gst_hash_table_node *node, *tempNode;
	int i;
	for(i = 0; i < ht.numberOfLists; i++) {
		node = ht.table[i];
		while(node != NULL){
			tempNode = node;
			node = node->next;
			
			table_element_2 *element = (table_element_2 *)tempNode->item->data;
			if(element->num_uses == 0){
				change_to_nop(element->instruction);
			}
		}
	}

	for(i = 0; i < ht.numberOfLists; i++) {
		node = ht.table[i];
		while(node != NULL){
			tempNode = node;
			node = node->next;
			free_table_element_2((table_element_2 *)tempNode->item->data);
			free(tempNode->item->key);
			free(tempNode->item);
			free(tempNode);
		}
	}
	free(ht.table);
}

void remove_nops(){
	//remove os nops do código
	opt_iloc_code *ptAux = code;
	opt_iloc_code *lastInstruction = NULL;
	while(ptAux != NULL){
		
		if(strcmp(ptAux->instruction.op, "nop") == 0 && ptAux->instruction.label == NULL){ //nop sem rótulo
			if(lastInstruction != NULL){
				lastInstruction->next = ptAux->next;
				free(ptAux->instruction.label);
				free(ptAux->instruction.op);
				free(ptAux->instruction.op1);
				free(ptAux->instruction.op2);
				free(ptAux->instruction.op3);
				free(ptAux);
			}
			else{
				lastInstruction = ptAux;
			}
		}
		else{
			lastInstruction = ptAux;
		}	
		ptAux = lastInstruction->next;
	}
}
