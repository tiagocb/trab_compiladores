%{
#include "tokens.h"
int counter = 0;
%}
%%
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



. { return TOKEN_ERRO; }
%%
