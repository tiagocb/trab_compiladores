#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

void printAST(comp_tree_t *ast);
int read_iloc_file(char *filename);
void optimize();

void yyerror (char const *mensagem) {
	printf("Erro de sintaxe na linha %d\n", obtemLinhaAtual());
}

int main (int argc, char **argv) {
	if(argc > 3){
		printf("Optimize: ./main iloc_file [-O0 | -O1]\n");
		printf("Compile + optimize: ./main [-O0 | -O1] < iks_file\n");
		return 1;
	}
	
	//Obtém o nível de otimização
	int optimization_level = 1;
	int opt_level_set = 0;
	int i;
	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			opt_level_set = i;
			optimization_level = atoi(argv[opt_level_set] + 2) + 1;
		}
	}
	
	//Lê arquivo ILOC
	if(argc == 2 && opt_level_set == 0) read_iloc_file(argv[1]);
	if(argc == 3){
		int pos = (opt_level_set == argc - 1 ? argc - 2: argc - 1);
		read_iloc_file(argv[pos]);
	}
	
	//Compila código IKS
	if(argc == 1 || (argc == 2 && opt_level_set == 1)){
		int resultado =  yyparse();
		switch(resultado){
			default:
			case 0: break;
			case 1: exit(IKS_SYNTAX_ERROR); break;
			case 2: printf("Exaustao da memoria.\n"); exit(IKS_MEMORY_ERROR); break;
		}
		
		//Converte código ILOC para código estruturado
		iloc_code *codeAux = ast->code;
		while(codeAux != NULL){
			opt_iloc_add_instruction(codeAux->operation);
			codeAux = codeAux->next;
		}
	}
	
	int j;
	for(j = 0; j < optimization_level; j++)
		optimize();
		
	opt_iloc_code_print();
	exit(IKS_SUCCESS);
}

void printAST(comp_tree_t *ast){
	void printASTRecursive(comp_tree_t *ast){
		if(ast == NULL) return;

		comp_tree_t *ptAux = ast;
		while (ptAux != NULL) {
			if(ptAux->value == IKS_AST_FUNCAO || ptAux->value == IKS_AST_IDENTIFICADOR || ptAux->value == IKS_AST_LITERAL)
				gv_declare(ptAux->value, ptAux, ptAux->dictPointer->key);
			else
				gv_declare(ptAux->value, ptAux, NULL);

			if(ptAux->parent != NULL) gv_connect(ptAux->parent, ptAux);
			printASTRecursive(ptAux->child);
			ptAux = ptAux->brother;
		}
		return;
	}

	gv_init(NULL);
	printASTRecursive(ast);
	gv_close();
}

int read_iloc_file(char *filename){
		//Preenche estrutura de dados a partir de código ILOC
		FILE *file = fopen(filename, "r");
		if(!file) return 1;
		char instruction[100];
		while(fgets(instruction, 100, file) != NULL){
			opt_iloc_add_instruction(instruction);
		}
		fclose(file);
		return 0;
}

void optimize(){
	use_machine_language();
	algebric_simplifications();
	control_flow_optimizations();
	propagate_copies();
	remove_redundant_instructions_and_evaluate_constant_operations();
	remove_nops();
}

