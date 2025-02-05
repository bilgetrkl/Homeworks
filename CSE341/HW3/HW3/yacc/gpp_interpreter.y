%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

int yylex();
int yydebug = 0;

extern int yylineno;
extern FILE *yyin;

void yyerror(const char *s);
%}

%union {
    double dval;
    char *sval;
}

%token KW_AND KW_OR KW_NOT KW_EQUAL KW_LESS KW_SET KW_DEFVAR KW_DEFFUN KW_FOR KW_IF KW_WHILE
%token KW_LIST KW_APPEND KW_CONCAT KW_NIL KW_EXIT KW_LOAD KW_DISP KW_TRUE KW_FALSE
%token OP_PLUS OP_MINUS OP_DIV OP_MULT OP_OP OP_CP OP_COMMA OP_APO
%token <dval> VALUEI VALUEF IDENTIFIER
%token <sval> STRING

%%

start:
    input
    ;

input:
    exp_list
    ;

exp_list:
    exp_list exp  
    | exp
    ;

exp:
    OP_OP OP_PLUS exp exp OP_CP
    | OP_OP OP_MINUS exp exp OP_CP
    | OP_OP OP_MULT exp exp OP_CP
    | OP_OP OP_DIV exp exp OP_CP
    | VALUEI
    | VALUEF
    | IDENTIFIER
    | expb
    | expi
    | list
    | function_call
    | print_statement
    | load_statement
    ;

expb:
    OP_OP KW_AND exp exp OP_CP
    | OP_OP KW_OR exp exp OP_CP
    | OP_OP KW_NOT exp OP_CP
    | OP_OP KW_LESS exp exp OP_CP
    | OP_OP KW_EQUAL exp exp OP_CP
    | KW_TRUE
    | KW_FALSE
    ;

expi:
    OP_OP KW_DEFFUN IDENTIFIER OP_OP exp_list OP_CP exp_list OP_CP
    | OP_OP KW_SET IDENTIFIER exp OP_CP
    | OP_OP KW_IF expb exp OP_CP
    | OP_OP KW_IF expb exp exp OP_CP
    | OP_OP KW_DEFVAR IDENTIFIER exp OP_CP
    | OP_OP KW_WHILE expb exp_list OP_CP
    | OP_OP KW_FOR OP_OP IDENTIFIER exp exp OP_CP exp_list OP_CP
    | OP_OP KW_CONCAT list list OP_CP 
    | OP_OP KW_APPEND list list OP_CP
    | OP_OP KW_EXIT OP_CP
    ;

list:
    OP_OP KW_LIST values OP_CP
    | OP_APO OP_OP values OP_CP 
    | OP_APO OP_OP OP_CP
    | KW_NIL
    ;

values:
    values OP_COMMA value
    | value
    ;

value:
    VALUEF
    | VALUEI
    | IDENTIFIER
    ;

function_call:
    OP_OP IDENTIFIER exp_list OP_CP
    ;

load_statement:
    OP_OP KW_LOAD STRING OP_CP
    ;

print_statement:   
    OP_OP KW_DISP exp OP_CP
    | OP_OP KW_DISP STRING OP_CP
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: ./%s [file] or ./%s \n", argv[0], argv[0]);
        return 1;
    }

    if (argc == 2) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("An error occurred");
            return 1;
        }

        yyin = file;
        if (yyparse() == 0) {
            printf("ACCEPT\n");
        } else {
            printf("REJECT\n");
        }

        fclose(file);
        return 0;
    }

    while (1) {
        char input[1024];
        printf("Enter the input:\n");
        if (!fgets(input, sizeof(input), stdin)) {
            continue;
        }

        FILE *fp = fmemopen(input, strlen(input), "r");
        if (!fp) {
            perror("An error occurred");
            continue;
        }

        yyin = fp;
        if (yyparse() == 0) {
            printf("ACCEPT\n");
        } else {
            printf("REJECT\n");
        }

        fclose(fp);
    }

    return 0;
}
