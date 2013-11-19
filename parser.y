%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include "main.h"
	#include "register_generator.h"
	#include "label_generator.h"
	#include "comp_stack.h"
	
	//Dados auxiliares na leitura de um uso de vetor multidimensional
	typedef struct {
		int dimensionCounter;
		comp_dict_item_t *vectorSymbol;
		comp_tree_t *vectorNode;
		comp_list_t *resultsRegisters;
	} VectorReadingInfo;
	
	comp_stack_t *vectorStack;

	//Deslocamentos de escopo local e global para definir endereço das variáveis/vetores
	int deslocamentoEscopoGlobal;
	int deslocamentoEscopoLocal;

	//Ponteiro para a tabela de símbolos que está sendo utilizada
	comp_dict_t *tabelaDeSimbolosAtual;

	//Variáveis auxiliares na leitura de uma declaração de um vetor multidimensional
	comp_dict_item_t *simboloVetor;
	comp_list_t *listaDeDimensoes;


	comp_dict_item_t *simboloFuncao;

	comp_dict_item_t *simboloDaFuncaoSendoChamada;
	comp_list_t *listaDeParametrosSendoLida;


	//Funções auxiliares
	comp_tree_t *createRoot(int value);
	int semanticError(int errorType, char *format, ...);
%}

%union{
	char *symbol;
	void *symbolTableElement;
	void *ast_type;
	int tipo;
	void *dimensionList;
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


%type<ast_type> do_while while_do if_then_else if_then
%type<ast_type> op_entrada var var_simples var_vetor literal
%type<ast_type> flow_control_command lista_de_dimensoes expressao
%type<ast_type> chamada_funcao lista_de_argumentos op_retorno inicio
%type<ast_type> programa decl_funcao corpo bloco_de_comando lista_de_comandos
%type<ast_type> ultimo_comando comando atribuicao op_saida lista_elementos_saida controle_fluxo

%type<symbolTableElement> cabecalho decl_var
%type<tipo> tipo
%type<symbol> nome_fun
%type<dimensionList> dim_list

//Definição da precedência dos operadores
%left '|'
%left '&'
%left TK_OC_OR
%left TK_OC_AND
%left '!'
%left TK_OC_NE TK_OC_EQ
%left '<' '>' TK_OC_LE TK_OC_GE
%left '+' '-'
%left '*' '/'
%right "then" TK_PR_ELSE

%%


/* Regra inicial */
inicio:	inicializacao programa
								{
									//Cria raiz da AST e adiciona ponteiro para o programa
									comp_tree_t *programa = (comp_tree_t *)$2;
									ast = createRoot(IKS_AST_PROGRAMA);
									appendOnChildPointer(ast, programa);

									//Adiciona o código gerado na raiz da AST
									ast->code = programa->code;
								};

/* Artimanha para inicializar algumas estruturas importantes */
inicializacao: /* VAZIO */
								{	
									//Cria tabela de escopo global
									tabelaDeSimbolosEscopoGlobal = malloc(sizeof(comp_dict_t));
									if(tabelaDeSimbolosEscopoGlobal == NULL) exit(IKS_MEMORY_ERROR);
									createDictionaty(tabelaDeSimbolosEscopoGlobal, 3, NULL);

									//Inicializa ponteiro para a tabela corrente
									tabelaDeSimbolosAtual = tabelaDeSimbolosEscopoGlobal;

									//Inicializa o deslocamento do escopo global e local
									deslocamentoEscopoGlobal = 0;
									deslocamentoEscopoLocal = 0;

									//Inicializa pilha de vetores
									createStack(&vectorStack);
								};
	
/* O programa é uma sequência de declarações de variáveis globais e declarações de funções */
programa:	decl_var_global ';' programa	{ $$ = $3; }
					| decl_funcao programa				{ $$ = $1; appendOnChildPointer($$, $2); }
					| /* VAZIO */									{ $$ = NULL; };


			
















/* Uma declaração global pode ser de um vetor ou de uma variável simples */
decl_var_global:	decl_var
								{
									//Obtém a entrada da tabela de simbolos da variável declarada
									comp_dict_item_t *item = (comp_dict_item_t *)$1;

									//Associa o endereço da variável
									item->address = deslocamentoEscopoGlobal;
									//Associa o número de bytes da variavel na tabela de símbolos e incrementa o deslocamento global
									switch(item->valueType){
										case IKS_INT:			item->numBytes = IKS_INT_SIZE; deslocamentoEscopoGlobal += IKS_INT_SIZE; break;
										case IKS_FLOAT:		item->numBytes = IKS_FLOAT_SIZE; deslocamentoEscopoGlobal += IKS_FLOAT_SIZE; break;
										case IKS_CHAR:		item->numBytes = IKS_CHAR_SIZE; deslocamentoEscopoGlobal += IKS_CHAR_SIZE; break;
										case IKS_STRING:	item->numBytes = IKS_STRING_SIZE; deslocamentoEscopoGlobal += IKS_STRING_SIZE; break;
										case IKS_BOOL:		item->numBytes = IKS_BOOL_SIZE; deslocamentoEscopoGlobal += IKS_BOOL_SIZE; break;
									}
									//Associa o tipo do identificador na tabela de símbolos
									item->nodeType = IKS_VARIABLE_ITEM;
								}

								| decl_var
								{
									//Obtém a entrada da tabela de simbolos da variável declarada
									comp_dict_item_t *item = (comp_dict_item_t *)$1;

									//Associa o endereço da variável
									item->address = deslocamentoEscopoGlobal;
									//Associa o tipo do identificador na tabela de símbolos
									item->nodeType = IKS_VECTOR_ITEM;

									simboloVetor = item;
								}
								dim_list
								{
									//Multiplica todos os valores das dimensoes
									int produtorio = 0;
									comp_list_t *ptAux = simboloVetor->dimensionList;
									while(ptAux != NULL){
										produtorio += (*(int *)ptAux->data);
										ptAux = ptAux->next;
									}
							
									//Associa o número de bytes do vetor na tabela de símbolos e incrementa o deslocamento global
									switch(simboloVetor->valueType){
										case IKS_INT:			simboloVetor->numBytes = produtorio * IKS_INT_SIZE; deslocamentoEscopoGlobal += simboloVetor->numBytes; break;
										case IKS_FLOAT:		simboloVetor->numBytes = produtorio * IKS_FLOAT_SIZE; deslocamentoEscopoGlobal += simboloVetor->numBytes; break;
										case IKS_CHAR:		simboloVetor->numBytes = produtorio * IKS_CHAR_SIZE; deslocamentoEscopoGlobal += simboloVetor->numBytes; break;
										case IKS_STRING:	simboloVetor->numBytes = produtorio * IKS_STRING_SIZE; deslocamentoEscopoGlobal += simboloVetor->numBytes; break;
										case IKS_BOOL:		simboloVetor->numBytes = produtorio * IKS_BOOL_SIZE; deslocamentoEscopoGlobal += simboloVetor->numBytes; break;
									}
								};

dim_list:		'[' TK_LIT_INT ']'
								{
									int *intValue = malloc(sizeof(int));
									*intValue = atoi($2);
									insertHead(&(simboloVetor->dimensionList), intValue);
									free($2);
								}
								| '[' TK_LIT_INT ']' dim_list
								{
									int *intValue = malloc(sizeof(int));
									*intValue = atoi($2);
									insertHead(&(simboloVetor->dimensionList), intValue);
									free($2);
								};

													
													
													
													







/* Declaração de variável/vetor/funcao */
decl_var:	tipo ':' TK_IDENTIFICADOR
								{
									//Verifica se o identificador já foi declarado no escopo atual
									comp_dict_item_t *item = searchKey(*tabelaDeSimbolosAtual, $3);
									if(item != NULL) semanticError(IKS_ERROR_DECLARED, "O identificador '%s' utilizado na declaracao da linha %d ja foi declarado na linha %d.", $3, obtemLinhaAtual(), item->line);
									
									//Insere o identificador na tabela de símbolos
									item = insertKey(tabelaDeSimbolosAtual, $3, IKS_UNDEFINED, obtemLinhaAtual());
									if(item == NULL) exit(IKS_MEMORY_ERROR);

									//Libera memória associada à string do identificador
									free($3);

									//Associa o seu tipo do valor do identificador na tabela de símbolos
									item->valueType = $1;

									$$ = item;
								};
							
							
							












/* Uma função é composta por um cabeçalho, parâmetros, declarações locais e um corpo. */
decl_funcao:	cabecalho '(' parametros ')' var_locais corpo
								{
									comp_tree_t *corpo = (comp_tree_t *)$6;

									//Cria nodo do tipo função
									comp_tree_t *funcao = createRoot(IKS_AST_FUNCAO);

									//Associa a entrada da tabela de símbolos e a sub-árvore do corpo no nodo
									funcao->dictPointer = $1;
									appendOnChildPointer($$, corpo);

									//Adiciona o código gerado da função ao nodo
									if(corpo != NULL) funcao->code = corpo->code;

									//Inicializa deslocamento do escopo local
									deslocamentoEscopoLocal = 0;

									$$ = funcao;
								}
									
								| cabecalho '(' /* VAZIO */ ')' var_locais corpo
								{
									comp_tree_t *corpo = (comp_tree_t *)$5;

									//Cria nodo do tipo função
									comp_tree_t *funcao = createRoot(IKS_AST_FUNCAO);

									//Associa a entrada da tabela de símbolos e a sub-árvore do corpo no nodo
									funcao->dictPointer = $1;
									appendOnChildPointer($$, corpo);

									//Adiciona o código gerado da função ao nodo
									if(corpo != NULL) funcao->code = corpo->code;

									//Inicializa deslocamento do escopo local
									deslocamentoEscopoLocal = 0;

									$$ = funcao;
								};

/* O cabeçalho indica o tipo de retorno da funcao e seu nome */
cabecalho:		decl_var
								{
									//Obtém a entrada da tabela de simbolos da função declarada
									comp_dict_item_t *item = (comp_dict_item_t *)$$;

									//Associa o tipo do identificador na tabela de símbolo (escopo global)
									item->nodeType = IKS_FUNCTION_ITEM;

									//Cria uma tabela de símbolos para a função e atualiza ponteiro para tabela de símbolos corrente
									tabelaDeSimbolosAtual = malloc(sizeof(comp_dict_t));
									if(tabelaDeSimbolosAtual == NULL) exit(IKS_MEMORY_ERROR);
	 								createDictionaty(tabelaDeSimbolosAtual, 3, tabelaDeSimbolosEscopoGlobal);

									//Associa a tabela de símbolos da função na entrada da tabela de símbolos
									item->functionSymbolTable = tabelaDeSimbolosAtual;

									simboloFuncao = item;
									$$ = $1;
								};









/* A lista de parâmetros contém uma sequência de declarações de variáveis */					
parametros:	decl_var ',' parametros
								{	
									//Obtém a entrada da tabela de simbolos da função declarada
									comp_dict_item_t *item = (comp_dict_item_t *)$1;

									//Associa o tipo do identificador na tabela de símbolos
									item->nodeType = IKS_VARIABLE_ITEM;

									//Associa o endereço da variável
									item->address = deslocamentoEscopoLocal;

									//Associa o número de bytes da variavel na tabela de símbolos e incrementa o deslocamento local
									switch(item->valueType){
										case IKS_INT:			item->numBytes = IKS_INT_SIZE; deslocamentoEscopoLocal += IKS_INT_SIZE; break;
										case IKS_FLOAT:		item->numBytes = IKS_FLOAT_SIZE; deslocamentoEscopoLocal += IKS_FLOAT_SIZE; break;
										case IKS_CHAR:		item->numBytes = IKS_CHAR_SIZE; deslocamentoEscopoLocal += IKS_CHAR_SIZE; break;
										case IKS_STRING:	item->numBytes = IKS_STRING_SIZE; deslocamentoEscopoLocal += IKS_STRING_SIZE; break;
										case IKS_BOOL:		item->numBytes = IKS_BOOL_SIZE; deslocamentoEscopoLocal += IKS_BOOL_SIZE; break;
									}

									//Insere o tipo do parâmetro na lista de parâmetros na tabela de símbolos
									insertTail(&(simboloFuncao->parametersList), &(item->valueType));
								}
								| ultimo_parametro;

ultimo_parametro:	decl_var
								{
									//Obtém a entrada da tabela de simbolos da função declarada
									comp_dict_item_t *item = (comp_dict_item_t *)$1;

									//Associa o tipo do identificador na tabela de símbolos
									item->nodeType = IKS_VARIABLE_ITEM;

									//Associa o endereço da variável
									item->address = deslocamentoEscopoLocal;

									//Associa o número de bytes da variavel na tabela de símbolos e incrementa o deslocamento local
									switch(item->valueType){
										case IKS_INT:			item->numBytes = IKS_INT_SIZE; deslocamentoEscopoLocal += IKS_INT_SIZE; break;
										case IKS_FLOAT:		item->numBytes = IKS_FLOAT_SIZE; deslocamentoEscopoLocal += IKS_FLOAT_SIZE; break;
										case IKS_CHAR:		item->numBytes = IKS_CHAR_SIZE; deslocamentoEscopoLocal += IKS_CHAR_SIZE; break;
										case IKS_STRING:	item->numBytes = IKS_STRING_SIZE; deslocamentoEscopoLocal += IKS_STRING_SIZE; break;
										case IKS_BOOL:		item->numBytes = IKS_BOOL_SIZE; deslocamentoEscopoLocal += IKS_BOOL_SIZE; break;
									}

									//Insere o tipo do parâmetro na lista de parâmetros na tabela de símbolos
									insertTail(&(simboloFuncao->parametersList), &(item->valueType));
								};

/* Na lista de declarações locais, só é possível declarar variáveis simples */									
var_locais:	decl_var ';' var_locais
								{
									//Obtém a entrada da tabela de simbolos da função declarada
									comp_dict_item_t *item = (comp_dict_item_t *)$1;

									//Associa o tipo do identificador na tabela de símbolos
									item->nodeType = IKS_VARIABLE_ITEM;

									//Associa o endereço da variável
									item->address = deslocamentoEscopoLocal;

									//Associa o número de bytes da variavel na tabela de símbolos e incrementa o deslocamento local
									switch(item->valueType){
										case IKS_INT:			item->numBytes = IKS_INT_SIZE; deslocamentoEscopoLocal += IKS_INT_SIZE; break;
										case IKS_FLOAT:		item->numBytes = IKS_FLOAT_SIZE; deslocamentoEscopoLocal += IKS_FLOAT_SIZE; break;
										case IKS_CHAR:		item->numBytes = IKS_CHAR_SIZE; deslocamentoEscopoLocal += IKS_CHAR_SIZE; break;
										case IKS_STRING:	item->numBytes = IKS_STRING_SIZE; deslocamentoEscopoLocal += IKS_STRING_SIZE; break;
										case IKS_BOOL:		item->numBytes = IKS_BOOL_SIZE; deslocamentoEscopoLocal += IKS_BOOL_SIZE; break;
									}
								}
								| /* VAZIO */;












			
/* O corpo da função é uma lista de comandos entre chaves */
corpo:	'{' lista_de_comandos '}'
								{
									//A tabela de símbolos atual passa a ser a tabela global novamente
									tabelaDeSimbolosAtual = tabelaDeSimbolosEscopoGlobal;

									$$ = $2;
								};

/* Um bloco de comandos é uma sequência de comandos entre chaves */
bloco_de_comando:	'{' lista_de_comandos '}'	
								{
									comp_tree_t *listaDeComandos = (comp_tree_t *)$2;

									//Cria nodo do tipo bloco
									comp_tree_t *bloco = createRoot(IKS_AST_BLOCO);

									//Associa a sub-árvore dos comandos no nodo
									appendOnChildPointer(bloco, $2);

									//Adiciona o código gerado dos comandos ao nodo
									if(listaDeComandos != NULL) bloco->code = listaDeComandos->code;

									$$ = bloco;
								};

/* Uma lista de comandos é uma sequência de comandos separados por ';' (não é necessário um ';' após o último comando da sequência) */
lista_de_comandos:	comando ';' lista_de_comandos	
								{
									comp_tree_t *comando = $1;
									comp_tree_t *comandosSubsequentes = $3;
									appendOnChildPointer(comando, comandosSubsequentes);
									if(comandosSubsequentes != NULL) concatCode(&(comando->code), &(comandosSubsequentes->code));
									$$ = comando;
								}
								| ';' lista_de_comandos		{ $$ = $2; }
								| ultimo_comando					{ $$ = $1; };
										
ultimo_comando:	comando										{ $$ = $1; }
								| /* VAZIO */							{ $$ = NULL; };

/* Um comando pode ser: Uma chamada de função, uma operação de retorno, uma operação de saída, uma operação de entrada, 
 * um comando de controle de fluxo, uma atribuição ou um bloco de comando.
 */
comando:	bloco_de_comando	{ $$ = $1; }
					| atribuicao			{ $$ = $1; }
					| controle_fluxo	{ $$ = $1; }
					| op_entrada			{ $$ = $1; }
					| op_saida				{ $$ = $1; }
					| op_retorno			{ $$ = $1; }
					| chamada_funcao	{ $$ = $1; };














/* Atribuição, entrada, saída e retorno */
atribuicao:				var_simples	'=' expressao
																{
																	//Verifica se o tipo da expressao e o tipo da variável são compatíveis
																	//Se não forem, imprime erro e termina
																	//comp_tree_t *varNode = (comp_tree_t *)$1;
																	//comp_tree_t *expressionNode = (comp_tree_t *)$3;
																	//checkCompatibleTypes(varNode->type, expressionNode->type);

																	switch(((comp_tree_t *)$1)->type){
																		case IKS_INT:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: break;
																				case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao float -> int
																				case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
																				case IKS_CHAR: semanticError(IKS_ERROR_CHAR_TO_X, "Na linha %d, a atribuicao de um char a uma variavel do tipo int eh invalida.", obtemLinhaAtual()); break;
																				case IKS_STRING: semanticError(IKS_ERROR_STRING_TO_X, "Na linha %d, a atribuicao de uma string a uma variavel do tipo int eh invalida.\n", obtemLinhaAtual()); break;
																			} break;
																		case IKS_FLOAT:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_FLOAT; ((comp_tree_t *)$3)->type = IKS_FLOAT; break; //Coercao int -> float
																				case IKS_FLOAT: break;
																				case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_FLOAT; ((comp_tree_t *)$3)->type = IKS_FLOAT; break; //Coercao bool -> float
																				case IKS_CHAR: semanticError(IKS_ERROR_CHAR_TO_X, "Na linha %d, a atribuicao de um char a uma variavel do tipo float eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_STRING: semanticError(IKS_ERROR_STRING_TO_X, "Na linha %d, a atribuicao de uma string a uma variavel do tipo float eh invalida.\n", obtemLinhaAtual()); break;
																			} break;
																		case IKS_BOOL:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
																				case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
																				case IKS_BOOL: break;
																				case IKS_CHAR: semanticError(IKS_ERROR_CHAR_TO_X, "Na linha %d, a atribuicao de um char a uma variavel do tipo bool eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_STRING: semanticError(IKS_ERROR_STRING_TO_X, "Na linha %d, a atribuicao de uma string a uma variavel do tipo bool eh invalida.\n", obtemLinhaAtual()); break;
																			} break;
																		case IKS_CHAR:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: semanticError(IKS_ERROR_WRONG_TYPE, "Na linha %d, a atribuicao de um int a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_FLOAT: semanticError(IKS_ERROR_WRONG_TYPE, "Na linha %d, a atribuicao de um float a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_BOOL: semanticError(IKS_ERROR_WRONG_TYPE, "Na linha %d, a atribuicao de um bool a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_CHAR: break;
																				case IKS_STRING: semanticError(IKS_ERROR_STRING_TO_X, "Na linha %d, a atribuicao de uma string a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); break;
																			} break;
																		case IKS_STRING:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: semanticError(IKS_ERROR_WRONG_TYPE, "Na linha %d, a atribuicao de um int a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_FLOAT: semanticError(IKS_ERROR_WRONG_TYPE, "Na linha %d, a atribuicao de um float a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_BOOL: semanticError(IKS_ERROR_WRONG_TYPE, "Na linha %d, a atribuicao de um bool a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_CHAR: semanticError(IKS_ERROR_CHAR_TO_X, "Na linha %d, a atribuicao de um char a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); break;
																				case IKS_STRING: break;
																			} break;
																	}
																	//Se forem, cria um nodo de atribuicao na AST
																	$$ = createRoot(IKS_AST_ATRIBUICAO);
																	//Associa a sub-árvore da variável no nodo de atribuicao
																	appendOnChildPointer($$, $1);
																	//Associa a sub-árvore da expressão no nodo de atribuição
																	appendOnChildPointer($$, $3);

																	//Gera código para armazenar o valor que está no registrador de resultado da expressão no endereço da variável
																	concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
																	concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
																	insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_STORE, ((comp_tree_t *)$3)->resultRegister, ((comp_tree_t *)$1)->resultRegister, 0);
																}
						| var_vetor	'=' expressao				{	//Verifica se o tipo da expressao e o tipo da variável são compatíveis
																	//Se não forem, imprime erro e termina
																	switch(((comp_tree_t *)$1)->type){
																		case IKS_INT:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: break;
																				case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao float -> int
																				case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
																				case IKS_CHAR: printf("Na linha %d, a atribuicao de um char a uma variavel do tipo int eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
																				case IKS_STRING: printf("Na linha %d, a atribuicao de uma string a uma variavel do tipo int eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
																			} break;
																		case IKS_FLOAT:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_FLOAT; ((comp_tree_t *)$3)->type = IKS_FLOAT; break; //Coercao int -> float
																				case IKS_FLOAT: break;
																				case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_FLOAT; ((comp_tree_t *)$3)->type = IKS_FLOAT; break; //Coercao bool -> float
																				case IKS_CHAR: printf("Na linha %d, a atribuicao de um char a uma variavel do tipo float eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
																				case IKS_STRING: printf("Na linha %d, a atribuicao de uma string a uma variavel do tipo float eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
																			} break;
																		case IKS_BOOL:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
																				case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
																				case IKS_BOOL: break;
																				case IKS_CHAR: printf("Na linha %d, a atribuicao de um char a uma variavel do tipo bool eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
																				case IKS_STRING: printf("Na linha %d, a atribuicao de uma string a uma variavel do tipo bool eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
																			} break;
																		case IKS_CHAR:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: printf("Na linha %d, a atribuicao de um int a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE); break;
																				case IKS_FLOAT: printf("Na linha %d, a atribuicao de um float a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE); break;
																				case IKS_BOOL: printf("Na linha %d, a atribuicao de um bool a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE); break;
																				case IKS_CHAR: break;
																				case IKS_STRING: printf("Na linha %d, a atribuicao de uma string a uma variavel do tipo char eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
																			} break;
																		case IKS_STRING:
																			switch(((comp_tree_t *)$3)->type){
																				case IKS_INT: printf("Na linha %d, a atribuicao de um int a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE); break;
																				case IKS_FLOAT: printf("Na linha %d, a atribuicao de um float a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE); break;
																				case IKS_BOOL: printf("Na linha %d, a atribuicao de um bool a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE); break;
																				case IKS_CHAR: printf("Na linha %d, a atribuicao de um char a uma variavel do tipo string eh invalida.\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
																				case IKS_STRING: break;
																			} break;
																	}
																	//Se forem, cria um nodo de atribuicao na AST
																	$$ = createRoot(IKS_AST_ATRIBUICAO);
																	//Associa a sub-árvore da variável no nodo de atribuicao
																	appendOnChildPointer($$, $1);;
																	//Associa a sub-árvore da expressão no nodo de atribuição
																	appendOnChildPointer($$, $3);

																	//Gera código para armazenar o valor que está no registrador de resultado da expressão no endereço da variável
																	concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
																	concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
																	insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_STORE, ((comp_tree_t *)$3)->resultRegister, ((comp_tree_t *)$1)->resultRegister, 0);
																};

op_entrada:				TK_PR_INPUT expressao	 						{	
																	//Verifica se a expressao é uma variável simples ou um vetor
																	//Se não for nenhum dos dois, imprime erro e termina
																	if(!(((comp_tree_t *)$2)->value == IKS_AST_IDENTIFICADOR || ((comp_tree_t *)$2)->value == IKS_AST_VETOR_INDEXADO)){
																		printf("O parametro do comando input na linha %d nao eh um identificador de uma variavel simples ou de um vetor.\n", obtemLinhaAtual());
																		exit(IKS_ERROR_WRONG_PAR_INPUT);
																	}
																	//Cria nodo do tipo entrada na AST
																	$$ = createRoot(IKS_AST_INPUT);
																	//Associa sub-árvore da variável no nodo do tipo entrada
																	appendOnChildPointer($$, $2);
																};


op_retorno:				TK_PR_RETURN expressao					{	//Verifica se o tipo de retorno da função é compatível com o tipo da expressão
																	//Se não for, imprime erro e termina
																	switch(simboloFuncao->valueType){
																		case IKS_INT:
																			switch(((comp_tree_t *)$2)->type){
																				case IKS_INT: break;
																				case IKS_FLOAT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao float -> int
																				case IKS_BOOL: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao bool -> int
																				case IKS_CHAR: printf("O valor de retorno da funcao '%s' deve ser um int, porem o valor de retorno na linha %d eh um char.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_STRING: printf("O valor de retorno da funcao '%s' deve ser um int, porem o valor de retorno na linha %d eh um string.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																			} break;
																		case IKS_FLOAT:
																			switch(((comp_tree_t *)$2)->type){
																				case IKS_INT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_INT_FLOAT; ((comp_tree_t *)$2)->type = IKS_FLOAT; break; //Coercao int -> float
																				case IKS_FLOAT: break;
																				case IKS_BOOL: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_BOOL_FLOAT; ((comp_tree_t *)$2)->type = IKS_FLOAT; break; //Coercao bool -> float
																				case IKS_CHAR: printf("O valor de retorno da funcao '%s' deve ser um float, porem o valor de retorno na linha %d eh um char.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_STRING: printf("O valor de retorno da funcao '%s' deve ser um float, porem o valor de retorno na linha %d eh um string.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																			} break;
																		case IKS_BOOL:
																			switch(((comp_tree_t *)$2)->type){
																				case IKS_INT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$2)->type = IKS_BOOL; break; //Coercao int -> bool
																				case IKS_FLOAT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$2)->type = IKS_BOOL; break; //Coercao float -> bool
																				case IKS_BOOL: break;
																				case IKS_CHAR: printf("O valor de retorno da funcao '%s' deve ser um bool, porem o valor de retorno na linha %d eh um char.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_STRING: printf("O valor de retorno da funcao '%s' deve ser um bool, porem o valor de retorno na linha %d eh um string.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																			} break;
																		case IKS_CHAR:
																			switch(((comp_tree_t *)$2)->type){
																				case IKS_INT: printf("O valor de retorno da funcao '%s' deve ser um char, porem o valor de retorno na linha %d eh um int.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_FLOAT: printf("O valor de retorno da funcao '%s' deve ser um char, porem o valor de retorno na linha %d eh um float.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_BOOL: printf("O valor de retorno da funcao '%s' deve ser um char, porem o valor de retorno na linha %d eh um bool.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_CHAR: break;
																				case IKS_STRING: printf("O valor de retorno da funcao '%s' deve ser um char, porem o valor de retorno na linha %d eh um string.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																			} break;
																		case IKS_STRING:
																			switch(((comp_tree_t *)$2)->type){
																				case IKS_INT: printf("O valor de retorno da funcao '%s' deve ser um string, porem o valor de retorno na linha %d eh um int.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_FLOAT: printf("O valor de retorno da funcao '%s' deve ser um string, porem o valor de retorno na linha %d eh um float.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_BOOL: printf("O valor de retorno da funcao '%s' deve ser um string, porem o valor de retorno na linha %d eh um bool.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_CHAR: printf("O valor de retorno da funcao '%s' deve ser um string, porem o valor de retorno na linha %d eh um char.\n", simboloFuncao->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_RETURN); break;
																				case IKS_STRING: break;
																			} break;
																	}
																	//Se for, cria nodo de retorno na AST
																	$$ = createRoot(IKS_AST_RETURN);
																	//Associa sub-árvore da expressão no nodo de retorno
																	appendOnChildPointer($$, $2);
																};

op_saida:				TK_PR_OUTPUT lista_elementos_saida		{ $$ = createRoot(IKS_AST_OUTPUT); appendOnChildPointer($$, $2);}
						| TK_PR_OUTPUT /* VAZIO */				{ $$ = createRoot(IKS_AST_OUTPUT); };
lista_elementos_saida:	expressao								{	//Verifica se a expressão é do tipo string ou é uma expressão aritmética
																	//Se não for, imprime erro e termina
																	switch(((comp_tree_t *)$1)->type){
																		case IKS_INT: break;
																		case IKS_FLOAT: break;
																		case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
																		case IKS_CHAR: printf("O parametro do comando output na linha %d eh um char, porem ele deve ser uma string ou uma expressao aritmetica.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_OUTPUT); break;
																		case IKS_STRING: break;
																	}
																	$$ = $1;
																}
						| expressao ',' lista_elementos_saida	{	//Verifica se a expressão é do tipo string ou é uma expressão aritmética
																	//Se não for, imprime erro e termina
																	switch(((comp_tree_t *)$1)->type){
																		case IKS_INT: break;
																		case IKS_FLOAT: break;
																		case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
																		case IKS_CHAR: printf("O parametro do comando output na linha %d eh um char, porem ele deve ser uma string ou uma expressao aritmetica.\n", obtemLinhaAtual()); exit(IKS_ERROR_WRONG_PAR_OUTPUT); break;
																		case IKS_STRING: break;
																	}
																	$$ = $1;
																	appendOnChildPointer($$, $3);
																};

			
			
			
			
			
			
			
			
	
			

/* Uma expressão tem como folha uma variável, um literal ou uma chamada de função. */
expressao:						var									{ $$ = $1; }
											| literal						{ $$ = $1; }
											| chamada_funcao		{ $$ = $1; }
											| '(' expressao ')'	{ $$ = $2; }
							
											| '-' expressao	
											{
												//Verifica se a expressão é compatível com o tipo float ou int
												switch(((comp_tree_t *)$2)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se for, cria um nó de inversão aritmética na AST
												$$ = createRoot(IKS_AST_ARIM_INVERSAO);
												//Associa o tipo no nó de inversão aritmética
												if(((comp_tree_t *)$2)->type == IKS_FLOAT) ((comp_tree_t *)$$)->type = IKS_FLOAT;
												else ((comp_tree_t *)$$)->type = IKS_INT;
												//Associa a sub-árvore da expressão no nó de inversão aritmética
												appendOnChildPointer($$, $2);

												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Gera código para inverter o valor do registrador de resultado da expressão e armazenar no novo registrador de resultado
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$2)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_RSUBI, ((comp_tree_t *)$2)->resultRegister, 0, ((comp_tree_t *)$$)->resultRegister);
											}
											
											| expressao '+' expressao
											{
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Cria um nó de soma aritmética na AST
												$$ = createRoot(IKS_AST_ARIM_SOMA);
												//Associa o tipo no nó de soma aritmética
												if(((comp_tree_t *)$1)->type == IKS_FLOAT || ((comp_tree_t *)$3)->type == IKS_FLOAT)
													((comp_tree_t *)$$)->type = IKS_FLOAT;
												else ((comp_tree_t *)$$)->type = IKS_INT;
												//Associa as sub-árvores das expressões no nó de soma aritmética
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);

												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
								
												//Gera código para somar o valor do registrador de resultado das expressões e armazenar no novo registrador de resultado
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_ADD, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, ((comp_tree_t *)$$)->resultRegister);
											}
											
											| expressao '-' expressao
											{
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Cria um nó de subtração aritmética na AST
												$$ = createRoot(IKS_AST_ARIM_SUBTRACAO);
												//Associa o tipo no nó de subtração aritmética
												if(((comp_tree_t *)$1)->type == IKS_FLOAT || ((comp_tree_t *)$3)->type == IKS_FLOAT)
													((comp_tree_t *)$$)->type = IKS_FLOAT;
												else ((comp_tree_t *)$$)->type = IKS_INT;
												//Associa as sub-árvores das expressões no nó de subtração aritmética
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);

												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
								
												//Gera código para subtrair o valor do registrador de resultado das expressões e armazenar no novo registrador de resultado
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_SUB, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, ((comp_tree_t *)$$)->resultRegister);
											}
											
											| expressao '/' expressao
											{
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Cria um nó de divisão aritmética na AST
												$$ = createRoot(IKS_AST_ARIM_DIVISAO);
												//Associa o tipo no nó de divisão aritmética
												if(((comp_tree_t *)$1)->type == IKS_FLOAT || ((comp_tree_t *)$3)->type == IKS_FLOAT)
													((comp_tree_t *)$$)->type = IKS_FLOAT;
												else ((comp_tree_t *)$$)->type = IKS_INT;
												//Associa as sub-árvores das expressões no nó de divisão aritmética
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);

												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
								
												//Gera código para dividir o valor do registrador de resultado das expressões e armazenar no novo registrador de resultado
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_DIV, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, ((comp_tree_t *)$$)->resultRegister);
											}
											
											| expressao '*' expressao
											{	
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												
												//Cria um nó de multiplicação aritmética na AST
												$$ = createRoot(IKS_AST_ARIM_MULTIPLICACAO);
												//Associa o tipo no nó de multiplicação aritmética
												if(((comp_tree_t *)$1)->type == IKS_FLOAT || ((comp_tree_t *)$3)->type == IKS_FLOAT)
													((comp_tree_t *)$$)->type = IKS_FLOAT;
												else ((comp_tree_t *)$$)->type = IKS_INT;
												//Associa as sub-árvores das expressões no nó de multiplicação aritmética
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);

												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
								
												//Gera código para multiplicar o valor do registrador de resultado das expressões e armazenar no novo registrador de resultado
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_MULT, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, ((comp_tree_t *)$$)->resultRegister);
											}
												
											| '!' expressao
											{
												//Verifica se a expressão é compatível com o tipo bool
												switch(((comp_tree_t *)$2)->type){
													case IKS_INT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$2)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$2)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para um bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para um bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se for, cria um nó de negação lógica na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_NEGACAO);
												//Associa o tipo no nó de negação lógica
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa a sub-árvore da expressão no nó de negação lógica
												appendOnChildPointer($$, $2);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria registrador auxiliar que contém o valor 1
												int regValue1 = getRegister();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, regValue1, 0);
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
															
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena o código da expressão
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$2)->code));
												
												//Avalia a expressão
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$2)->resultRegister, regValue1, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Expressão é verdadeira
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Expressão é falsa
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}

											| expressao '<' expressao
											{
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de comparação lógica L na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_L);
												//Associa o tipo no nó de comparação lógica L
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de comparação lógica L
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena códigos das expressões
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia o comando
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPLT, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Resultado é verdadeiro
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Resultado é falso
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao '>' expressao
											{	
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de comparação lógica G na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_G);
												//Associa o tipo no nó de comparação lógica G
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de comparação lógica G
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena códigos das expressões
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia o comando
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGT, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Resultado é verdadeiro
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Resultado é falso
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao TK_OC_LE expressao
											{	
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de comparação lógica LE na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_LE);
												//Associa o tipo no nó de comparação lógica LE
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de comparação lógica LE
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);

												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena códigos das expressões
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia o comando
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPLE, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Resultado é verdadeiro
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Resultado é falso
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao TK_OC_GE expressao
											{
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de comparação lógica GE na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_GE);
												//Associa o tipo no nó de comparação lógica GE
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de comparação lógica GE
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena códigos das expressões
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia o comando
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Resultado é verdadeiro
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Resultado é falso
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao TK_OC_EQ expressao
											{	
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de comparação lógica EQ na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_IGUAL);
												//Associa o tipo no nó de comparação lógica EQ
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de comparação lógica EQ
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena códigos das expressões
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia o comando
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPEQ, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Resultado é verdadeiro
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Resultado é falso
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao TK_OC_NE expressao
											{	
												//Verifica se as expressões são compatíveis com int ou float
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: break;
													case IKS_FLOAT: break;
													case IKS_BOOL: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$3)->type = IKS_INT; break; //Coercao bool -> int
													case IKS_CHAR: printf("Nao eh possivel converter um char para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para int ou float na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de comparação lógica DIF na AST
												$$ = createRoot(IKS_AST_LOGICO_COMP_DIF);
												//Associa o tipo no nó de comparação lógica DIF
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de comparação lógica DIF
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar o resultado da avaliação do comando
												int regEvaluation = getRegister();
								
												//Concatena códigos das expressões
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia o comando
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPNE, ((comp_tree_t *)$1)->resultRegister, ((comp_tree_t *)$3)->resultRegister, regEvaluation);
												int labelTrue = getLabel();
												int labelFalse = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelTrue, labelFalse);

												//Resultado é verdadeiro
												insertOperation(&(((comp_tree_t *)$$)->code), labelTrue, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Resultado é falso
												insertOperation(&(((comp_tree_t *)$$)->code), labelFalse, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao TK_OC_OR expressao
											{
												//Verifica se as expressões são compatíveis com bool
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de operador lógico OU
												$$ = createRoot(IKS_AST_LOGICO_OU);
												//Associa o tipo no nó de operador lógico OU
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de operador lógico OU
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria registrador auxiliar que contém o valor 1
												int regValue1 = getRegister();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, regValue1, 0);
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar a avaliação de cada expressão
												int regEvaluation = getRegister();
												
												//Concatena o código da expressão 1
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												
												//Avalia expressão 1
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$1)->resultRegister, regValue1, regEvaluation);
												int labelExp1True = getLabel();
												int labelExp1False = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelExp1True, labelExp1False);
												
												//Expressão 1 é verdadeira (o resultado é verdadeiro)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp1True, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Expressão 1 é falsa (avalia a próxima expressão)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp1False, ILOC_NOP, 0, 0, 0);
												
												//Concatena o código da expressão 2
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia expressão 2
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$3)->resultRegister, regValue1, regEvaluation);
												int labelExp2True = getLabel();
												int labelExp2False = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelExp2True, labelExp2False);
												
												//Expressão 2 é verdadeira (o resultado é verdadeiro)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp2True, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Expressão 2 é falsa (o resultado é falso)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp2False, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação da expressão 2
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											}
												
											| expressao TK_OC_AND expressao
											{
												//Verifica se as expressões são compatíveis com bool
												switch(((comp_tree_t *)$1)->type){
													case IKS_INT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se forem, cria um nó de operador lógico E
												$$ = createRoot(IKS_AST_LOGICO_E);
												//Associa o tipo no nó de operador lógico E
												((comp_tree_t *)$$)->type = IKS_BOOL;
												//Associa as sub-árvores das expressões no nó de operador lógico E
												appendOnChildPointer($$, $1);
												appendOnChildPointer($$, $3);
												
												//Obtém registrador de resultado
												((comp_tree_t *)$$)->resultRegister = getRegister();
												
												//Cria registrador auxiliar que contém o valor 1
												int regValue1 = getRegister();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, regValue1, 0);
												
												//Cria o label de saída da avaliação
												int nextLabel = getLabel();
												
												//Cria registrador para armazenar a avaliação de cada expressão
												int regEvaluation = getRegister();
												
												//Concatena o código da expressão 1
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$1)->code));
												
												//Avalia expressão 1
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$1)->resultRegister, regValue1, regEvaluation);
												int labelExp1True = getLabel();
												int labelExp1False = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelExp1True, labelExp1False);
												
												//Expressão 1 é falsa (o resultado é falso)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp1False, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Expressão 1 é verdadeira (avalia a próxima expressão)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp1True, ILOC_NOP, 0, 0, 0);
												
												//Concatena o código da expressão 2
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Avalia expressão 2
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$3)->resultRegister, regValue1, regEvaluation);
												int labelExp2True = getLabel();
												int labelExp2False = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, regEvaluation, labelExp2True, labelExp2False);
												
												//Expressão 2 é verdadeira (o resultado é verdadeiro)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp2True, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, nextLabel, 0, 0);
												
												//Expressão 2 é falsa (o resultado é falso)
												insertOperation(&(((comp_tree_t *)$$)->code), labelExp2False, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
												
												//Fim da avaliação da expressão 2
												insertOperation(&(((comp_tree_t *)$$)->code), nextLabel, ILOC_NOP, 0, 0, 0);
											};

		
		
		
		
		
		
		

