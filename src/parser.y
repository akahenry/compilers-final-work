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

symbol_datatype_t current_type;
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

// TODO de analise sintatica:
// [x] criar e destruir escopo
// [ ] adicionar simbolos na tabela de simbolos
//      localização
//      natureza
//      tipo
//      tamanho
//      argumentos e tipos
//      valor do token (yylval)
// declaracoes:
//      [ ] erro na dupla declaração no mesmo escopo                            ERR_DECLARED
//      [ ] erro no uso sem declaração em escopos superiores                    ERR_UNDECLARED
//      [ ] erro na declaração de vetor de string                               ERR_STRING_VECTOR
// uso de identificadores:
//      [ ] erro vetor sendo usado como função ou variavel                      ERR_VECTOR
//      (deve ser usado com indexação)
//      [ ] erro função sendo usada como vetor ou variavel                      ERR_FUNCTION
//      (deve ser usada com parentesis e args)
//      [ ] erro variavel sendo usada como vetor ou função                      ERR_VARIABLE
//      (deve ser usado sem nada)
// [ ] tipo do token é herdado pelo nó (conversão implicita e inferencia)
//      erro na conversão implicita (coerção) de string e char:
//          [ ] char para algo                                                  ERR_CHAR_TO_X
//          [ ] string para algo                                                ERR_STRING_TO_X
// argumentos compativeis com declaração de função:
//      [ ] erro no uso com menos argumentos                                    ERR_MISSING_ARGS
//      [ ] erro no uso com mais argumentos                                     ERR_EXCESS_ARGS
//      [ ] erro no uso com argumentos de tipos errados                         ERR_WRONG_TYPE_ARGS
// [ ] erro quando argumentos, retorno e parametros de funções são string       ERR_FUNCTION_STRING
// [ ] erro na atribuição de um valor de um tipo para variavel de outro         ERR_WRONG_TYPE
// input e output só aceitam int e float:
//      [ ] erro se tipo no input não é int ou float                            ERR_WRONG_PAR_INPUT
//      [ ] erro se tipo no output não é int ou float                           ERR_WRONG_PAR_OUTPUT
// [ ] erro se o numero de shift for maior que 16                               ERR_WRONG_PAR_SHIFT
// [ ] erro se a atribuição de string for maior que o tamanho max declarado     ERR_STRING_MAX

// conversões implícitas
//      int -> float | bool
//      bool -> float | int
//      float -> int | bool
// inferência
//      dois (float | int | bool) -> mesmo tipo dos dois
//      float e int -> float
//      bool e int -> int
//      bool e float -> float

// tamanhos
//      char    1 byte
//      bool    1 byte
//      int     4 bytes
//      float   8 bytes
//      string  tamanho definido na declaração (em bytes)
//      vetor   tamanho do vetor na declaração * tamanho do tipo

// mensagens de erro em linguagem natural:
//      linha
//      identificador
//      natureza

initial: program
{
    $$ = $1;
    arvore = (void*)$$;
    printf("PROGRAMA: \n");
    print_symbol_table();
};

program: %empty             { $$ = NULL; }
    | globalvardec program  { $$ = $2; }
    | funcdec program       { if ($1 != NULL) { $$ = node_link($1, $2); } else { $$ = $2; }; }
    ;

globalvardec: type globalidentifierslist ';'
    | TK_PR_STATIC type globalidentifierslist ';'
    ;

type: TK_PR_INT     { current_type = SYMBOL_DATATYPE_INT; }
    | TK_PR_FLOAT   { current_type = SYMBOL_DATATYPE_FLOAT; }
    | TK_PR_BOOL    { current_type = SYMBOL_DATATYPE_BOOL; }
    | TK_PR_CHAR    { current_type = SYMBOL_DATATYPE_CHAR; }
    | TK_PR_STRING  { current_type = SYMBOL_DATATYPE_STRING; }
    ;

globalidentifierslist: globalidentifierslist ',' globalidentifier
    | globalidentifier
    ;

globalidentifier: TK_IDENTIFICADOR
    {
        printf("Identificador %s, Tipo: %d\n", $1->text, (int)current_type);
        add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type);
    }
    | TK_IDENTIFICADOR '[' TK_LIT_INT ']'
    ;

funcdec: funcheader commandblock { $$ = node_create($1->text, $2, NULL, NULL, NULL, NULL); }
    ;

