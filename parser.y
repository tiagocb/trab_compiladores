%{
#include <stdio.h>
#include "main.h"

comp_tree_t *createRoot(int value);
%}

%union{
	void *symbol;
	void *ast_type;
	int tipo;
};

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
%token<symbol> TK_LIT_INT
%token<symbol> TK_LIT_FLOAT
%token<symbol> TK_LIT_FALSE
%token<symbol> TK_LIT_TRUE
%token<symbol> TK_LIT_CHAR
%token<symbol> TK_LIT_STRING
%token<symbol> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type<ast_type> op_entrada var var_simples var_vetor literal expressao chamada_funcao lista_de_argumentos
%type<ast_type> op_retorno inicio programa decl_funcao corpo bloco_de_comando lista_de_comandos ultimo_comando
%type<ast_type> comando atribuicao op_saida lista_elementos_saida controle_fluxo

%type<symbol> cabecalho decl_var

%type<tipo> tipo

%left TK_OC_AND TK_OC_OR
%left '!'
%left '<' '>' TK_OC_NE TK_OC_LE TK_OC_GE TK_OC_EQ
%left '&' '|'
%left '+' '-'
%left '*' '/'

%right "then" TK_PR_ELSE

%%

inicio:	programa	{ $$ = createRoot(IKS_AST_PROGRAMA); appendOnChildPointer($$, $1); ast = $$;};

/* O programa é uma sequência de declarações globais (com um ';' no final) e declarações de funções */
programa:	decl_var_global ';' programa	{ $$ = $3; }
		| decl_funcao programa		{ $$ = $1; appendOnBrotherPointer($$, $2); }
		| /* VAZIO */			{ $$ = NULL };


/* Uma declaração global pode ser de um vetor ou de uma variável simples.
 * A declaração é composta pelo tipo da variável, dois pontos, seu nome e, se ela for um vetor, o tamanho do vetor.
 */
decl_var_global:	decl_var
			| decl_var '[' TK_LIT_INT ']';
decl_var:		tipo ':' TK_IDENTIFICADOR	{
								comp_dict_item_t *item = (comp_dict_item_t *)$3;
			  					if(item != NULL){;
									switch($1){
										case 0: item->type = IKS_SIMBOLO_INT; break;
										case 1: item->type = IKS_SIMBOLO_FLOAT; break;
										case 2: item->type = IKS_SIMBOLO_CHAR; break;
										case 3: item->type = IKS_SIMBOLO_BOOL; break;
										case 4: item->type = IKS_SIMBOLO_STRING; break;
									}
								}
								$$ = item;
							};


/* Uma função é composta por um cabeçalho, uma lista de parâmetros entre parenteses, uma lista de declarações locais e um corpo.
 * O cabeçalho é composto por um tipo, dois pontos e o nome da função (é como uma declaração de variável).
 * A lista de parâmetros é composta por declarações de variáveis simples separadas por vírgula.
 * A lista de declarações locais é composta por declarações de variáveis simples com um ';' no fim de cada declaração.
 * O corpo é um bloco de comando.
 */
decl_funcao:		cabecalho '(' lista_de_parametros ')' lista_decl_var_locais corpo	{ $$ = createRoot(IKS_AST_FUNCAO); ((comp_tree_t *)$$)->dictPointer = $1; appendOnChildPointer($$, $6); }
			| cabecalho '(' /* VAZIO */ ')' lista_decl_var_locais corpo		{ $$ = createRoot(IKS_AST_FUNCAO); ((comp_tree_t *)$$)->dictPointer = $1; appendOnChildPointer($$, $5); };
cabecalho:		decl_var	{ $$ = $1; };
lista_de_parametros:	decl_var ',' lista_de_parametros
			| ultimo_parametro;
ultimo_parametro:	decl_var;
lista_decl_var_locais:	decl_var ';' lista_decl_var_locais
			| /* VAZIO */;
corpo:			'{' lista_de_comandos '}'	{ $$ = $2; };


/* Um bloco de comandos é uma sequência de comandos entre chaves */
bloco_de_comando:	'{' lista_de_comandos '}'	{ $$ = createRoot(IKS_AST_BLOCO); appendOnChildPointer($$, $2);};


/* Uma lista de comandos é uma sequência de comandos separados por vírgula.
 * Não é necessário uma vírgula após o último comando da sequência.
 */
lista_de_comandos:	comando ';' lista_de_comandos	{ $$ = $1; appendOnBrotherPointer($$, $3); }
			| ';' lista_de_comandos		{ $$ = $2; }
			| ultimo_comando		{ $$ = $1; };