/* Chamada de uma função */
chamada_funcao:		nome_fun '(' lista_de_argumentos ')'	{
									//Verifica se o identificador já foi declarado (no escopo global)
									comp_dict_item_t *item = searchKey(*tabelaDeSimbolosEscopoGlobal, $1);
									//Se não foi, imprime o erro e termina
									if(item == NULL){
										printf("O identificador '%s' utilizado na linha %d nao foi declarado.\n", $1, obtemLinhaAtual());
										exit(IKS_ERROR_UNDECLARED);
									}
									//Se foi, verifica se ele foi declarado como uma função
									//Se não foi, imprime o erro e termina
									switch(item->nodeType){
										case IKS_VARIABLE_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como uma variavel simples e nao como uma funcao.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_VARIABLE); break;
										case IKS_VECTOR_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como um vetor e nao como uma funcao.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_VECTOR); break;
										case IKS_FUNCTION_ITEM: break;
									}
									free($1);
									//Verifica se ainda faltam argumentos
									//Se ainda faltam, imprime erro e termina
									if(listaDeParametrosSendoLida != NULL){
										printf("A chamada da funcao '%s' na linha %d possui menos argumentos que o necessario.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual());
										exit(IKS_ERROR_MISSING_ARGS);
									}
									//Cria um nó de chamada de função na AST
									$$ = createRoot(IKS_AST_CHAMADA_DE_FUNCAO);
									//Associa o tipo do nó de chamada de função (tipo de retorno da função)
									((comp_tree_t *)$$)->type = item->valueType;
									//Cria um nó de identificador como filho do nó de chamada de função
									appendOnChildPointer($$, createRoot(IKS_AST_IDENTIFICADOR));
									//Associa um ponteiro para uma entrada na tabela de símbolos no nó de identificador
									((comp_tree_t *)$$)->child->dictPointer = item;
									//Associa a sub-árvore que contém os argumentos no nó de chamada de função
									appendOnChildPointer($$, $3);
								}
			| nome_fun '(' /* VAZIO */ ')'		{	//Verifica se o identificador já foi declarado (no escopo global)
									comp_dict_item_t *item = searchKey(*tabelaDeSimbolosEscopoGlobal, $1);
									//Se não foi, imprime o erro e termina
									if(item == NULL){
										printf("O identificador '%s' utilizado na linha %d nao foi declarado.\n", $1, obtemLinhaAtual());
										exit(IKS_ERROR_UNDECLARED);
									}
									//Se foi, verifica se ele foi declarado como uma função
									//Se não foi, imprime o erro e termina
									switch(item->nodeType){
										case IKS_VARIABLE_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como uma variavel simples e nao como uma funcao.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_VARIABLE); break;
										case IKS_VECTOR_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como um vetor e nao como uma funcao.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_VECTOR); break;
										case IKS_FUNCTION_ITEM: break;
									}
									//Se foi, verifica se a função não precisa de parâmetros
									//Se precisa, imprime erro e termina
									if(countListNodes(item->parametersList) > 0){
										printf("Faltam argumentos na chamada da funcao '%s' na linha %d.\n", $1, obtemLinhaAtual());
										exit(IKS_ERROR_MISSING_ARGS);
									}
									//Se não precisa, a chamada é válida
									free($1);
									//Cria um nó de chamada de função na AST
									$$ = createRoot(IKS_AST_CHAMADA_DE_FUNCAO);
									//Associa o tipo do nó de chamada de função (tipo de retorno da função)
									((comp_tree_t *)$$)->type = item->valueType;
									//Cria um nó de identificador como filho do nó de chamada de função
									appendOnChildPointer($$, createRoot(IKS_AST_IDENTIFICADOR));
									//Associa um ponteiro para uma entrada na tabela de símbolos no nó de identificador
									((comp_tree_t *)$$)->child->dictPointer = item;
								};
																		
