%{
#include "parser.h"
#include "common.h"
#include "comp_dict.h"

int linha = 1;
int obtemLinhaAtual();

comp_dict_t tabelaDeSimbolos;
void inicializaTabelaDeSimbolos();
comp_dict_item_t *insereSimbolo(int tipo);
void imprimeTabelaDeSimbolos();
%}

CARACTER_	[A-Za-z_]
DIGITO		[0-9]
%x IN_COMMENT

%%
\n	{ linha++; }
int	{ return TK_PR_INT; }
float	{ return TK_PR_FLOAT; }
bool	{ return TK_PR_BOOL; }
char	{ return TK_PR_CHAR; }
string	{ return TK_PR_STRING; }
if	{ return TK_PR_IF; }
then	{ return TK_PR_THEN; }
else	{ return TK_PR_ELSE; }
while	{ return TK_PR_WHILE; }
do	{ return TK_PR_DO; }
input	{ return TK_PR_INPUT; }
output	{ return TK_PR_OUTPUT; }
return	{ return TK_PR_RETURN; }

","|";"|":"|"("|")"|"["|"]"|"{"|"}"|"+"|"-"|"*"|"/"|"<"|">"|"="|"!"|"|"|"&"|"$" { return yytext[0]; }

"<""=" { return TK_OC_LE; }
">""=" { return TK_OC_GE; }
"=""=" { return TK_OC_EQ; }
"!""=" { return TK_OC_NE; }
"|""|" { return TK_OC_OR; }
"&""&" { return TK_OC_AND; }

{DIGITO}+		{ insereSimbolo(IKS_SIMBOLO_INT); return TK_LIT_INT; }
{DIGITO}+"."{DIGITO}+	{ insereSimbolo(IKS_SIMBOLO_FLOAT); return TK_LIT_FLOAT; }
false			{ insereSimbolo(IKS_SIMBOLO_BOOL); return TK_LIT_FALSE; }
true			{ insereSimbolo(IKS_SIMBOLO_BOOL); return TK_LIT_TRUE; }
("'")([^\n])("'")	{ insereSimbolo(IKS_SIMBOLO_CHAR); return TK_LIT_CHAR; }
("\"")([^"\""]*)("\"")	{ insereSimbolo(IKS_SIMBOLO_STRING); return TK_LIT_STRING; }

{CARACTER_}({CARACTER_}|{DIGITO})*	{ yylval.symbol = insereSimbolo(IKS_SIMBOLO_IDENTIFICADOR); return TK_IDENTIFICADOR; }

"/""/"[^\n]* { } //comentarios comecando por //

"/""*" { BEGIN(IN_COMMENT); }	//comentarios do tipo /*...*/
<IN_COMMENT>{
	"*""/"		{ BEGIN(INITIAL); }
	[^"*"\n]+	{ } // eat comment in chunks
	"*"		{ } // eat the lone star
	\n		{ linha++; }
	.		{ }
}

" "		{ }
"	"	{ }
.		{ return TOKEN_ERRO; }
%%

int obtemLinhaAtual() {
	return linha;
}

void inicializaTabelaDeSimbolos() {
	createDictionaty(&tabelaDeSimbolos, 10);
}

comp_dict_item_t *insereSimbolo(int tipo) {
	return insertKey(&tabelaDeSimbolos, yytext, tipo, linha);
}

void imprimeTabelaDeSimbolos() {
	printDictionary(tabelaDeSimbolos);
}

