%{

#include "linked_list.h"
#include "fpp.h"
#include "fpl.h"

int yyerror(linded_list **ll, yyscan_t scanner, const char *msg) 
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

%output "fpp.c"
%defines "fpp.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { linked_list **ll }
%parse-param { yyscan_t scanner }

%union
{
	char *str;
	linked_list *ll;
}

%token TOKEN_EQ
%token TOKEN_MM
%token TOKEN_MT
%token TOKEN_ML
%token TOKEN_LT
%token TOKEN_LL
%token TOKEN_SL
%token TOKEN_LC
%token TOKEN_TR
%token TOKEN_FL
%token TOKEN_PM
%token TOKEN_NM
%token TOKEN_OM
%token TOKEN_LM
%token TOKEN_NUMBER

%type <str> TOKEN_NUMBER 

%%

input
	: list
	;
list
	: assignation
	| list assignation
	;
assignation
	: TOKEN_MM TOKEN_EQ TOKEN_NUMBER {linked_list_add(ll, "-mm"); linked_list_add(ll, $3);} 
	| TOKEN_MT TOKEN_EQ TOKEN_NUMBER {linked_list_add(ll, "-mt"); linked_list_add(ll, $3);} 
	| TOKEN_ML TOKEN_EQ TOKEN_NUMBER {linked_list_add(ll, "-ml"); linked_list_add(ll, $3);} 
	| TOKEN_LT TOKEN_EQ TOKEN_SL {linked_list_add(ll, "-lt"); linked_list_add(ll, "syslog");} 
	| TOKEN_LT TOKEN_EQ TOKEN_LC {linked_list_add(ll, "-lt"); linked_list_add(ll, "local");} 
	| TOKEN_LL TOKEN_EQ TOKEN_NUMBER {linked_list_add(ll, "-ll"); linked_list_add(ll, $3);} 
	| TOKEN_PM TOKEN_EQ TOKEN_TR {linked_list_add(ll, "-psm");} 
	| TOKEN_NM TOKEN_EQ TOKEN_TR {linked_list_add(ll, "-ndsm");} 
	| TOKEN_OM TOKEN_EQ TOKEN_TR {linked_list_add(ll, "-nosm");} 
	| TOKEN_LM TOKEN_EQ TOKEN_TR {linked_list_add(ll, "-hslm");} 
	| TOKEN_PM TOKEN_EQ TOKEN_FL {} 
	| TOKEN_NM TOKEN_EQ TOKEN_FL {} 
	| TOKEN_OM TOKEN_EQ TOKEN_FL {} 
	| TOKEN_LM TOKEN_EQ TOKEN_FL {} 
	;
%%