ultimo_comando:		comando				{ $$ = $1; }
			| /* VAZIO */			{ $$ = NULL; };


/* Um comando pode ser: Vazio, uma chamada de função, uma operação de retorno, uma operação de saída, uma operação de entrada, 
 * um comando de controle de fluxo, uma atribuição ou um bloco de comando.
 */
comando:	bloco_de_comando	{ $$ = $1; }
		| atribuicao		{ $$ = $1; }
		| controle_fluxo	{ $$ = $1; }
		| op_entrada		{ $$ = $1; }
		| op_saida		{ $$ = $1; }
		| op_retorno		{ $$ = $1; }
		| chamada_funcao	{ $$ = $1; };


/* Atribuição, entrada, saída e retorno */
atribuicao:		var_simples	'=' expressao		{ $$ = createRoot(IKS_AST_ATRIBUICAO); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
			| var_vetor	'=' expressao		{ $$ = createRoot(IKS_AST_ATRIBUICAO); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); };
op_entrada:		TK_PR_INPUT var	 			{ $$ = createRoot(IKS_AST_INPUT); appendOnChildPointer($$, $2);};
op_retorno:		TK_PR_RETURN expressao			{ $$ = createRoot(IKS_AST_RETURN); appendOnChildPointer($$, $2);};
op_saida:		TK_PR_OUTPUT lista_elementos_saida	{ $$ = createRoot(IKS_AST_OUTPUT); appendOnChildPointer($$, $2);}
			| TK_PR_OUTPUT /* VAZIO */		{ $$ = createRoot(IKS_AST_OUTPUT); };
lista_elementos_saida:	expressao				{ $$ = $1; }
			| expressao ',' lista_elementos_saida	{ $$ = $1; appendOnBrotherPointer($$, $3); };


/* Uma expressão tem como folha uma variável, um literal ou uma chamada de função.
 */
