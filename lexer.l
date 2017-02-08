%{
//header section insert into output
#include "FSM.h"
#include "parser.h"
#include <stdio.h>
%}

%option outfile="lexer.c" header-file="lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

FSMTYPE "F"
STATES "s"
INPUTS "i"
OUTPUTS "o"
INITIAL "n0"
TRANS "p"

INTEGER [0-9]+

WS	[ \t\r\n]+

COMMENT	\/\/[^\n]*

/* begin tokenizing*/
%%
{WS} { /*ignore white spaces, we are kind*/ }
{COMMENT} { /*ignore comments*/ }
{FSMTYPE} {return TOKEN_FSMTYPE;}
{STATES} {return TOKEN_STATES;}
{INPUTS} {return TOKEN_INPUTS;}
{OUTPUTS} {return TOKEN_OUTPUTS;}
{INITIAL} {return TOKEN_INITIAL;}
{TRANS} {return TOKEN_TRANS;}
{INTEGER} {sscanf(yytext, "%llu", &yylval->num); return TOKEN_NUMBER;}

. { printf("Warning! Unrecognized token: %s\n", yytext); } 

%%

