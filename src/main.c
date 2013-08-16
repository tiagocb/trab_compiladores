#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

int getLineNumber (void)
{
	/* deve ser implementada */
	return obtemLinhaAtual();
}

int main (int argc, char **argv)
{
	inicializaTabelaDeSimbolos();
	//a tabela de símbolos está definida em scanner.l
	//ela associa uma string a um inteiro
	//no momento ela armazena identificadores e literais do tipo int, float, char e string
	//ela armazena o identificador ou o próprio literal como string e o número da linha onde foi encontrado como o inteiro.
	//na tabela de símbolos as strings devem ser únicas portando não há mais que uma ocorrência de um literal ou identificador
	//Importante: quando o analizador léxico identifica um dos elementos acima, ele já insere na tabela de símbolos.

	//as operações sobre a tabela de símbolos são feitas pelas seguintes funções definidas em scanner.l
	// void inicializaTabelaDeSimbolos();
	// void insereLexema();
	// void imprimeTabelaDeSimbolos(); //cuidado, muita informacao sera imprimida
	// outras em breve

	int token = TOKEN_ERRO;
	while (token = yylex()){
		printf ("token <%d> at %d\n", token, getLineNumber());
	}

	return 0;
}