nome_fun:		TK_IDENTIFICADOR 			{
									simboloDaFuncaoSendoChamada = searchKey(*tabelaDeSimbolosEscopoGlobal, $1);
									if(simboloDaFuncaoSendoChamada == NULL){
										printf("O identificador '%s' utilizado na linha %d nao foi declarado.\n", $1, obtemLinhaAtual());
										exit(IKS_ERROR_UNDECLARED);
									}
									listaDeParametrosSendoLida = simboloDaFuncaoSendoChamada->parametersList;
									$$ = $1;
								}

lista_de_argumentos:	expressao ',' lista_de_argumentos	{	//Verifica se o tipo de expressão é compatível com o parâmetro da posição correspondente
									if(listaDeParametrosSendoLida == NULL){
										printf("A chamada da funcao '%s' na linha %d possui mais argumentos que o necessario.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual());
										exit(IKS_ERROR_EXCESS_ARGS);
									}
									int tipoDoArgumento = *((int *)listaDeParametrosSendoLida->data);
									switch(tipoDoArgumento){
										case IKS_INT:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: break;
												case IKS_FLOAT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao float -> int
												case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
												case IKS_CHAR: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo int.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo int.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_FLOAT:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_INT_FLOAT; ((comp_tree_t *)$1)->type = IKS_FLOAT; break; //Coercao int -> float
												case IKS_FLOAT: break;
												case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_FLOAT; ((comp_tree_t *)$1)->type = IKS_FLOAT; break; //Coercao bool -> float
												case IKS_CHAR: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo float.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo float.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_BOOL:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao int -> bool
												case IKS_FLOAT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao float -> bool
												case IKS_BOOL: break;
												case IKS_CHAR: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo bool.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo bool.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_CHAR:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo int, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_FLOAT: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo float, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_BOOL: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo bool, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_CHAR: break;
												case IKS_STRING: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_STRING:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo int, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_FLOAT: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo float, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_BOOL: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo bool, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_CHAR: printf("Um argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: break;
											} break;
									}
									listaDeParametrosSendoLida = listaDeParametrosSendoLida->next;
									$$ = $1;
									appendOnChildPointer($$, $3);
								}

			| expressao				{	//Verifica se o tipo da expressão é compatível com o último parâmetro da função
									if(listaDeParametrosSendoLida == NULL){
										printf("A chamada da funcao '%s' na linha %d possui mais argumentos que o necessario.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual());
										exit(IKS_ERROR_EXCESS_ARGS);
									}
									int tipoDoArgumento = *((int *)listaDeParametrosSendoLida->data);
									switch(tipoDoArgumento){
										case IKS_INT:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: break;
												case IKS_FLOAT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao float -> int
												case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$1)->type = IKS_INT; break; //Coercao bool -> int
												case IKS_CHAR: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo int.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo int.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_FLOAT:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_INT_FLOAT; ((comp_tree_t *)$1)->type = IKS_FLOAT; break; //Coercao int -> float
												case IKS_FLOAT: break;
												case IKS_BOOL: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_BOOL_FLOAT; ((comp_tree_t *)$1)->type = IKS_FLOAT; break; //Coercao bool -> float
												case IKS_CHAR: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo float.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo float.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_BOOL:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao int -> bool
												case IKS_FLOAT: ((comp_tree_t *)$1)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$1)->type = IKS_BOOL; break; //Coercao float -> bool
												case IKS_BOOL: break;
												case IKS_CHAR: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo bool.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo bool.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_CHAR:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo int, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_FLOAT: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo float, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_BOOL: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo bool, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_CHAR: break;
												case IKS_STRING: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo string, porem, devia ser do tipo char.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
											} break;
										case IKS_STRING:
											switch(((comp_tree_t *)$1)->type){
												case IKS_INT: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo int, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_FLOAT: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo float, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_BOOL: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo bool, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_CHAR: printf("O ultimo argumento da chamada da funcao '%s' na linha %d eh do tipo char, porem, devia ser do tipo string.\n", simboloDaFuncaoSendoChamada->key, obtemLinhaAtual()); exit(IKS_ERROR_WRONG_TYPE_ARGS); break;
												case IKS_STRING: break;
											} break;
									}
									listaDeParametrosSendoLida = listaDeParametrosSendoLida->next;
									$$ = $1;
								};



			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
