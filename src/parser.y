%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

ASTNode* ast_root = NULL;
%}

%union {
    int ival;
    char* sval;
    struct ASTNode* node;
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token INT IF ELSE FOR RETURN
%token PLUS MINUS TIMES DIVIDE
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN
%token EQ NEQ LT LE GT GE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA

%type <node> program function_def statement statement_list expression
%type <node> declaration assignment for_statement if_statement return_statement

%%

program
    : function_def { ast_root = $1; }
    ;

function_def
    : INT IDENTIFIER LPAREN RPAREN LBRACE statement_list RBRACE
        { $$ = make_function_node($2, $6); free($2); }
    ;

statement_list
    : statement { $$ = $1; }
    | statement_list statement { $$ = make_seq_node($1, $2); }
    ;

statement
    : declaration SEMICOLON { $$ = $1; }
    | assignment SEMICOLON { $$ = $1; }
    | if_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | return_statement SEMICOLON { $$ = $1; }
    ;

declaration
    : INT IDENTIFIER ASSIGN expression
        { $$ = make_decl_node($2, $4); free($2); }
    ;

assignment
    : IDENTIFIER ASSIGN expression
        { $$ = make_binop_node('=', make_var_node($1), $3); free($1); }
    | IDENTIFIER PLUS_ASSIGN expression
        { $$ = make_binop_node('=', make_var_node($1),
                              make_binop_node('+', make_var_node($1), $3)); free($1); }
    ;

if_statement
    : IF LPAREN expression RPAREN LBRACE statement_list RBRACE
        { $$ = make_if_node($3, $6); }
    ;

for_statement
    : FOR LPAREN declaration SEMICOLON expression SEMICOLON assignment RPAREN LBRACE statement_list RBRACE
        { $$ = make_for_node($3, $5, $7, $10); }
    ;

return_statement
    : RETURN expression
        { $$ = make_return_node($2); }
    ;

expression
    : NUMBER { $$ = make_int_node($1); }
    | IDENTIFIER { $$ = make_var_node($1); free($1); }
    | expression PLUS expression { $$ = make_binop_node('+', $1, $3); }
    | expression MINUS expression { $$ = make_binop_node('-', $1, $3); }
    | expression TIMES expression { $$ = make_binop_node('*', $1, $3); }
    | expression DIVIDE expression { $$ = make_binop_node('/', $1, $3); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parser error: %s\n", s);
    exit(1);
}
