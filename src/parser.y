%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror (char const *s);
%}
%start tempcheck // For testing purpose. TO DO: change to program

%token TK_PR_INT        // int
%token TK_PR_FLOAT      // float
%token TK_PR_BOOL       // bool
%token TK_PR_CHAR       // char
%token TK_PR_STRING     // string
%token TK_PR_IF         // if
%token TK_PR_THEN       // then
%token TK_PR_ELSE       // else
%token TK_PR_WHILE      // while
%token TK_PR_DO         // do
%token TK_PR_INPUT      // input
%token TK_PR_OUTPUT     // output
%token TK_PR_RETURN     // return
%token TK_PR_CONST      // const
%token TK_PR_STATIC     // static
%token TK_PR_FOREACH    // foreach
%token TK_PR_FOR        // for
%token TK_PR_SWITCH     // switch
%token TK_PR_CASE       // case
%token TK_PR_BREAK      // break
%token TK_PR_CONTINUE   // continue
%token TK_PR_CLASS      // class
%token TK_PR_PRIVATE    // private
%token TK_PR_PUBLIC     // public
%token TK_PR_PROTECTED  // protected
%token TK_PR_END        // end
%token TK_PR_DEFAULT    // default
%token TK_OC_LE         // <=
%token TK_OC_GE         // >=
%token TK_OC_EQ         // ==
%token TK_OC_NE         // !=
%token TK_OC_AND        // &&
%token TK_OC_OR         // ||
%token TK_OC_SL         // >>
%token TK_OC_SR         // <<
%token TK_LIT_INT       // 3
%token TK_LIT_FLOAT     // 3.7
%token TK_LIT_FALSE     // false
%token TK_LIT_TRUE      // true
%token TK_LIT_CHAR      // 'a'
%token TK_LIT_STRING    // "str"
%token TK_IDENTIFICADOR // x0
%token TOKEN_ERRO

%%

// for checking if the input is valid. TO DO: delete
tempcheck:  program { printf("Valid input!\n"); } ;

program:	/* empty? */

            // Assignment
            atrib

            // Input & Output
            | TK_PR_INPUT TK_IDENTIFICADOR
            | TK_PR_OUTPUT TK_IDENTIFICADOR
            | TK_PR_OUTPUT literal

            // TODO: Functions

            // Shift operations
            | TK_IDENTIFICADOR shiftOp TK_LIT_INT
            | TK_IDENTIFICADOR '[' expression ']' shiftOp TK_LIT_INT

            // Simple statements
            | TK_PR_RETURN
            | TK_PR_BREAK
            | TK_PR_CONTINUE

            TK_PR_IF '(' expression ')' block
            TK_PR_IF '(' expression ')' block TK_PR_ELSE block
            TK_PR_FOR '(' atrib ':' expression ':' atrib ')' block // TODO: tá certo isso?
            TK_PR_WHILE '(' expression ')' TK_PR_DO block
            ;

atrib:      | TK_IDENTIFICADOR '=' expression
            | TK_IDENTIFICADOR '[' expression ']' '=' expression
            ;

literal:    TK_LIT_CHAR
            | TK_LIT_FALSE
            | TK_LIT_TRUE
            | TK_LIT_FLOAT
            | TK_LIT_INT
            | TK_LIT_STRING
            ;

shiftOp:    TK_OC_SL
            | TK_OC_SR
            ;

expression: TK_PR_INT; // TO DO

block:      TK_PR_INT; // TO DO

%%

int yyerror(char const *s) {
    extern char *yytext;
    extern int num_lines;

    printf("Error %s: at symbol %s, in line %d.\n", s, yytext, num_lines);
    return 1;
}