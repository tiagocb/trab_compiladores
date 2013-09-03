%{
#include "parser.h"
#include "comp_dict.h"

#define IKS_SIMBOLO_INDEFINIDO 0
#define IKS_SIMBOLO_LITERAL_INT 1
#define IKS_SIMBOLO_LITERAL_FLOAT 2
#define IKS_SIMBOLO_LITERAL_CHAR 3
#define IKS_SIMBOLO_LITERAL_STRING 4
#define IKS_SIMBOLO_LITERAL_BOOL 5
#define IKS_SIMBOLO_IDENTIFICADOR 6

int linha = 1;
int obtemLinhaAtual();

comp_dict_t tabelaDeSimbolos;
void inicializaTabelaDeSimbolos();
void insereLexema(int tipo);
void imprimeTabelaDeSimbolos();
%}

CARACTER_ [A-Za-z_]
DIGITO [0-9]
%x IN_COMMENT

%%
\n { linha++;}
int { return TK_PR_INT; }
float { return TK_PR_FLOAT; }
bool { return TK_PR_BOOL; }
char { return TK_PR_CHAR; }
string { return TK_PR_STRING; }
if { return TK_PR_IF; }
then { return TK_PR_THEN; }
else { return TK_PR_ELSE; }
while { return TK_PR_WHILE; }
do { return TK_PR_DO; }
input { return TK_PR_INPUT; }
output { return TK_PR_OUTPUT; }
return { return TK_PR_RETURN; }

","|";"|":"|"("|")"|"["|"]"|"{"|"}"|"+"|"-"|"*"|"/"|"<"|">"|"="|"!"|"|"|"&"|"$" { return yytext[0]; }

"<""=" { return TK_OC_LE; }
">""=" { return TK_OC_GE; }
"=""=" { return TK_OC_EQ; }
"!""=" { return TK_OC_NE; }
"&""&" { return TK_OC_AND; }
"|""|" { return TK_OC_OR; }

{DIGITO}+ { insereLexema(IKS_SIMBOLO_LITERAL_INT); return TK_LIT_INT; }
("-")?{DIGITO}+"."{DIGITO}+ { insereLexema(IKS_SIMBOLO_LITERAL_FLOAT); return TK_LIT_FLOAT; }
false { insereLexema(IKS_SIMBOLO_LITERAL_BOOL); return TK_LIT_FALSE; }
true { insereLexema(IKS_SIMBOLO_LITERAL_BOOL); return TK_LIT_TRUE; }
("'")([^\n])("'") { insereLexema(IKS_SIMBOLO_LITERAL_CHAR); return TK_LIT_CHAR; }
("\"")([^"\""]*)("\"") { insereLexema(IKS_SIMBOLO_LITERAL_STRING); return TK_LIT_STRING; }

{CARACTER_}({CARACTER_}|{DIGITO})* { insereLexema(IKS_SIMBOLO_IDENTIFICADOR); return TK_IDENTIFICADOR; }

"/""/"[^\n]* { } //comentarios comecando por //

"/""*" { BEGIN(IN_COMMENT); }	//comentarios do tipo /*...*/
<IN_COMMENT>{
	"*""/" { BEGIN(INITIAL); }
	[^"*"\n]+ { }   // eat comment in chunks
	"*"       { }       // eat the lone star
	\n        { linha++; }
	.	  { }
}

" " { }
"	" { }
. { insereLexema(IKS_SIMBOLO_INDEFINIDO); return TOKEN_ERRO; }
%%

int obtemLinhaAtual(){
	return linha;
}

void inicializaTabelaDeSimbolos(){
	createDictionaty(&tabelaDeSimbolos, 10);
}

void insereLexema(int tipo){
	insertKey(&tabelaDeSimbolos, yytext, linha, tipo);
}

void imprimeTabelaDeSimbolos(){
	printDictionary(tabelaDeSimbolos);
}

