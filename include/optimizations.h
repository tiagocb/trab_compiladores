#include "hash_table.h"

typedef struct {
	char *label;
	char *op;
	char *op1;
	char *op2;
	char *op3;
} opt_iloc_instruction;

typedef struct _opt_iloc_code {
	opt_iloc_instruction instruction;
	struct _opt_iloc_code *next;
} opt_iloc_code;

//Operações de construção do código iloc estruturado
void opt_iloc_add_instruction(char *instruction);
void opt_iloc_code_print();

//Técnicas de otimização
void use_machine_language();
void algebric_simplifications();
void control_flow_optimizations();
void propagate_copies();
void remove_redundant_instructions_and_evaluate_constant_operations();
void remove_nops();

//Classes de instruções
char *RW_instructions[] = {"add", "sub", "mult", "div", "addI", "subI", "rsubI", "multI", "divI", "rdivI", "lshift", "lshiftI", "rshift", "rshiftI", "and", "andI", "or", "orI", "xor", "xorI", "loadI", "load", "loadAI", "loadAO", "cload", "cloadAI", "cloadAO", "i2i", "c2c", "c2i", "i2c", "cmp_LT", "cmp_LE", "cmp_EQ", "cmp_GE", "cmp_GT", "cmp_NE", "inc", "dec"};
int num_RW_instructions = 39;
char *J_instructions[] = {"jump", "jumpI", "cbr"};
int num_J_instructions = 3;
char *zero_operands_instructions[] = {"nop"};
int num_zero_operands_instructions = 1;
char *one_operand_instructions[] = {"inc", "dec", "jump", "jumpI"};
int num_one_operand_instructions = 4;
char *two_operands_instructions[] = {"loadI", "load", "cload", "store", "cstore", "i2i", "c2c", "c2i", "i2c"};
int num_two_operands_instructions = 9;
char *three_operands_instructions[] = {"add", "sub", "mult", "div", "addI", "subI", "rsubI", "multI", "divI", "rdivI", "lshift", "lshiftI", "rshift", "rshiftI", "and", "andI", "or", "orI", "xor", "xorI", "loadAI", "loadAO", "cloadAI", "cloadAO", "storeAI", "storeAO", "cstoreAI", "cstoreAO", "cbr", "cmp_LT", "cmp_LE", "cmp_EQ", "cmp_GE", "cmp_GT", "cmp_NE"};
int num_three_operands_instructions = 35;

//Operações de testes sobre as classes
int is_from_class(char *op, char *instruction_class[], int class_size){
	int i;
	for(i = 0; i < class_size; i++)
		if(strcmp(op, instruction_class[i]) == 0)
			return 1;
	return 0;
}		
