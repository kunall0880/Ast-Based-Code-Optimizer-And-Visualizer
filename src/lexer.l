%{
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

void yyerror(const char* s);
%}

%%

[ \t\n]+        ; /* Skip whitespace */
"int"           { return INT; }
"if"            { return IF; }
"else"          { return ELSE; }
"for"           { return FOR; }
"return"        { return RETURN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return DIVIDE; }
"="             { return ASSIGN; }
"+="            { return PLUS_ASSIGN; }
"-="            { return MINUS_ASSIGN; }
"=="            { return EQ; }
"!="            { return NEQ; }
"<"             { return LT; }
"<="            { return LE; }
">"             { return GT; }
">="            { return GE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
";"             { return SEMICOLON; }
","             { return COMMA; }

[0-9]+          { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }

.               { printf("Unexpected character: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}
