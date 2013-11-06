#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void printAST(comp_tree_t *ast);

void yyerror (char const *mensagem) {
	printf("Erro de sintaxe na linha %d\n", obtemLinhaAtual());
}

int main (int argc, char **argv) {
	int resultado =  yyparse();

	switch(resultado){
		default:
		case 0:
				//printf("Sucesso.\n");
				break;
		case 1: exit(IKS_SYNTAX_ERROR); break;
		case 2: printf("Exaustao da memoria.\n"); exit(IKS_MEMORY_ERROR); break;
	}
	
	//'tabelaDeSimbolosEscopoGlobal' eh um ponteiro para a tabela de símbolos global. A partir dela, pode-se acessar as outras tabelas
	//printDictionary(*tabelaDeSimbolosEscopoGlobal);
	
	//O ponteiro 'ast' contém a AST construída 
	//printAST(ast);
	printCode(ast->code);
	
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