funcheader: type TK_IDENTIFICADOR '(' ')'                       { $$ = $2; }
    | TK_PR_STATIC type TK_IDENTIFICADOR '(' ')'                { $$ = $3; }
    | type TK_IDENTIFICADOR '(' parameterslist ')'              { $$ = $2; }
    | TK_PR_STATIC type TK_IDENTIFICADOR '(' parameterslist ')' { $$ = $3; }
    ;

parameterslist: parameterslist ',' parameter
    | parameter
    ;

parameter: type TK_IDENTIFICADOR
    | TK_PR_CONST type TK_IDENTIFICADOR
    ;

commandblock: openscope closescope           { $$ = NULL; }
    | openscope commandssequence closescope
        {
            $$ = $2;
        }
    ;

openscope: '{' { open_scope(); }
    ;

closescope: '}' { printf("\nBLOCO DE COMANDO: \n"); print_symbol_table(); close_scope(); }
    ;

commandssequence: command ';' commandssequence { if ($1 != NULL) { $$ = node_link($1, $3); } else { $$ = $3; }; }
    | command ';' { $$ = $1; }
    ;

command: localvardec        { $$ = $1; }
    | varassignment         { $$ = $1; }
    | input                 { $$ = $1; }
    | output                { $$ = $1; }
    | shiftcommand          { $$ = $1; }
    | controlflowcommand    { $$ = $1; }
    | return                { $$ = $1; }
    | TK_PR_BREAK           { $$ = node_create("break", NULL, NULL, NULL, NULL, NULL); }
    | TK_PR_CONTINUE        { $$ = node_create("continue", NULL, NULL, NULL, NULL, NULL); }
    | commandblock          { $$ = $1; }
    | funccall              { $$ = $1; }
    ;

localvardec: type localidentifierslist { $$ = $2; }
    | TK_PR_STATIC type localidentifierslist { $$ = $3; }
    | TK_PR_STATIC TK_PR_CONST type localidentifierslist { $$ = $4; }
    | TK_PR_CONST type localidentifierslist { $$ = $3; }
    ;

localidentifierslist: localidentifier ',' localidentifierslist { if ($1 != NULL) { $$ = node_link($1, $3); } else { $$ = $3; }; }
    | localidentifier { $$ = $1; }
    | localidentifierdeclaration { $$ = $1; }
    | localidentifierdeclaration ',' localidentifierslist { $$ = $3; }
    ;

localidentifier: TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR { $$ = node_create($2->text, node_create_leaf($1), node_create_leaf($3), NULL, NULL, NULL); }
    | TK_IDENTIFICADOR TK_OC_LE literal { $$ = node_create($2->text, node_create_leaf($1), $3, NULL, NULL, NULL); }
    ;

localidentifierdeclaration: TK_IDENTIFICADOR { $$ = NULL; }
    ;

literal: TK_LIT_CHAR { $$ = node_create_leaf($1); }
    | TK_LIT_STRING { $$ = node_create_leaf($1); }
    | literalboolean { $$ = $1; }
    | literalnumber { $$ = $1; }
    ;

literalnumber: TK_LIT_FLOAT { $$ = node_create_leaf($1); }
    | TK_LIT_INT { $$ = node_create_leaf($1); }
    ;

literalboolean: TK_LIT_FALSE { $$ = node_create_leaf($1); }
    | TK_LIT_TRUE { $$ = node_create_leaf($1); }
    ;

varassignment: varname '=' expression { $$ = node_create("=", $1, $3, NULL, NULL, NULL); }
    ;

input: TK_PR_INPUT TK_IDENTIFICADOR { $$ = node_create("input", node_create_leaf($2), NULL, NULL, NULL, NULL); }
    ;

output: TK_PR_OUTPUT TK_IDENTIFICADOR { $$ = node_create("output", node_create_leaf($2), NULL, NULL, NULL, NULL); }
    | TK_PR_OUTPUT literal { $$ = node_create("output", $2, NULL, NULL, NULL, NULL); }
    ;

funccall: TK_IDENTIFICADOR '(' ')' 
{ 
    char* label = calloc(strlen($1->text) + strlen("call ") + 1, sizeof(char));
    sprintf(label, "call %s", $1->text);
    $$ = node_create(label, NULL, NULL, NULL, NULL, NULL);
    free(label);
}
    | TK_IDENTIFICADOR '(' argslist ')' 
{ 
    char* label = calloc(strlen($1->text) + strlen("call ") + 1, sizeof(char));
    sprintf(label, "call %s", $1->text);
    $$ = node_create(label, $3, NULL, NULL, NULL, NULL); 
    free(label);
}
    ;