/* Controle de fluxo */

controle_fluxo:	 if_then 				{ $$ = $1; }
								| if_then_else 	{ $$ = $1; }
								| while_do 			{ $$ = $1; }
								| do_while 			{ $$ = $1; };
									
flow_control_command:	comando	{ $$ = $1; }
											| ';'		{ $$ = NULL; };
											

if_then:				TK_PR_IF '(' expressao ')' TK_PR_THEN flow_control_command	%prec "then"
								{	
									//Verifica se a expressão é compatível com o tipo bool
									switch(((comp_tree_t *)$3)->type){
										case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
										case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
										case IKS_BOOL: break;
										case IKS_CHAR: printf("Nao eh possivel converter um char para um bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
										case IKS_STRING: printf("Nao eh possivel converter um string para um bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
									}
									//Se for, cria nó de if-else na AST
									$$ = createRoot(IKS_AST_IF_ELSE);
									//Associa a sub-árvore da expressão booleana no nodo if-else
									appendOnChildPointer($$, $3);
									//Associa a sub-árvore do comando "then" no nodo if-else
									appendOnChildPointer($$, $6);
									//Associa NULL como o terceiro filho do nodo if-else para indicar que não tem comando "else"
									appendOnChildPointer($$, NULL);
									
									//Concatena o código da expressão
									concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
									
									//Verifica se o resultado é true ou false
									int tmpRegister = getRegister();
									int tmpRegister2 = getRegister();
									insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, tmpRegister2, 0);
									insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$3)->resultRegister, tmpRegister2, tmpRegister);
									
									int tmpLabel1 = getLabel();
									int tmpLabel2 = getLabel();
									insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, tmpRegister, tmpLabel1, tmpLabel2);
									
									insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel1, ILOC_NOP, 0, 0, 0);
									//Concatena código do comando
									if($6 != NULL) concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$6)->code));
									
									insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel2, ILOC_NOP, 0, 0, 0);
								};
																					
