%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h" 

extern int yylineno;
%}

%option yylineno
%option noyywrap
%x SKIP_LINE

%%

"and"       { return KW_AND; }
"or"        { return KW_OR; }
"not"       { return KW_NOT; }
"equal"     { return KW_EQUAL; }
"less"      { return KW_LESS; }
"nil"       { return KW_NIL; }
"list"      { return KW_LIST; }
"append"    { return KW_APPEND; }
"concat"    { return KW_CONCAT; }
"set"       { return KW_SET; }
"defvar"    { return KW_DEFVAR;}
"deffun"    { return KW_DEFFUN; }
"for"       { return KW_FOR; }
"if"        { return KW_IF; }
"exit"      { return KW_EXIT; }
"load"      { return KW_LOAD; }
"print"     { return KW_DISP; }
"true"      { return KW_TRUE; }
"false"     { return KW_FALSE; }
"while"     { return KW_WHILE; }

"+"         { return OP_PLUS; }
"-"         { return OP_MINUS; }
"/"         { return OP_DIV; }
"*"         { return OP_MULT; }
"("         { return OP_OP; }
")"         { return OP_CP; }
","         { return OP_COMMA; }
"'"         { return OP_APO; }

\"[^\"]*\"  { yylval.sval = strdup(yytext); return STRING; }
[0-9]+      { yylval.dval = atoi(yytext); return VALUEI; }
[0-9]+"."[0-9]+ { yylval.dval = atof(yytext); return VALUEF; }
[a-zA-Z][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
\n      
[ \t\r]+   
[0-9]+[a-zA-Z_][a-zA-Z0-9_]* { printf("SYNTAX_ERROR: %s at line %d is not a value\n", yytext, yylineno); }
.           { printf("SYNTAX_ERROR: %s at line %d cannot be tokenized\n", yytext, yylineno); }

<SKIP_LINE>\n { BEGIN(INITIAL); } 
<SKIP_LINE>. 

%%
