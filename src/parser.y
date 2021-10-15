/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

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
%token <valor_lexico> TK_OC_LE         // <=
%token <valor_lexico> TK_OC_GE         // >=
%token <valor_lexico> TK_OC_EQ         // ==
%token <valor_lexico> TK_OC_NE         // !=
%token <valor_lexico> TK_OC_AND        // &&
%token <valor_lexico> TK_OC_OR         // ||
%token <valor_lexico> TK_OC_SL         // >>
%token <valor_lexico> TK_OC_SR         // <<
%token <valor_lexico> TK_LIT_INT       // 3
%token <valor_lexico> TK_LIT_FLOAT     // 3.7
%token <valor_lexico> TK_LIT_FALSE     // false
%token <valor_lexico> TK_LIT_TRUE      // true
%token <valor_lexico> TK_LIT_CHAR      // 'a'
%token <valor_lexico> TK_LIT_STRING    // "str"
%token <valor_lexico> TK_IDENTIFICADOR // x0
%token TOKEN_ERRO

%left '?' ':'
%left TK_OC_OR
%left TK_OC_AND
%left '|'
%left '^'
%left TK_OC_EQ TK_OC_NE
%left '<' '>' TK_OC_GE TK_OC_LE
%left '-' '+'
%left '*' '/' '%'
%left '&'
%right '#' '!'

%type <valor_lexico> funcheader 

%type <node> initial
%type <node> program
%type <node> funcdec
%type <node> commandblock
%type <node> commandssequence
%type <node> command
%type <node> localvardec
%type <node> localidentifierslist
%type <node> localidentifier
%type <node> localidentifierdeclaration
%type <node> literal
%type <node> literalnumber
%type <node> literalboolean
%type <node> varassignment
%type <node> input
%type <node> output
%type <node> funccall
%type <node> argslist
%type <node> shiftcommand
%type <node> return
%type <node> controlflowcommand
%type <node> if
%type <node> expression
%type <node> arithmeticexpression
%type <node> logicexpression
%type <node> thernaryoperator
%type <node> varname

%%

initial: program
{
    $$ = $1;
    arvore = (void*)$$;
};

program: %empty             { $$ = NULL; }
    | globalvardec program  { $$ = $2; }
    | funcdec program       { if ($1 != NULL) { $$ = link_nodes($1, $2); } else { $$ = $2; }; }
    ;

// *
globalvardec: type globalidentifierslist ';'
    | TK_PR_STATIC type globalidentifierslist ';'
    ;

// *
type: TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL
    | TK_PR_CHAR
    | TK_PR_STRING
    ;

// *
globalidentifierslist: globalidentifierslist ',' globalidentifier
    | globalidentifier
    ;

// *
globalidentifier: TK_IDENTIFICADOR
    | TK_IDENTIFICADOR '[' TK_LIT_INT ']'
    ;

funcdec: funcheader commandblock { $$ = create_node($1->text, $2, NULL, NULL, NULL, NULL); }
    ;

funcheader: type TK_IDENTIFICADOR '(' ')'                       { $$ = $2; }
    | TK_PR_STATIC type TK_IDENTIFICADOR '(' ')'                { $$ = $3; }
    | type TK_IDENTIFICADOR '(' parameterslist ')'              { $$ = $2; }
    | TK_PR_STATIC type TK_IDENTIFICADOR '(' parameterslist ')' { $$ = $3; }
    ;

// *
parameterslist: parameterslist ',' parameter
    | parameter
    ;

// *
parameter: type TK_IDENTIFICADOR
    | TK_PR_CONST type TK_IDENTIFICADOR
    ;

commandblock: '{' '}'           { $$ = NULL; }
    | '{' commandssequence '}'  { $$ = $2; }
    ;

commandssequence: command ';' commandssequence { if ($1 != NULL) { $$ = link_nodes($1, $3); } else { $$ = $3; }; }
    | command ';'                               { $$ = $1; }
    ;