if_then_else:	TK_PR_IF '(' expressao ')' TK_PR_THEN	flow_control_command TK_PR_ELSE flow_control_command	{	//Verifica se a expressão é compatível com o tipo bool
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se for, cria nó de if-else na AST
												$$ = createRoot(IKS_AST_IF_ELSE);
												//Associa a sub-árvore da expressão booleana no nodo if-else
												appendOnChildPointer($$, $3);
												//Associa a sub-árvore do comando "then" no nodo if-else
												appendOnChildPointer($$, $6);
												//Associa a sub-árvore do comando "else" no nodo if-else
												appendOnChildPointer($$, $8);
												
												//Concatena o código da expressão
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Verifica se o resultado é true ou false
												int tmpRegister = getRegister();
												int tmpRegister2 = getRegister();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, tmpRegister2, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$3)->resultRegister, tmpRegister2, tmpRegister);
												
												int tmpLabel1 = getLabel();
												int tmpLabel2 = getLabel();
												int tmpLabel3 = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, tmpRegister, tmpLabel1, tmpLabel2);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel1, ILOC_NOP, 0, 0, 0);
												//Concatena código do comando 1
												if($6 != NULL) concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$6)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, tmpLabel3, 0, 0);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel2, ILOC_NOP, 0, 0, 0);
												//Concatena código do comando 2
												if($8 != NULL) concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$8)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, tmpLabel3, 0, 0);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel3, ILOC_NOP, 0, 0, 0);
											};
																					
