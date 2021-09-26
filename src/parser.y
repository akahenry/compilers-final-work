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
%left '&'
%left TK_OC_EQ TK_OC_NE
%left '<' '>' TK_OC_GE TK_OC_LE
%left '-' '+'
%left '*' '/' '%'
%right '#' '!'

%type <node> initial
%type <node> program
// %type <node> globalvardec
// %type <node> type
// %type <node> globalidentifierslist
// %type <node> globalidentifier
%type <node> funcdec
%type <valor_lexico> funcheader // REVISAR essa definição de tipo
// %type <node> parameterslist
// %type <node> parameter
%type <node> commandblock
%type <node> commandssequence
%type <node> command
// %type <node> localvardec
// %type <node> localidentifierslist
// %type <node> localidentifier
%type <node> literal
%type <node> literalnumber
%type <node> literalboolean
// %type <node> varassignment
// %type <node> input
// %type <node> output
// %type <node> funccall
// %type <node> argslist
%type <node> shiftcommand
// %type <node> return
// %type <node> controlflowcommand
// %type <node> if
%type <node> expression
%type <node> arithmeticexpression
// %type <node> logicexpression
// %type <node> thernaryoperator
%type <node> varname

%%

// REVISAR: nodos que deveriam ter um filho mas não tem (e.g. função sem comandos),
// botei pra não printar esse "filho" que na verdade é NULL 

// Produções marcadas com * não precisam de regras semanticas por não terem nada a adicionar na arvore

initial: program
{
    $$ = $1;
    arvore = (void*)$$;
};

program: %empty             { $$ = NULL; }
    | globalvardec program  { $$ = $2; }
    | funcdec program       { $$ = link_nodes($1, $2); }
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

// REVISAR - declara com dois filhos, um no começo é nulo pra ser preenchido depois (função seguinte na lista de funções)
funcdec: funcheader commandblock { $$ = create_node(NULL, $1->text, $2, NULL, NULL, NULL); }
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

commandssequence: command ';' commandssequence  { $$ = link_nodes($1, $3); }
    | command ';'                               { $$ = $1; }
    ;

command: localvardec        { $$ = NULL; } // TODO
    | varassignment         { $$ = NULL; } // TODO
    | input                 { $$ = NULL; } // TODO
    | output                { $$ = NULL; } // TODO
    | shiftcommand          { $$ = $1; }
    | controlflowcommand    { $$ = NULL; } // TODO
    | return                { $$ = NULL; } // TODO
    | TK_PR_BREAK           { $$ = NULL; } // TODO
    | TK_PR_CONTINUE        { $$ = NULL; } // TODO
    | commandblock          { $$ = NULL; } // TODO
    | funccall              { $$ = NULL; } // TODO
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

shiftcommand: varname TK_OC_SL TK_LIT_INT { $$ = create_node(NULL, $2->text, $1, create_leaf($3), NULL, NULL); }
    | varname TK_OC_SR TK_LIT_INT { $$ = create_node(NULL, $2->text, $1, create_leaf($3), NULL, NULL); }
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

expression: arithmeticexpression    { $$ = $1; } // TODO
    | logicexpression               { $$ = NULL; } // TODO
    | thernaryoperator              { $$ = NULL; } // TODO
    ;

arithmeticexpression: '+' arithmeticexpression      { $$ = create_node(NULL, "+", $2, NULL, NULL, NULL); }
    | '-' arithmeticexpression                      { $$ = create_node(NULL, "-", $2, NULL, NULL, NULL); }
    | '*' arithmeticexpression                      { $$ = create_node(NULL, "*", $2, NULL, NULL, NULL); }
    | '&' arithmeticexpression                      { $$ = create_node(NULL, "&", $2, NULL, NULL, NULL); }
    | '#' arithmeticexpression                      { $$ = create_node(NULL, "#", $2, NULL, NULL, NULL); }
    | arithmeticexpression '+' arithmeticexpression { $$ = create_node(NULL, "+", $1, $3, NULL, NULL); }
    | arithmeticexpression '-' arithmeticexpression { $$ = create_node(NULL, "-", $1, $3, NULL, NULL); }
    | arithmeticexpression '*' arithmeticexpression { $$ = create_node(NULL, "*", $1, $3, NULL, NULL); }
    | arithmeticexpression '/' arithmeticexpression { $$ = create_node(NULL, "/", $1, $3, NULL, NULL); }
    | arithmeticexpression '%' arithmeticexpression { $$ = create_node(NULL, "%", $1, $3, NULL, NULL); }
    | arithmeticexpression '|' arithmeticexpression { $$ = create_node(NULL, "|", $1, $3, NULL, NULL); }
    | arithmeticexpression '&' arithmeticexpression { $$ = create_node(NULL, "&", $1, $3, NULL, NULL); }
    | arithmeticexpression '^' arithmeticexpression { $$ = create_node(NULL, "^", $1, $3, NULL, NULL); }
    | TK_LIT_INT                                    { $$ = create_leaf($1); }
    | TK_LIT_FLOAT                                  { $$ = create_leaf($1); }
    | varname                                       { $$ = $1; }
    | funccall                                      { $$ = NULL; } // TODO
    | '(' arithmeticexpression ')'                  { $$ = $2; }
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

varname: TK_IDENTIFICADOR                   { $$ = create_leaf($1); }
    | TK_IDENTIFICADOR '[' expression ']'   { $$ = create_node(NULL, "[]", create_leaf($1), $3, NULL, NULL); }
    ;

%%

int yyerror(const char *s)
{
    fprintf(stderr,"Error: %s in line %d\n", s, num_lines);

    return 1;
}