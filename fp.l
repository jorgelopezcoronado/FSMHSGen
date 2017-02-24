%{
//header section insert into output
#include "linked_list.h"
#include "fpp.h"
#include <stdio.h>
%}

%option outfile="fpl.c" header-file="fpl.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge
%option caseless 

EQ "="
MM "MaxMemSize"
MT "MaxTime"
ML "MaxLength"
LT "LogType"
LL "LogLevel"
SL "syslog"
LC "local"
TR "true"
FL "false"
PM "psm"
NM "ndsm"
OM "nosm"
LM "hslm"

INTEGER [0-9]+

WS	[ \t\r\n]+

COMMENT	#[^\n]*

/* begin tokenizing*/
%%
{WS} { /*ignore white spaces, we are kind*/ }
{COMMENT} { /*ignore comments*/ }
{EQ} {return TOKEN_EQ;}
{MM} {return TOKEN_MM;}
{MT} {return TOKEN_MT;}
{ML} {return TOKEN_ML;}
{LT} {return TOKEN_LT;}
{LL} {return TOKEN_LL;}
{SL} {return TOKEN_LL;}
{LC} {return TOKEN_LL;}
{TR} {return TOKEN_TR;}
{FL} {return TOKEN_FL;}
{PM} {return TOKEN_PM;}
{NM} {return TOKEN_NM;}
{OM} {return TOKEN_OM;}
{LM} {return TOKEN_LM;}
{INTEGER} {sscanf(yytext, "%s", &yylval->num); return TOKEN_NUMBER;}

. { printf("Warning! Unrecognized token: %s\n", yytext); } 

%%

