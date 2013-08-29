%{
#include <stdio.h>
%}

/* Declaração dos tokens da gramática da Linguagem K */
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%

/* Programa */

programa :	decl_var_global programa
		| funcao programa
		| /* VAZIO */;

/* Declarações globais */
decl_var_global :	decl_var_simples ';'
			| decl_var_vetor ';';

decl_var_simples :	tipo ':' TK_IDENTIFICADOR;
decl_var_vetor :	tipo ':' TK_IDENTIFICADOR '[' TK_LIT_INT ']';

/* Definições de funções */
funcao :	cabecalho lista_decl_var_local corpo;
cabecalho :	tipo ':' TK_IDENTIFICADOR '(' /* VAZIO */ ')'
		| tipo ':' TK_IDENTIFICADOR '(' lista_de_parametros ')';
lista_de_parametros :	decl_var_simples
			| decl_var_simples ',' lista_de_parametros;
lista_decl_var_local:	/* VAZIO */
			| decl_var_simples ';' lista_decl_var_local;
corpo :	bloco_de_comando;

/* Bloco de comandos e comando */
bloco_de_comando :	'{' lista_de_comandos '}';
lista_de_comandos :	/* VAZIO */
			| comando ';' lista_de_comandos;
comando :	bloco_de_comando
		| atribuicao
		| controle_fluxo
		| op_entrada
		| op_saida
		| op_retorno
		| chamada_funcao
		| /* VAZIO */;

/* Atribuição, entrada, saída e retorno */
atribuicao :	var_simples '=' expressao
		| var_vetor '=' expressao;
op_entrada :	TK_PR_INPUT var;
op_saida :	TK_PR_OUTPUT lista_elementos_saida;
lista_elementos_saida :	TK_LIT_STRING
			| expressao
			| TK_LIT_STRING ',' lista_elementos_saida
			| expressao ',' lista_elementos_saida;
op_retorno :	TK_PR_RETURN expressao;

/* Expressões */
/*
	Por enquanto, uma expressão pode ser booleana ou aritmética. Também pode ser um misto das duas.
	As expressões de caracteres e inteiras ainda não estão definidas
*/

expressao :	literal
		| var
		| chamada_funcao
		| '(' expressao ')'
		| expressao '+' expressao
		| expressao '-' expressao
		| expressao '/' expressao
		| expressao '*' expressao
		| expressao TK_OC_LE expressao
		| expressao TK_OC_GE expressao
		| expressao TK_OC_EQ expressao
		| expressao TK_OC_NE expressao
		| expressao TK_OC_AND expressao
		| expressao TK_OC_OR expressao;

/*
expressao_aritmetica: 	'(' expressao_aritmetica ')'
			| expressao_aritmetica '+' expressao_aritmetica
			| expressao_aritmetica '-' expressao_aritmetica
			| expressao_aritmetica '/' expressao_aritmetica
			| expressao_aritmetica '*' expressao_aritmetica
			| var
			| TK_LIT_INT
			| TK_LIT_FLOAT
			| chamada_funcao;

expressao_logica: 	'(' expressao_logica ')'
			| expressao_logica TK_OC_AND expressao_logica
			| expressao_logica TK_OC_OR expressao_logica
			| var
			| TK_LIT_FALSE
			| TK_LIT_TRUE
			| chamada_funcao
			| expressao_aritmetica TK_OC_LE expressao_aritmetica
			| expressao_aritmetica TK_OC_GE expressao_aritmetica
			| expressao_aritmetica TK_OC_EQ expressao_aritmetica
			| expressao_aritmetica TK_OC_NE expressao_aritmetica;

expressao_inteira :	TK_LIT_INT; //Será definida nas próximas etapas
expressao_caracteres	: TK_LIT_CHAR; //Será definida nas próximas etapas

*/

/* Chamada de uma função */
chamada_funcao :	TK_IDENTIFICADOR '(' /* VAZIO */ ')'
			| TK_IDENTIFICADOR '(' lista_de_argumentos ')';
lista_de_argumentos :	expressao
			| expressao ',' lista_de_argumentos;

/* Controle de fluxo */
controle_fluxo:	TK_PR_IF '(' expressao ')' TK_PR_THEN comando
		| TK_PR_IF '(' expressao ')' TK_PR_THEN comando TK_PR_ELSE comando
		| TK_PR_WHILE '(' expressao ')' TK_PR_DO comando
		| TK_PR_DO comando TK_PR_WHILE '(' expressao ')';

/* Literais */
literal :	TK_LIT_FALSE
		| TK_LIT_TRUE
		| TK_LIT_INT
		| TK_LIT_FLOAT
		| TK_LIT_CHAR
		| TK_LIT_STRING;

/* Variáveis */
var :	var_simples
	| var_vetor;
var_simples :	TK_IDENTIFICADOR;
var_vetor :	TK_IDENTIFICADOR '[' expressao ']';

/* Tipos */
tipo :	TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_CHAR
	| TK_PR_BOOL
	| TK_PR_STRING;

%%
