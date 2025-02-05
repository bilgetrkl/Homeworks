%{
#include <stdio.h>
extern int yylineno; 
%}

%option yylineno 
%x SKIP_LINE

%%

"and"       { printf("KW_AND\n"); }
"or"        { printf("KW_OR\n"); }
"not"       { printf("KW_NOT\n"); }
"equal"     { printf("KW_EQUAL\n"); }
"less"      { printf("KW_LESS\n"); }
"nil"       { printf("KW_NIL\n"); }
"list"      { printf("KW_LIST\n"); }
"append"    { printf("KW_APPEND\n"); }
"concat"    { printf("KW_CONCAT\n"); }
"set"       { printf("KW_SET\n"); }
"deffun"    { printf("KW_DEFFUN\n"); }
"for"       { printf("KW_FOR\n"); }
"if"        { printf("KW_IF\n"); }
"exit"      { printf("KW_EXIT\n"); }
"load"      { printf("KW_LOAD\n"); }
"print"     { printf("KW_DISP\n"); }
"true"      { printf("KW_TRUE\n"); }
"false"     { printf("KW_FALSE\n"); }

"+"         { printf("OP_PLUS\n"); }
"-"         { printf("OP_MINUS\n"); }
"/"         { printf("OP_DIV\n"); }
"*"         { printf("OP_MULT\n"); }
"("         { printf("OP_OP\n"); }
")"         { printf("OP_CP\n"); }
","         { printf("OP_COMMA\n"); }

";"[^\n]*   { printf("COMMENT\n"); }
[0-9]+      { printf("VALUEI \n"); }
[0-9]+"f"[0-9]+ { printf("VALUEF\n"); }
[a-zA-Z][a-zA-Z0-9_]* { printf("IDENTIFIER\n"); }
\n         { }
[ \t\r]+    {  }
[0-9]+[a-zA-Z_][a-zA-Z0-9_]* { printf("SYNTAX_ERROR: %s at line %d is not a value\n", yytext, yylineno); }
.           { printf("SYNTAX_ERROR: %s at line %d cannot be tokenized\n", yytext, yylineno); }  

<SKIP_LINE>\n { BEGIN(INITIAL); } 
<SKIP_LINE>.  {  }

%%

int main(int argc, char **argv) {
    FILE *file = NULL;

    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("Error opening file");
            return 1;
        }
        yyin = file; 
        yylex();      
        fclose(file); 
    } else {
        yyin = stdin;
        printf("Enter input:\n");
        while (!feof(stdin)) {
            yylex(); 
        }
    }

    return 0;
}

