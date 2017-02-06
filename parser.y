%{

#include "paren.h"
#include "parser.h"
#include "lexer.h"

int yyerror(paren **paren, yyscan_t scanner, const char *msg) 
{
	printf("Unrecognized structure, message: %s\n", msg);
}

%}


%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output "parser.c"
%defines "parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { paren **parenobj }
%parse-param { yyscan_t scanner }

%union 
{
	paren *parenobj;
}

%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_LBRACK
%token TOKEN_RBRACK
%token TOKEN_LCBRACK
%token TOKEN_RCBRACK


%type <parenobj> P P2

%%

input
	: P { *parenobj = $1; }
	;
P	
	: P2[le] P[ri] { $$ = createParen(NONE, $le, $ri);} 
	| P2[p] {$$ = $p;}
	;
P2
	: TOKEN_LPAREN P[p] TOKEN_RPAREN { $$ = createParen(PAREN, $p, NULL); }
	| TOKEN_LBRACK P[p] TOKEN_RBRACK { $$ = createParen(BRACK, $p, NULL); }
	| TOKEN_LCBRACK P[p] TOKEN_RCBRACK { $$ = createParen(CBRACK, $p, NULL); }
	| TOKEN_LPAREN TOKEN_RPAREN { $$ = createParen(PAREN, NULL, NULL); }
	| TOKEN_LBRACK TOKEN_RBRACK { $$ = createParen(BRACK, NULL, NULL); }
	| TOKEN_LCBRACK TOKEN_RCBRACK { $$ = createParen(CBRACK, NULL, NULL); }
	;

%%

