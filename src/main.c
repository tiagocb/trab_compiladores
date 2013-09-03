#include <stdio.h>
#include <stdlib.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"

#define IKS_SYNTAX_SUCESSO		0
#define IKS_SYNTAX_ERRO			1

void yyerror (char const *mensagem) {
	fprintf(stderr, "%s. Line %d\n", mensagem, obtemLinhaAtual());
}

int main (int argc, char **argv) {
	inicializaTabelaDeSimbolos();
	/* A tabela de símbolos está definida em scanner.l.
	 * Ela associa uma string a dois inteiros (lexema, linha que foi encontrado e tipo do lexema).
	 * Na tabela de símbolos as strings devem ser únicas portando não há mais que uma ocorrência de um literal ou identificador.
	 *
	 * As operações sobre a tabela de símbolos são feitas pelas seguintes funções definidas em scanner.l
	 * void inicializaTabelaDeSimbolos();
	 * void imprimeTabelaDeSimbolos(); //cuidado, muita informacao sera imprimida
	 * outras em breve
	 */

	switch ( yyparse() ) {
		default:
		case 0: // SUCCESS parsing
			//imprimeTabelaDeSimbolos(); //Verás os identificadores e literais do código compilado
			exit(IKS_SYNTAX_SUCESSO);
		break;

		case 1: // ERROR: input is incorrect and error recovery is impossible
			exit(IKS_SYNTAX_ERRO);
		break;

		case 2: // ERROR: memory exhaustion
			exit(IKS_SYNTAX_ERRO);
		break;
	}
}
