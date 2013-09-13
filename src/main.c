#include <stdio.h>
#include <stdlib.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "main.h"

void generateASTFile(comp_tree_t *ast);

void yyerror (char const *mensagem) {
	fprintf(stderr, "%s. Line %d\n", mensagem, obtemLinhaAtual());
}

int main (int argc, char **argv) {
	createDictionaty(&tabelaDeSimbolos, 10);//Cria tabela de símbolos

	int resultado =  yyparse();

	//printDictionary(tabelaDeSimbolos);
	//printTree(ast);

	switch(resultado){
		default:
		case 0: // SUCCESS parsing

			gv_init(NULL);
			generateASTFile(ast);
			gv_close();

			exit(IKS_SYNTAX_SUCESSO); break;
		case 1: // ERROR: input is incorrect and error recovery is impossible
			exit(IKS_SYNTAX_ERRO); break;
		case 2: // ERROR: memory exhaustion
			exit(IKS_SYNTAX_ERRO); break;
	}
}

void generateASTFile(comp_tree_t *ast){
	if(ast == NULL) return;

	comp_tree_t *ptAux = ast;

	while (ptAux != NULL) {
		if(ptAux->value == IKS_AST_FUNCAO || ptAux->value == IKS_AST_IDENTIFICADOR || ptAux->value == IKS_AST_LITERAL)
			gv_declare(ptAux->value, ptAux, ptAux->dictPointer->key);
		else
			gv_declare(ptAux->value, ptAux, NULL);

		if(ptAux->parent != NULL) gv_connect(ptAux->parent, ptAux);

		generateASTFile(ptAux->child);

		ptAux = ptAux->brother;
	}

	return;
}

