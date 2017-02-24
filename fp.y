%{

#include "linked_list.h"
#include "fpp.h"
#include "fpl.h"

int fperror(linked_list **ll, yyscan_t scanner, const char *msg) 
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
%error-verbose 
%name-prefix "fp"
%lex-param   { yyscan_t scanner }
%parse-param { linked_list **ll }
%parse-param { yyscan_t scanner }

%union
{
	char *str;
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
	: TOKEN_MM TOKEN_EQ TOKEN_NUMBER {char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, $3)+ 1)); sprintf(msg, $3); linked_list_add_end(*ll, "-mm"); linked_list_add_end(*ll, msg);} 
	| TOKEN_MT TOKEN_EQ TOKEN_NUMBER {char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, $3)+ 1)); sprintf(msg, $3); linked_list_add_end(*ll, "-mt"); linked_list_add_end(*ll, msg);} 
	| TOKEN_ML TOKEN_EQ TOKEN_NUMBER {char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, $3)+ 1)); sprintf(msg, $3); linked_list_add_end(*ll, "-ml"); linked_list_add_end(*ll, msg);} 
	| TOKEN_LT TOKEN_EQ TOKEN_SL {linked_list_add_end(*ll, "-lt"); linked_list_add_end(*ll, "syslog");} 
	| TOKEN_LT TOKEN_EQ TOKEN_LC {linked_list_add_end(*ll, "-lt"); linked_list_add_end(*ll, "local");} 
	| TOKEN_LL TOKEN_EQ TOKEN_NUMBER {char *msg = (char*)malloc(sizeof(char)*(snprintf(NULL, 0, $3)+ 1)); sprintf(msg, $3); linked_list_add_end(*ll, "-ll"); linked_list_add_end(*ll, msg);} 
	| TOKEN_PM TOKEN_EQ TOKEN_TR {linked_list_add_end(*ll, "-psm");} 
	| TOKEN_NM TOKEN_EQ TOKEN_TR {linked_list_add_end(*ll, "-ndsm");} 
	| TOKEN_OM TOKEN_EQ TOKEN_TR {linked_list_add_end(*ll, "-nosm");} 
	| TOKEN_LM TOKEN_EQ TOKEN_TR {linked_list_add_end(*ll, "-hslm");} 
	| TOKEN_PM TOKEN_EQ TOKEN_FL {} 
	| TOKEN_NM TOKEN_EQ TOKEN_FL {} 
	| TOKEN_OM TOKEN_EQ TOKEN_FL {} 
	| TOKEN_LM TOKEN_EQ TOKEN_FL {} 
	;
%%