argslist: expression ',' argslist {  if ($1 != NULL) { $$ = node_link($1, $3); } else { $$ = $3; };  }
    | expression { $$ = $1; }
    ;

shiftcommand: varname TK_OC_SL TK_LIT_INT { $$ = node_create($2->text, $1, node_create_leaf($3), NULL, NULL, NULL); }
    | varname TK_OC_SR TK_LIT_INT { $$ = node_create($2->text, $1, node_create_leaf($3), NULL, NULL, NULL); }
    ;

return: TK_PR_RETURN expression { $$ = node_create("return", $2, NULL, NULL, NULL, NULL); }
    ;

controlflowcommand: if { $$ = $1; }
    | TK_PR_FOR '(' varassignment ':' expression ':' varassignment ')' commandblock { $$ = node_create("for", $3, $5, $7, $9, NULL); }
    | TK_PR_WHILE '(' expression ')' TK_PR_DO commandblock { $$ = node_create("while", $3, $6, NULL, NULL, NULL); }
    ;

if: TK_PR_IF '(' expression ')' commandblock { $$ = node_create("if", $3, $5, NULL, NULL, NULL); }
    | TK_PR_IF '(' expression ')' commandblock TK_PR_ELSE commandblock { $$ = node_create("if", $3, $5, $7, NULL, NULL); }
    ;

expression: arithmeticexpression    { $$ = $1; }
    | logicexpression               { $$ = $1; }
    | thernaryoperator              { $$ = $1; }
    ;

arithmeticexpression: '+' arithmeticexpression      { $$ = node_create("+", $2, NULL, NULL, NULL, NULL); }
    | '-' arithmeticexpression                      { $$ = node_create("-", $2, NULL, NULL, NULL, NULL); }
    | '*' arithmeticexpression                      { $$ = node_create("*", $2, NULL, NULL, NULL, NULL); }
    | '&' arithmeticexpression                      { $$ = node_create("&", $2, NULL, NULL, NULL, NULL); }
    | '#' arithmeticexpression                      { $$ = node_create("#", $2, NULL, NULL, NULL, NULL); }
    | arithmeticexpression '+' arithmeticexpression { $$ = node_create("+", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '-' arithmeticexpression { $$ = node_create("-", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '*' arithmeticexpression { $$ = node_create("*", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '/' arithmeticexpression { $$ = node_create("/", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '%' arithmeticexpression { $$ = node_create("%", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '|' arithmeticexpression { $$ = node_create("|", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '&' arithmeticexpression { $$ = node_create("&", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '^' arithmeticexpression { $$ = node_create("^", $1, $3, NULL, NULL, NULL); }
    | TK_LIT_INT                                    { $$ = node_create_leaf($1); }
    | TK_LIT_FLOAT                                  { $$ = node_create_leaf($1); }
    | varname                                       { $$ = $1; }
    | funccall                                      { $$ = $1; }
    | '(' arithmeticexpression ')'                  { $$ = $2; }
    ;

logicexpression: '!' logicexpression { $$ = node_create("!", $2, NULL, NULL, NULL, NULL); }
    | '!' varname { $$ = node_create("!", $2, NULL, NULL, NULL, NULL); }
    | '?' logicexpression { $$ = node_create("?", $2, NULL, NULL, NULL, NULL); }
    | '?' varname { $$ = node_create("?", $2, NULL, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_EQ arithmeticexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_NE arithmeticexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_GE arithmeticexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression TK_OC_LE arithmeticexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '>' arithmeticexpression { $$ = node_create(">", $1, $3, NULL, NULL, NULL); }
    | arithmeticexpression '<' arithmeticexpression { $$ = node_create("<", $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_AND logicexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_AND logicexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_AND varname { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_AND varname { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_OR logicexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_OR logicexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | logicexpression TK_OC_OR varname { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_OR varname { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | TK_LIT_TRUE { $$ = node_create_leaf($1); }
    | TK_LIT_FALSE { $$ = node_create_leaf($1); }
    ;

thernaryoperator: expression '?' expression ':' expression { $$ = node_create("?:", $1, $3, $5, NULL, NULL); }
    ;

varname: TK_IDENTIFICADOR                   { $$ = node_create_leaf($1); }
    | TK_IDENTIFICADOR '[' expression ']'   { $$ = node_create("[]", node_create_leaf($1), $3, NULL, NULL, NULL); }
    ;

%%

int yyerror(const char *s)
{
    fprintf(stderr,"Error: %s in line %d\n", s, num_lines);

    return 1;
}