while_do:	TK_PR_WHILE '(' expressao ')'	TK_PR_DO flow_control_command			{	//Verifica se a expressão é compatível com o tipo bool
												switch(((comp_tree_t *)$3)->type){
													case IKS_INT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$3)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$3)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se for, cria nó de while-do na AST
												$$ = createRoot(IKS_AST_WHILE_DO);
												//Associa a sub-árvore da expressão booleana no nodo while-do
												appendOnChildPointer($$, $3);
												//Associa a sub-árvore do comando no nodo while-do
												appendOnChildPointer($$, $6);
												
												//Concatena o código da expressão
												int tmpLabel1 = getLabel();
												int tmpLabel2 = getLabel();
												int tmpLabel3 = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel1, ILOC_NOP, 0, 0, 0);
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$3)->code));
												
												//Verifica se o resultado é true ou false
												int tmpRegister = getRegister();
												int tmpRegister2 = getRegister();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, tmpRegister2, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$3)->resultRegister, tmpRegister2, tmpRegister);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, tmpRegister, tmpLabel2, tmpLabel3);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel2, ILOC_NOP, 0, 0, 0);
												//Concatena código do comando
												if($6 != NULL) concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$6)->code));
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, tmpLabel1, 0, 0);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel3, ILOC_NOP, 0, 0, 0);
											};
																					
