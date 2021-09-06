%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror (char const *s);
%}

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

%left '|' '^'
%left '<' '>' TK_OC_EQ TK_OC_NE TK_OC_GE TK_OC_LE
%left '+' '-'
%left '*' '/'
%right '&' '#'


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

globalidentifierslist: globalidentifier
    | globalidentifier ',' globalidentifierslist
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

parameterslist: parameter
    | parameter ',' parameterslist
    ;

parameter: type TK_IDENTIFICADOR
    ;

commandblock: '{' '}'
    | '{' commandssequence '}'
    ;

commandssequence: command ';'
    | command ';' commandssequence
    | controlflowcommand
    | controlflowcommand commandssequence
    ;

command: localvardec
    | varassignment
    | input
    | output
    | shiftcommand
    | return
    | TK_PR_BREAK
    | TK_PR_CONTINUE
    | commandblock
    | funccall
    ;

localvardec: type localidentifierslist
    | TK_PR_STATIC type localidentifierslist
    | TK_PR_STATIC TK_PR_CONST localidentifierslist
    ;

localidentifierslist: localidentifier
    | localidentifier ',' localidentifierslist
    ;

localidentifier: TK_IDENTIFICADOR
    | TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR
    | TK_IDENTIFICADOR TK_OC_LE literal
    ;

literal: TK_LIT_CHAR
    | TK_LIT_FALSE
    | TK_LIT_TRUE
    | TK_LIT_STRING
    | literalnumber
    ;

literalnumber: TK_LIT_FLOAT
    | TK_LIT_INT
    ;

varassignment: TK_IDENTIFICADOR '=' expression
    | TK_IDENTIFICADOR '[' expression ']' '=' expression
    ;

input: TK_PR_INPUT TK_IDENTIFICADOR
    ;

output: TK_PR_OUTPUT TK_IDENTIFICADOR
    | TK_PR_OUTPUT literal
    ;

funccall: TK_IDENTIFICADOR '(' ')'
    | TK_IDENTIFICADOR '(' argslist ')'
    ;

argslist: arg
    | arg ',' argslist
    ;

arg: expression
    ;

shiftcommand: TK_IDENTIFICADOR shift TK_LIT_INT
    | TK_IDENTIFICADOR '[' expression ']' shift TK_LIT_INT
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
    | literal
    ;

arithmeticexpression: arithmeticunaryoperator arithmeticoperand
    | arithmeticoperand arithmeticbinaryoperator arithmeticoperand
    /* | expression '?' expression ':' expression */ // TODO: ternary operator without conflicts 
    ;

arithmeticunaryoperator: '+'
    | '-'
    | '&'
    | '*'
    | '#'
    ;

arithmeticbinaryoperator: '+'
    | '-'
    | '*'
    | '/'
    | '%'
    | '|'
    | '&'
    | '^'
    ;

operand: TK_IDENTIFICADOR
    | TK_IDENTIFICADOR '[' expression ']'
    | literalnumber
    | funccall
    ;

arithmeticoperand: operand
    | '(' arithmeticexpression ')'
    ;


logicexpression: logicunaryoperator logicoperand
    | logicoperand logicbinaryoperator logicoperand
    ;

logicoperand: operand
    | arithmeticexpression
    /* | logicexpression  */ // TODO: logic expression recursion without conflict
    ;

logicunaryoperator: '!'
    | '?'
    ;

logicbinaryoperator: TK_OC_AND
    | TK_OC_OR
    | TK_OC_EQ
    | TK_OC_NE
    | TK_OC_GE
    | TK_OC_LE
    | '>'
    | '<'
    ;

%%

int yyerror(char const *s) {
    extern char *yytext;
    extern int num_lines;

    fprintf(stderr, "Syntax error [line:%d]\n", num_lines);
    return 1;
}