command: localvardec        { $$ = $1; }
    | varassignment         { $$ = $1; }
    | input                 { $$ = $1; }
    | output                { $$ = $1; }
    | shiftcommand          { $$ = $1; }
    | controlflowcommand    { $$ = $1; }
    | return                { $$ = $1; }
    | TK_PR_BREAK           { $$ = create_node("break", NULL, NULL, NULL, NULL, NULL); }
    | TK_PR_CONTINUE        { $$ = create_node("continue", NULL, NULL, NULL, NULL, NULL); }
    | commandblock          { $$ = $1; }
    | funccall              { $$ = $1; }
    ;

localvardec: type localidentifierslist { $$ = $2; }
    | TK_PR_STATIC type localidentifierslist { $$ = $3; }
    | TK_PR_STATIC TK_PR_CONST type localidentifierslist { $$ = $4; }
    | TK_PR_CONST type localidentifierslist { $$ = $3; }
    ;

localidentifierslist: localidentifier ',' localidentifierslist { if ($1 != NULL) { $$ = link_nodes($1, $3); } else { $$ = $3; }; }
    | localidentifier { $$ = $1; }
    | localidentifierdeclaration { $$ = $1; }
    | localidentifierdeclaration ',' localidentifierslist { $$ = $3; }
    ;

localidentifier: TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR { $$ = create_node($2->text, create_leaf($1), create_leaf($3), NULL, NULL, NULL); }
    | TK_IDENTIFICADOR TK_OC_LE literal { $$ = create_node($2->text, create_leaf($1), $3, NULL, NULL, NULL); }
    ;

localidentifierdeclaration: TK_IDENTIFICADOR { $$ = NULL; }
    ;

literal: TK_LIT_CHAR { $$ = create_leaf($1); }
    | TK_LIT_STRING { $$ = create_leaf($1); }
    | literalboolean { $$ = $1; }
    | literalnumber { $$ = $1; }
    ;

literalnumber: TK_LIT_FLOAT { $$ = create_leaf($1); }
    | TK_LIT_INT { $$ = create_leaf($1); }
    ;

literalboolean: TK_LIT_FALSE { $$ = create_leaf($1); }
    | TK_LIT_TRUE { $$ = create_leaf($1); }
    ;

varassignment: varname '=' expression { $$ = create_node("=", $1, $3, NULL, NULL, NULL); }
    ;

input: TK_PR_INPUT TK_IDENTIFICADOR { $$ = create_node("input", create_leaf($2), NULL, NULL, NULL, NULL); }
    ;

output: TK_PR_OUTPUT TK_IDENTIFICADOR { $$ = create_node("output", create_leaf($2), NULL, NULL, NULL, NULL); }
    | TK_PR_OUTPUT literal { $$ = create_node("output", $2, NULL, NULL, NULL, NULL); }
    ;

funccall: TK_IDENTIFICADOR '(' ')' 
{ 
    char* label = calloc(strlen($1->text) + strlen("call ") + 1, sizeof(char));
    sprintf(label, "call %s", $1->text);
    $$ = create_node(label, NULL, NULL, NULL, NULL, NULL);
    free(label);
}
    | TK_IDENTIFICADOR '(' argslist ')' 
{ 
    char* label = calloc(strlen($1->text) + strlen("call ") + 1, sizeof(char));
    sprintf(label, "call %s", $1->text);
    $$ = create_node(label, $3, NULL, NULL, NULL, NULL); 
    free(label);
}
    ;

argslist: expression ',' argslist {  if ($1 != NULL) { $$ = link_nodes($1, $3); } else { $$ = $3; };  }
    | expression { $$ = $1; }
    ;

shiftcommand: varname TK_OC_SL TK_LIT_INT { $$ = create_node($2->text, $1, create_leaf($3), NULL, NULL, NULL); }
    | varname TK_OC_SR TK_LIT_INT { $$ = create_node($2->text, $1, create_leaf($3), NULL, NULL, NULL); }
    ;