expressao:	var				{ $$ = $1; }
		| literal			{ $$ = $1; }
		| chamada_funcao		{ $$ = $1; }
		| '(' expressao ')'		{ $$ = $2; }
		| '-' expressao			{ $$ = createRoot(IKS_AST_ARIM_INVERSAO); appendOnChildPointer($$, $2); }
		| '!' expressao			{ $$ = createRoot(IKS_AST_LOGICO_COMP_NEGACAO); appendOnChildPointer($$, $2); }
		| expressao '+' expressao	{ $$ = createRoot(IKS_AST_ARIM_SOMA); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao '-' expressao	{ $$ = createRoot(IKS_AST_ARIM_SUBTRACAO); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao '/' expressao	{ $$ = createRoot(IKS_AST_ARIM_DIVISAO); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao '*' expressao	{ $$ = createRoot(IKS_AST_ARIM_MULTIPLICACAO); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao '<' expressao	{ $$ = createRoot(IKS_AST_LOGICO_COMP_L); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao '>' expressao	{ $$ = createRoot(IKS_AST_LOGICO_COMP_G); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao TK_OC_LE expressao	{ $$ = createRoot(IKS_AST_LOGICO_COMP_LE); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao TK_OC_GE expressao	{ $$ = createRoot(IKS_AST_LOGICO_COMP_GE); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao TK_OC_EQ expressao	{ $$ = createRoot(IKS_AST_LOGICO_COMP_IGUAL); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao TK_OC_NE expressao	{ $$ = createRoot(IKS_AST_LOGICO_COMP_DIF); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao TK_OC_OR expressao	{ $$ = createRoot(IKS_AST_LOGICO_OU); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); }
		| expressao TK_OC_AND expressao	{ $$ = createRoot(IKS_AST_LOGICO_E); appendOnChildPointer($$, $1); appendOnChildPointer($$, $3); };


/* Chamada de uma função */
chamada_funcao:		TK_IDENTIFICADOR '(' lista_de_argumentos ')'	{ $$ = createRoot(IKS_AST_CHAMADA_DE_FUNCAO); appendOnChildPointer($$, createRoot(IKS_AST_IDENTIFICADOR)); ((comp_tree_t *)$$)->child->dictPointer = (comp_dict_item_t *)$1; appendOnChildPointer($$, $3);}
			| TK_IDENTIFICADOR '(' /* VAZIO */ ')'		{ $$ = createRoot(IKS_AST_CHAMADA_DE_FUNCAO); appendOnChildPointer($$, createRoot(IKS_AST_IDENTIFICADOR)); ((comp_tree_t *)$$)->child->dictPointer = (comp_dict_item_t *)$1; };
lista_de_argumentos:	expressao ',' lista_de_argumentos		{ $$ = $1; appendOnBrotherPointer($$, $3); }
			| expressao					{ $$ = $1; };

/* Controle de fluxo */
controle_fluxo:	TK_PR_IF '(' expressao ')' TK_PR_THEN comando	%prec "then"		{ $$ = createRoot(IKS_AST_IF_ELSE); appendOnChildPointer($$, $3); appendOnChildPointer($$, $6); appendOnChildPointer($$, NULL); }
		| TK_PR_IF '(' expressao ')' TK_PR_THEN ';'	%prec "then"		{ $$ = createRoot(IKS_AST_IF_ELSE); appendOnChildPointer($$, $3); appendOnChildPointer($$, NULL); appendOnChildPointer($$, NULL); }
		| TK_PR_IF '(' expressao ')' TK_PR_THEN	comando TK_PR_ELSE comando	{ $$ = createRoot(IKS_AST_IF_ELSE); appendOnChildPointer($$, $3); appendOnChildPointer($$, $6); appendOnChildPointer($$, $8); }
		| TK_PR_IF '(' expressao ')' TK_PR_THEN	';' TK_PR_ELSE comando		{ $$ = createRoot(IKS_AST_IF_ELSE); appendOnChildPointer($$, $3); appendOnChildPointer($$, NULL); appendOnChildPointer($$, $8); }
		| TK_PR_IF '(' expressao ')' TK_PR_THEN	comando TK_PR_ELSE ';'		{ $$ = createRoot(IKS_AST_IF_ELSE); appendOnChildPointer($$, $3); appendOnChildPointer($$, $6); appendOnChildPointer($$, NULL); }
		| TK_PR_IF '(' expressao ')' TK_PR_THEN	';' TK_PR_ELSE ';'		{ $$ = createRoot(IKS_AST_IF_ELSE); appendOnChildPointer($$, $3); appendOnChildPointer($$, NULL); appendOnChildPointer($$, NULL); }

		| TK_PR_WHILE '(' expressao ')'	TK_PR_DO comando	{ $$ = createRoot(IKS_AST_WHILE_DO); appendOnChildPointer($$, $3); appendOnChildPointer($$, $6); }
		| TK_PR_WHILE '(' expressao ')'	TK_PR_DO ';'		{ $$ = createRoot(IKS_AST_WHILE_DO); appendOnChildPointer($$, $3); appendOnChildPointer($$, NULL);}
		| TK_PR_DO comando TK_PR_WHILE '(' expressao ')'	{ $$ = createRoot(IKS_AST_DO_WHILE); appendOnChildPointer($$, $2); appendOnChildPointer($$, $5); }
		| TK_PR_DO ';' TK_PR_WHILE '(' expressao ')'		{ $$ = createRoot(IKS_AST_DO_WHILE); appendOnChildPointer($$, NULL);  appendOnChildPointer($$, $5); };

/* Literais */
literal:	TK_LIT_FALSE	{ $$ = createRoot(IKS_AST_LITERAL); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1; }
		| TK_LIT_TRUE	{ $$ = createRoot(IKS_AST_LITERAL); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1; }
		| TK_LIT_INT	{ $$ = createRoot(IKS_AST_LITERAL); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1; }
		| TK_LIT_FLOAT	{ $$ = createRoot(IKS_AST_LITERAL); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1; }
		| TK_LIT_CHAR	{ $$ = createRoot(IKS_AST_LITERAL); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1; }
		| TK_LIT_STRING	{ $$ = createRoot(IKS_AST_LITERAL); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1; };

/* Variáveis */
var:	var_simples					{ $$ = $1;}
	| var_vetor					{ $$ = $1;};
var_simples:	TK_IDENTIFICADOR			{ $$ = createRoot(IKS_AST_IDENTIFICADOR); ((comp_tree_t *)$$)->dictPointer = (comp_dict_item_t *)$1;}; 
var_vetor:	TK_IDENTIFICADOR '[' expressao ']'	{ $$ = createRoot(IKS_AST_VETOR_INDEXADO); appendOnChildPointer($$, createRoot(IKS_AST_IDENTIFICADOR)); ((comp_tree_t *)$$)->child->dictPointer = (comp_dict_item_t *)$1; appendOnChildPointer($$, $3); };

/* Tipos */
tipo:	TK_PR_INT	{$$ = 0;}
	| TK_PR_FLOAT	{$$ = 1;}
	| TK_PR_CHAR	{$$ = 2;}
	| TK_PR_BOOL	{$$ = 3;}
	| TK_PR_STRING	{$$ = 4;};

%%

comp_tree_t *createRoot(int value){
	comp_tree_t **root;
	createTree(root);
	insert(root, value, 1, 0);
	return *root;
}
