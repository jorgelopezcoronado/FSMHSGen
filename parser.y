%{

#include "FSM.h"
#include "parser.h"
#include "lexer.h"

int fsmerror(fsm_ll **machine, yyscan_t scanner, const char *msg) 
{
	printf("FSM file Unrecognized structure, message: %s\n", msg);
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
%error-verbose 
%name-prefix "fsm" 
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
	: TOKEN_FSMTYPE TOKEN_NUMBER { /*just ignore the machine type for us*/}  
	| TOKEN_FSMTYPE TOKEN_NUMBER  subheader { /*just ignore the machine type for us*/}  
	;
subheader
	: properties
	| subheader properties  
	;
properties
	: TOKEN_STATES TOKEN_NUMBER {set_maxs(*machine,$2);} 
	| TOKEN_INPUTS TOKEN_NUMBER {set_maxi(*machine,$2);} 
	| TOKEN_OUTPUTS TOKEN_NUMBER {set_maxo(*machine,$2);} 
	| TOKEN_TRANS TOKEN_NUMBER {set_maxt(*machine,$2);} 
	| TOKEN_INITIAL TOKEN_NUMBER {set_init(*machine,$2);} 
	;
transitions 
	: TOKEN_NUMBER TOKEN_NUMBER TOKEN_NUMBER TOKEN_NUMBER transitions {add_fsm_ll_transition(*machine, $1, $2, $4, $3);}
	|
	;
%%