return: TK_PR_RETURN expression { $$ = create_node("return", $2, NULL, NULL, NULL, NULL); }
    ;

controlflowcommand: if { $$ = $1; }
    | TK_PR_FOR '(' varassignment ':' expression ':' varassignment ')' commandblock { $$ = create_node("for", $3, $5, $7, $9, NULL); }
    | TK_PR_WHILE '(' expression ')' TK_PR_DO commandblock { $$ = create_node("while", $3, $6, NULL, NULL, NULL); }
    ;

if: TK_PR_IF '(' expression ')' commandblock { $$ = create_node("if", $3, $5, NULL, NULL, NULL); }
    | TK_PR_IF '(' expression ')' commandblock TK_PR_ELSE commandblock { $$ = create_node("if", $3, $5, $7, NULL, NULL); }
    ;

expression: arithmeticexpression    { $$ = $1; }
    | logicexpression               { $$ = $1; }
    | thernaryoperator              { $$ = $1; }
    ;

arithmeticexpression: '+' arithmeticexpression      { $$ = create_node("+", $2, NULL, NULL, NULL, NULL); }
    | '-' arithmeticexpression                      { $$ = create_node("-", $2, NULL, NULL, NULL, NULL); }
    | '*' arithmeticexpression                      { $$ = create_node("*", $2, NULL, NULL, NULL, NULL); }
    | '&' arithmeticexpression                      { $$ = create_node("&", $2, NULL, NULL, NULL, NULL); }
    | '#' arithmeticexpression                      { $$ = create_node("#", $2, NULL, NULL, NULL, NULL); }
    | arithmeticexpression '+' arithmeticexpression { $$ = create_node("+", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '-' arithmeticexpression { $$ = create_node("-", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '*' arithmeticexpression { $$ = create_node("*", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '/' arithmeticexpression { $$ = create_node("/", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '%' arithmeticexpression { $$ = create_node("%", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '|' arithmeticexpression { $$ = create_node("|", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '&' arithmeticexpression { $$ = create_node("&", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '^' arithmeticexpression { $$ = create_node("^", $1, $3, NULL, NULL, NULL); }
    | TK_LIT_INT                                    { $$ = create_leaf($1); }
    | TK_LIT_FLOAT                                  { $$ = create_leaf($1); }
    | varname                                       { $$ = $1; }
    | funccall                                      { $$ = $1; }
    | '(' arithmeticexpression ')'                  { $$ = $2; }
    ;

logicexpression: '!' logicexpression { $$ = create_node("!", $2, NULL, NULL, NULL, NULL); }
    | '!' varname { $$ = create_node("!", $2, NULL, NULL, NULL, NULL); }
    | '?' logicexpression { $$ = create_node("?", $2, NULL, NULL, NULL, NULL); }
    | '?' varname { $$ = create_node("?", $2, NULL, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_EQ arithmeticexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_NE arithmeticexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_GE arithmeticexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_LE arithmeticexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '>' arithmeticexpression { $$ = create_node(">", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '<' arithmeticexpression { $$ = create_node("<", $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_AND logicexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_AND logicexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_AND varname { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_AND varname { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_OR logicexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_OR logicexpression { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_OR varname { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_OR varname { $$ = create_node($2->text, $1, $3, NULL, NULL, NULL); }
    | TK_LIT_TRUE { $$ = create_leaf($1); }
    | TK_LIT_FALSE { $$ = create_leaf($1); }
    ;

thernaryoperator: expression '?' expression ':' expression { $$ = create_node("?:", $1, $3, $5, NULL, NULL); }
    ;

varname: TK_IDENTIFICADOR                   { $$ = create_leaf($1); }
    | TK_IDENTIFICADOR '[' expression ']'   { $$ = create_node("[]", create_leaf($1), $3, NULL, NULL, NULL); }
    ;

%%

int yyerror(const char *s)
{
    fprintf(stderr,"Error: %s in line %d\n", s, num_lines);

    return 1;
}