do_while:	TK_PR_DO flow_control_command TK_PR_WHILE '(' expressao ')'			{	//Verifica se a expressão é compatível com o tipo bool
												switch(((comp_tree_t *)$5)->type){
													case IKS_INT: ((comp_tree_t *)$5)->tipoCoercao = IKS_COERCAO_INT_BOOL; ((comp_tree_t *)$5)->type = IKS_BOOL; break; //Coercao int -> bool
													case IKS_FLOAT: ((comp_tree_t *)$5)->tipoCoercao = IKS_COERCAO_FLOAT_BOOL; ((comp_tree_t *)$5)->type = IKS_BOOL; break; //Coercao float -> bool
													case IKS_BOOL: break;
													case IKS_CHAR: printf("Nao eh possivel converter um char para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
													case IKS_STRING: printf("Nao eh possivel converter uma string para bool na linha %d\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
												}
												//Se for, cria nó de do-while na AST
												$$ = createRoot(IKS_AST_DO_WHILE);
												//Associa a sub-árvore do comando no nodo do-while
												appendOnChildPointer($$, $2);
												//Associa a sub-árvore da expressão booleana no nodo do-while
												appendOnChildPointer($$, $5);
												
												//Concatena o código do comando
												int tmpLabel1 = getLabel();
												int tmpLabel2 = getLabel();
												int tmpLabel3 = getLabel();
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel1, ILOC_NOP, 0, 0, 0);
												if($2 != NULL) concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$2)->code));
												
												//Concatena o código da expressão
												concatCode(&(((comp_tree_t *)$$)->code), &(((comp_tree_t *)$5)->code));
												
												//Verifica se o resultado é true ou false
												int tmpRegister = getRegister();
												int tmpRegister2 = getRegister();
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, tmpRegister2, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CMPGE, ((comp_tree_t *)$5)->resultRegister, tmpRegister2, tmpRegister);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_CBR, tmpRegister, tmpLabel2, tmpLabel3);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel2, ILOC_NOP, 0, 0, 0);
												insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_JUMPI, tmpLabel1, 0, 0);
												
												insertOperation(&(((comp_tree_t *)$$)->code), tmpLabel3, ILOC_NOP, 0, 0, 0);
											};

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
/* Literais */
literal:	TK_LIT_FALSE	
					{	
						//Cria o nó de literal na árvore
						$$ = createRoot(IKS_AST_LITERAL);
						//Adiciona o literal na tabela de símbolos e o associa ao nó de literal
						((comp_tree_t *)$$)->dictPointer = insertKey(tabelaDeSimbolosAtual, $1, IKS_BOOL, obtemLinhaAtual());
						((comp_tree_t *)$$)->dictPointer->nodeType = IKS_LITERAL_ITEM;
						free($1);
						//Associa o seu tipo (tipo do literal)
						((comp_tree_t *)$$)->type = IKS_BOOL;

						//Obtém registrador de resultado
						((comp_tree_t *)$$)->resultRegister = getRegister();
								
						//Gera código para armazenar o valor do literal no registrador de resultado
						insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 0, ((comp_tree_t *)$$)->resultRegister, 0);
					}

					| TK_LIT_TRUE
					{	
						//Cria o nó de literal na árvore
						$$ = createRoot(IKS_AST_LITERAL);
						//Adiciona o literal na tabela de símbolos e o associa ao nó de literal
						((comp_tree_t *)$$)->dictPointer = insertKey(tabelaDeSimbolosAtual, $1, IKS_BOOL, obtemLinhaAtual());
						((comp_tree_t *)$$)->dictPointer->nodeType = IKS_LITERAL_ITEM;
						free($1);
						//Associa o seu tipo (tipo do literal)
						((comp_tree_t *)$$)->type = IKS_BOOL;

						//Obtém registrador de resultado
						((comp_tree_t *)$$)->resultRegister = getRegister();
								
						//Gera código para armazenar o valor do literal no registrador de resultado
						insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, 1, ((comp_tree_t *)$$)->resultRegister, 0);
					}

					| TK_LIT_INT
					{	
						//Cria o nó de literal na árvore
						$$ = createRoot(IKS_AST_LITERAL);
						//Adiciona o literal na tabela de símbolos e o associa ao nó de literal
						((comp_tree_t *)$$)->dictPointer = insertKey(tabelaDeSimbolosAtual, $1, IKS_INT, obtemLinhaAtual());
						((comp_tree_t *)$$)->dictPointer->nodeType = IKS_LITERAL_ITEM;
						free($1);
						//Associa o seu tipo (tipo do literal)
						((comp_tree_t *)$$)->type = IKS_INT;

						//Obtém registrador de resultado
						((comp_tree_t *)$$)->resultRegister = getRegister();
								
						//Gera código para armazenar o valor do literal no registrador de resultado
						insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, ((comp_tree_t *)$$)->dictPointer->intValue, ((comp_tree_t *)$$)->resultRegister, 0);
					}

					| TK_LIT_FLOAT	
					{	
						//Cria o nó de literal na árvore
						$$ = createRoot(IKS_AST_LITERAL);
						//Adiciona o literal na tabela de símbolos e o associa ao nó de literal
						((comp_tree_t *)$$)->dictPointer = insertKey(tabelaDeSimbolosAtual, $1, IKS_FLOAT, obtemLinhaAtual());
						((comp_tree_t *)$$)->dictPointer->nodeType = IKS_LITERAL_ITEM;
						free($1);
						//Associa o seu tipo (tipo do literal)
						((comp_tree_t *)$$)->type = IKS_FLOAT;
						
						//Obtém registrador de resultado
						((comp_tree_t *)$$)->resultRegister = getRegister();
								
						//Gera código para armazenar o valor do literal no registrador de resultado
						insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, (int)(((comp_tree_t *)$$)->dictPointer->floatValue), ((comp_tree_t *)$$)->resultRegister, 0);
					}

					| TK_LIT_CHAR	
					{	
						//Cria o nó de literal na árvore
						$$ = createRoot(IKS_AST_LITERAL);
						//Adiciona o literal na tabela de símbolos e o associa ao nó de literal
						((comp_tree_t *)$$)->dictPointer = insertKey(tabelaDeSimbolosAtual, $1, IKS_CHAR, obtemLinhaAtual());
						((comp_tree_t *)$$)->dictPointer->nodeType = IKS_LITERAL_ITEM;
						free($1);
						//Associa o seu tipo (tipo do literal)
						((comp_tree_t *)$$)->type = IKS_CHAR;
					}

					| TK_LIT_STRING	
					{	
						//Cria o nó de literal na árvore
						$$ = createRoot(IKS_AST_LITERAL);
						//Adiciona o literal na tabela de símbolos e o associa ao nó de literal
						((comp_tree_t *)$$)->dictPointer = insertKey(tabelaDeSimbolosAtual, $1, IKS_STRING, obtemLinhaAtual());
						((comp_tree_t *)$$)->dictPointer->nodeType = IKS_LITERAL_ITEM;
						free($1);
						//Associa o seu tipo (tipo do literal)
						((comp_tree_t *)$$)->type = IKS_STRING;
					};

		
		
		
		
		
		
/* Uso de variáveis e vetores */

var:	var_simples	
							{
								$$ = $1;
								//Carrega o conteúda da variável em um registrador
								int resultRegister = getRegister();
								insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOAD, ((comp_tree_t *)$$)->resultRegister, resultRegister, 0);
								((comp_tree_t *)$$)->resultRegister = resultRegister;
							}

			| var_vetor
							{ 
								$$ = $1;
								//Carrega o conteúda da variável em um registrador
								int resultRegister = getRegister();
								insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOAD, ((comp_tree_t *)$$)->resultRegister, resultRegister, 0);
								((comp_tree_t *)$$)->resultRegister = resultRegister;
							};




var_simples:	TK_IDENTIFICADOR
							{	
								//Verifica se o identificador já foi declarado no escopo local
								int varGlobal = 0;
								comp_dict_item_t *item = searchKey(*tabelaDeSimbolosAtual, $1);
								if(item == NULL){
									//Se não foi, verifica se ele já foi declarado no escopo global
									item = searchKey(*tabelaDeSimbolosEscopoGlobal, $1);
									//Se ainda não foi, imprime o erro e termina
									if(item == NULL){
										printf("O identificador '%s' utilizado na linha %d nao foi declarado.\n", $1, obtemLinhaAtual());
										exit(IKS_ERROR_UNDECLARED);
									}
									varGlobal = 1;
								}

								//Se foi declarado em alguns dos escopos, verifica se ele foi declarado como variável simples
								//Se não foi declarado como variável simples, imprime o erro e termina
								switch(item->nodeType){
									case IKS_VARIABLE_ITEM: break;
									case IKS_VECTOR_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como um vetor e nao como uma variavel simples.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_VECTOR); break;
									case IKS_FUNCTION_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como uma funcao e nao como uma variavel simples.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_FUNCTION); break;
								}
								free($1);

								//Cria um nodo de identificador na AST
								$$ = createRoot(IKS_AST_IDENTIFICADOR);
								//Associa o tipo do nó no nodo do identificador (o tipo da variável)
								((comp_tree_t *)$$)->type = item->valueType;
								//Associa um ponteiro para a entrada na tabela de símbolos no nodo do identificador
								((comp_tree_t *)$$)->dictPointer = item;

								//Obtém registrador de resultado
								((comp_tree_t *)$$)->resultRegister = getRegister();
								
								//Gera código para ler variável da memória e armazenar o seu valor no registrador de resultado
								insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_LOADI, item->address, ((comp_tree_t *)$$)->resultRegister, 0);
								if(varGlobal == 1) insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_ADDI, ((comp_tree_t *)$$)->resultRegister, ILOC_BSS, ((comp_tree_t *)$$)->resultRegister);
								else insertOperation(&(((comp_tree_t *)$$)->code), ILOC_NO_LABEL, ILOC_ADDI, ((comp_tree_t *)$$)->resultRegister, ILOC_RARP, ((comp_tree_t *)$$)->resultRegister);
							};
													
