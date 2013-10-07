%{
#include <string.h>
#include "parser.h"
#include "main.h"

int linha = 1;
int obtemLinhaAtual();
%}

CARACTER_	[A-Za-z_]
DIGITO		[0-9]
%x IN_COMMENT
%option noyywrap

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


{DIGITO}+							{ yylval.symbol = strdup(yytext); return TK_LIT_INT; }
{DIGITO}+"."{DIGITO}+				{ yylval.symbol = strdup(yytext); return TK_LIT_FLOAT; }
false								{ yylval.symbol = strdup(yytext); return TK_LIT_FALSE; }
true								{ yylval.symbol = strdup(yytext); return TK_LIT_TRUE; }
("'")([^\n])("'")					{ yylval.symbol = strdup(yytext); return TK_LIT_CHAR; }
("\"")([^"\""]*)("\"")				{ yylval.symbol = strdup(yytext); return TK_LIT_STRING; }
{CARACTER_}({CARACTER_}|{DIGITO})*	{ yylval.symbol = strdup(yytext); return TK_IDENTIFICADOR; }

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
