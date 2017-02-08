%{

#include "FSM.h"
#include "parser.h"
#include "lexer.h"

int yyerror(fsm_ll **machine, yyscan_t scanner, const char *msg) 
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
%parse-param { fsm_ll **machine }
%parse-param { yyscan_t scanner }

%union
{
	size_t num;
	fsm_ll *machine;
}

%token TOKEN_FSMTYPE
%token TOKEN_STATES
%token TOKEN_INPUTS
%token TOKEN_OUTPUTS
%token TOKEN_INITIAL
%token TOKEN_TRANS
%token TOKEN_NUMBER

%type <num> TOKEN_NUMBER 
%%

input
	: fsm 
	;
fsm
	: header transitions 
	;	
header	
	: TOKEN_FSMTYPE TOKEN_NUMBER  subheader { /*just ignore the machine type for us*/}  
	;
subheader
	: TOKEN_STATES TOKEN_NUMBER subheader {set_maxs(*machine,$2);} 
	| TOKEN_INPUTS TOKEN_NUMBER subheader {set_maxi(*machine,$2);} 
	| TOKEN_OUTPUTS TOKEN_NUMBER subheader {set_maxo(*machine,$2);} 
	| TOKEN_TRANS TOKEN_NUMBER subheader {set_maxt(*machine,$2);} 
	| TOKEN_INITIAL TOKEN_NUMBER subheader {set_init(*machine,$2);} 
	|
	;
transitions 
	: TOKEN_NUMBER TOKEN_NUMBER TOKEN_NUMBER TOKEN_NUMBER transitions {add_fsm_ll_transition(*machine, $2, $1, $4, $3);}
	|
	;
%%