var_vetor:		TK_IDENTIFICADOR
							{	
								//Verifica se o identificador já foi declarado no escopo local
								comp_dict_item_t *item = searchKey(*tabelaDeSimbolosAtual, $1);
								if(item == NULL){
									//Se não foi, verifica se ele já foi declarado no escopo global
									item = searchKey(*tabelaDeSimbolosEscopoGlobal, $1);
									//Se ainda não foi, imprime o erro e termina
									if(item == NULL){
										printf("O identificador '%s' utilizado na linha %d nao foi declarado.\n", $1, obtemLinhaAtual());
										exit(IKS_ERROR_UNDECLARED);
									}
								}
								//Se foi declarado em alguns dos escopos, verifica se ele foi declarado como um vetor
								//Se não foi declarado como vetor, imprime o erro e termina
								switch(item->nodeType){
									case IKS_VARIABLE_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como uma variavel simples e nao como vetor.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_VARIABLE); break;
									case IKS_VECTOR_ITEM: break;
									case IKS_FUNCTION_ITEM: printf("O identificador '%s' utilizado na linha %d foi declarado como uma funcao e nao como um vetor.\n", $1, obtemLinhaAtual()); exit(IKS_ERROR_FUNCTION); break;
								}
								free($1);
								
								VectorReadingInfo *vectorBeingRead = malloc(sizeof(VectorReadingInfo));
								push(&vectorStack, vectorBeingRead);
								vectorBeingRead->dimensionCounter = 0;
								vectorBeingRead->vectorSymbol = item;
								vectorBeingRead->resultsRegisters = NULL;

								//Cria um nodo de vetor indexado na AST
								vectorBeingRead->vectorNode = createRoot(IKS_AST_VETOR_INDEXADO);
								//Associa o tipo do nó no nodo de vetor indexado (o tipo do vetor)
								vectorBeingRead->vectorNode->type = item->valueType;
								//Cria um nodo de identificador como filho do nodo de vetor indexado
								appendOnChildPointer(vectorBeingRead->vectorNode, createRoot(IKS_AST_IDENTIFICADOR));
								//Associa um ponteiro para a entrada na tabela de símbolos no nodo do identificador
								vectorBeingRead->vectorNode->child->dictPointer = item;
							}

							lista_de_dimensoes
							{
								//Obtém o vetor que acaba de ser lido da pilha de vetores
								VectorReadingInfo *vectorRead = (VectorReadingInfo *)getTop(vectorStack);
								pop(&vectorStack);
								
								//Verifica se o número de dimensoes lido é maior ou menor que o número de dimensoes declarado
								//Se sim, imprime erro e termina
								if(vectorRead->dimensionCounter != countListNodes(vectorRead->vectorSymbol->dimensionList)){
									printf("O vetor multidimensional '%s' utilizado na linha %d possui %d dimensoes e nao %d.\n", vectorRead->vectorSymbol->key, obtemLinhaAtual(), countListNodes(vectorRead->vectorSymbol->dimensionList), vectorRead->dimensionCounter);
									exit(IKS_ERROR_WRONG_DIM_NUMBER);
								}

								//Associa a sub-árvore das expressões como filha do nodo de vetor indexado
								appendOnChildPointer(vectorRead->vectorNode, $3);

								//Obtém registrador de resultado
								vectorRead->vectorNode->resultRegister = getRegister();

								//Gera código para calcular o endereço da variável
								//gera valor variável
								comp_list_t *ptDimension = vectorRead->vectorSymbol->dimensionList;
								comp_list_t *ptAccess = vectorRead->resultsRegisters;
								int tmpRegister1, tmpRegister2;
								int counter = 1;
								while(counter <= vectorRead->dimensionCounter){
									if(counter == 1){
										tmpRegister1 = getRegister();
										insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_I2I, *((int *)ptAccess->data), tmpRegister1, 0);
									}else{
										tmpRegister2 = getRegister();
										insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_MULTI, tmpRegister1, *((int *)ptDimension->data), tmpRegister2);
										insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_ADD, tmpRegister2, *((int *)ptAccess->data), tmpRegister1);
									}
									ptAccess = ptAccess->next;
									ptDimension = ptDimension->next;
									counter++;
								}

								//multiplica pelo tamanho do tipo
								tmpRegister2 = getRegister();
								switch(vectorRead->vectorSymbol->valueType){
									case IKS_INT: insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_MULTI, tmpRegister1, IKS_INT_SIZE, tmpRegister2); break;
									case IKS_FLOAT: insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_MULTI, tmpRegister1, IKS_FLOAT_SIZE, tmpRegister2); break;
									case IKS_CHAR: insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_MULTI, tmpRegister1, IKS_CHAR_SIZE, tmpRegister2); break;
									case IKS_STRING: insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_MULTI, tmpRegister1, IKS_STRING_SIZE, tmpRegister2); break;
									case IKS_BOOL: insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_MULTI, tmpRegister1, IKS_BOOL_SIZE, tmpRegister2); break;
								}
	
								//soma valor com o endereço base
								insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_ADDI, tmpRegister2, vectorRead->vectorSymbol->address, vectorRead->vectorNode->resultRegister);
								insertOperation(&(vectorRead->vectorNode->code), ILOC_NO_LABEL, ILOC_ADDI, vectorRead->vectorNode->resultRegister, ILOC_BSS, vectorRead->vectorNode->resultRegister);
								
								$$ = vectorRead->vectorNode;
								clearList(&(vectorRead->resultsRegisters));
								free(vectorRead);
							};








lista_de_dimensoes: '[' expressao ']' 											
							{	
								//Verifica se a expressão é compatível com o tipo inteiro
								//Se não for, imprime erro e termina
								switch(((comp_tree_t *)$2)->type){
									case IKS_INT: break;
									case IKS_FLOAT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao float -> int
									case IKS_BOOL: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao bool -> int
									case IKS_CHAR: printf("Nao eh possivel converter um tipo char para um tipo int na linha %d.\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
									case IKS_STRING: printf("Nao eh possivel converter um tipo string para um tipo int na linha %d.\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
								}

								//Obtém o vetor que está sendo lido da pilha de vetores
								VectorReadingInfo *vectorBeingRead = (VectorReadingInfo *)getTop(vectorStack);
								
								//Insere o registrador de resultado da expressão na lista de registradores de resultados
								insertHead(&(vectorBeingRead->resultsRegisters), &(((comp_tree_t *)$2)->resultRegister));

								vectorBeingRead->dimensionCounter++;
								$$ = $2;

								//concatena o código da expressão no código do vetor
								concatCode(&(vectorBeingRead->vectorNode->code), &(((comp_tree_t *)$2)->code));
							}

										| '[' expressao ']' lista_de_dimensoes
							{	
								//Verifica se a expressão é compatível com o tipo inteiro
								//Se não for, imprime erro e termina
								switch(((comp_tree_t *)$2)->type){
									case IKS_INT: break;
									case IKS_FLOAT: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_FLOAT_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao float -> int
									case IKS_BOOL: ((comp_tree_t *)$2)->tipoCoercao = IKS_COERCAO_BOOL_INT; ((comp_tree_t *)$2)->type = IKS_INT; break; //Coercao bool -> int
									case IKS_CHAR: printf("Nao eh possivel converter um tipo char para um tipo int na linha %d.\n", obtemLinhaAtual()); exit(IKS_ERROR_CHAR_TO_X); break;
									case IKS_STRING: printf("Nao eh possivel converter um tipo string para um tipo int na linha %d.\n", obtemLinhaAtual()); exit(IKS_ERROR_STRING_TO_X); break;
								}
								
								//Obtém o vetor que está sendo lido da pilha de vetores
								VectorReadingInfo *vectorBeingRead = (VectorReadingInfo *)getTop(vectorStack);

								//Insere o registrador de resultado da expressão na lista de registradores de resultados
								insertHead(&(vectorBeingRead->resultsRegisters), &(((comp_tree_t *)$2)->resultRegister));

								vectorBeingRead->dimensionCounter++;
								$$ = $2;
								appendOnChildPointer($$, $4);

								//concatena o código da expressão no código do vetor
								concatCode(&(vectorBeingRead->vectorNode->code), &(((comp_tree_t *)$2)->code));
							};






/* Tipos */
tipo:	TK_PR_INT				{ $$ = IKS_INT; }
			| TK_PR_FLOAT		{ $$ = IKS_FLOAT; }
			| TK_PR_CHAR		{ $$ = IKS_CHAR; }
			| TK_PR_BOOL		{ $$ = IKS_BOOL; }
			| TK_PR_STRING	{ $$ = IKS_STRING; };

		
%%

comp_tree_t *createRoot(int value){
	comp_tree_t *root;
	createTree(&root);
	insert(&root, value);
	return root;
}

/* Imprime a mensagem de erro semântico e termina */
int semanticError(int errorType, char *format, ...){
	va_list ap;
	char buffer[500];
	va_start(ap, format);

	vsnprintf(buffer, 500, format, ap);
	printf("%s\n", buffer);

	va_end(ap);
	exit(errorType);
}



