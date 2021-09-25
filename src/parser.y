/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "token.h"

#define YYERROR_VERBOSE 1

extern int num_lines;
extern void* arvore;

int yylex(void);
int yyerror (char const *s);
%}

%define parse.error verbose

%union {
    struct _token_t* valor_lexico;
    struct _node_t* node;
}

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

%left '?' ':'
%left TK_OC_OR
%left TK_OC_AND
%left '|'
%left '^'
%left '&'
%left TK_OC_EQ TK_OC_NE
%left '<' '>' TK_OC_GE TK_OC_LE
%left '-' '+'
%left '*' '/' '%'
%right '#' '!'

%%

program: %empty
    | globalvardec program
    | funcdec program
    ;

globalvardec: type globalidentifierslist ';'
    | TK_PR_STATIC type globalidentifierslist ';'
    ;

type: TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL
    | TK_PR_CHAR
    | TK_PR_STRING
    ;

globalidentifierslist: globalidentifierslist ',' globalidentifier
    | globalidentifier
    ;

globalidentifier: TK_IDENTIFICADOR
    | TK_IDENTIFICADOR '[' TK_LIT_INT ']'
    ;

funcdec: funcheader commandblock
    ;

funcheader: type TK_IDENTIFICADOR '(' ')'
    | TK_PR_STATIC type TK_IDENTIFICADOR '(' ')'
    | type TK_IDENTIFICADOR '(' parameterslist ')'
    | TK_PR_STATIC type TK_IDENTIFICADOR '(' parameterslist ')'
    ;

parameterslist: parameterslist ',' parameter
    | parameter
    ;

parameter: type TK_IDENTIFICADOR
    | TK_PR_CONST type TK_IDENTIFICADOR
    ;

commandblock: '{' '}'
    | '{' commandssequence '}'
    ;

commandssequence: commandssequence command ';'
    | command ';'
    ;

command: localvardec
    | varassignment
    | input
    | output
    | shiftcommand
    | controlflowcommand
    | return
    | TK_PR_BREAK
    | TK_PR_CONTINUE
    | commandblock
    | funccall
    ;

localvardec: type localidentifierslist
    | TK_PR_STATIC type localidentifierslist
    | TK_PR_STATIC TK_PR_CONST type localidentifierslist
    | TK_PR_CONST type localidentifierslist
    ;

localidentifierslist: localidentifierslist ',' localidentifier
    | localidentifier
    ;

localidentifier: TK_IDENTIFICADOR
    | TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR
    | TK_IDENTIFICADOR TK_OC_LE literal
    ;

literal: TK_LIT_CHAR
    | TK_LIT_STRING
    | literalboolean
    | literalnumber
    ;

literalnumber: TK_LIT_FLOAT
    | TK_LIT_INT
    ;

literalboolean: TK_LIT_FALSE
    | TK_LIT_TRUE
    ;

varassignment: varname '=' expression
    ;

input: TK_PR_INPUT TK_IDENTIFICADOR
    ;

output: TK_PR_OUTPUT TK_IDENTIFICADOR
    | TK_PR_OUTPUT literal
    ;

funccall: TK_IDENTIFICADOR '(' ')'
    | TK_IDENTIFICADOR '(' argslist ')'
    ;

argslist: argslist ',' expression
    | expression 
    ;

shiftcommand: varname shift TK_LIT_INT
    ;

shift: TK_OC_SL
    | TK_OC_SR
    ;

return: TK_PR_RETURN expression
    ;

controlflowcommand: if
    | TK_PR_FOR '(' varassignment ':' expression ':' varassignment ')' commandblock
    | TK_PR_WHILE '(' expression ')' TK_PR_DO commandblock
    ;

if: TK_PR_IF '(' expression ')' commandblock
    | TK_PR_IF '(' expression ')' commandblock TK_PR_ELSE commandblock
    ;

expression: arithmeticexpression
    | logicexpression
    | thernaryoperator
    ;

arithmeticexpression: '+' arithmeticexpression
    | '-' arithmeticexpression
    | '*' arithmeticexpression
    | '&' arithmeticexpression
    | '#' arithmeticexpression
    | arithmeticexpression '+' arithmeticexpression
    | arithmeticexpression '-' arithmeticexpression
    | arithmeticexpression '*' arithmeticexpression
    | arithmeticexpression '/' arithmeticexpression
    | arithmeticexpression '%' arithmeticexpression
    | arithmeticexpression '|' arithmeticexpression
    | arithmeticexpression '&' arithmeticexpression
    | arithmeticexpression '^' arithmeticexpression
    | TK_LIT_INT
    | TK_LIT_FLOAT
    | varname
    | funccall
    | '(' arithmeticexpression ')'
    ;

logicexpression: '!' logicexpression
    | '!' varname
    | '?' logicexpression
    | '?' varname
    | arithmeticexpression TK_OC_EQ arithmeticexpression
    | arithmeticexpression TK_OC_NE arithmeticexpression
    | arithmeticexpression TK_OC_GE arithmeticexpression
    | arithmeticexpression TK_OC_LE arithmeticexpression
    | arithmeticexpression '>' arithmeticexpression
    | arithmeticexpression '<' arithmeticexpression
    | logicexpression TK_OC_AND logicexpression
    | varname TK_OC_AND logicexpression
    | logicexpression TK_OC_AND varname
    | varname TK_OC_AND varname
    | logicexpression TK_OC_OR logicexpression
    | varname TK_OC_OR logicexpression
    | logicexpression TK_OC_OR varname
    | varname TK_OC_OR varname
    | TK_LIT_TRUE
    | TK_LIT_FALSE
    ;

thernaryoperator: expression '?' expression ':' expression
    ;

varname: TK_IDENTIFICADOR
    | TK_IDENTIFICADOR '[' expression ']'
    ;

%%

int yyerror(const char *s)
{
    fprintf(stderr,"Error: %s in line %d\n", s, num_lines);

    return 1;
}