/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#include "intermediate_code.h"

#define YYERROR_VERBOSE 1

extern int num_lines;
extern void* arvore;

int yylex(void);
int yyerror (char const *s);

symbol_datatype_t current_type;
int opening_function = 0;
int is_argless_function = 0;
symbol_datatype_t current_function_type;
queue_t* params_types = NULL;
stack_t* args_types = NULL;
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
%token <valor_lexico> '^'
%token <valor_lexico> '&'
%token <valor_lexico> '#'
%token <valor_lexico> '+'
%token <valor_lexico> '-'
%token <valor_lexico> '*'
%token <valor_lexico> '/'
%token <valor_lexico> '%'
%token <valor_lexico> '|'
%token <valor_lexico> ':'
%token <valor_lexico> '='
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
%type <valor_lexico> funcidentifier 

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

// TODO etapa 5

// [ ] estrutura de dados para instruções ILOC com argumentos (nome de registradores, valores constantes ou nomes de rótulos)
// [ ] lista de comandos ILOC que será o programa traduzido para ILOC

// [x] função que fornece nomes de rótulos (convenção: L e numero inteiro positivo)
// [x] função que gera nomes de registradores (convenção: r e numero inteiro positivo)

// [x] Cálculo de endereço na declaração de variáveis
// [x] Expressões aritméticas com operações unárias e binárias
// [ ] Expressões ternárias (OPCIONAL?)
// [ ] Expressões lógicas (com curto-circuito)
// Comando de atribuição
//      Atribuição na declaração local
//          [x] Variável recebe literal
//          [ ] Variável recebe variável
//      [ ] Atribuição como comando
// [ ] Comandos de fluxo de controle
//      [ ] if com else opcional
//      [ ] while
//      [ ] for
// [ ] chamada de função

initial: program
{
    $$ = $1;
    arvore = (void*)$$;
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
        add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type, NULL, ADD_DISP);
    }
    | TK_IDENTIFICADOR '[' TK_LIT_INT ']' 
    {
        add_symbol($3->text, $3, SYMBOL_TYPE_LITERAL_INT, SYMBOL_DATATYPE_INT, NULL, IGNORE_DISP);
        add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_VECTOR, current_type, $3, ADD_DISP);
    }
    ;

funcdec: funcheader commandblock
    {
        $$ = node_create($1->text, $2, NULL, NULL, NULL, NULL);
        $$->type = current_function_type;
    }
    ;

funcheader: type funcidentifier '(' ')'
    { 
        $$ = $2;
        symbol_item_t* identifier = get_symbol($2->text);
        identifier->params_queue = params_types;
        params_types = NULL;
        opening_function = 1;
        is_argless_function = 1;
    }
    | TK_PR_STATIC type funcidentifier '(' ')'
    { 
        $$ = $3;
        symbol_item_t* identifier = get_symbol($3->text);
        identifier->params_queue = params_types;
        params_types = NULL;
        opening_function = 1;
        is_argless_function = 1;
    }
    | type funcidentifier '(' parameterslist ')'
    { 
        $$ = $2;
        symbol_item_t* identifier = get_symbol($2->text);
        identifier->params_queue = params_types;
        params_types = NULL;
    }
    | TK_PR_STATIC type funcidentifier '(' parameterslist ')'
    { 
        $$ = $3;
        symbol_item_t* identifier = get_symbol($3->text);
        identifier->params_queue = params_types;
        params_types = NULL;
    }
    ;

funcidentifier: TK_IDENTIFICADOR
    {
        $$ = $1;

        if (current_type != SYMBOL_DATATYPE_STRING)
        {
            add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_FUNCTION, current_type, NULL, IGNORE_DISP);
            current_function_type = current_type;
        }
        else
        {
            fprintf(stderr, "Semantic Error: cannot return datatype `string` in functions (line %d)\n", $1->line);
            exit(ERR_FUNCTION_STRING);
        }
    }
    ;

parameterslist: parameterslist ',' parameter
    | parameter
    {
        // allocate for return value
        add_disp_symbol_table(4);
    }
    ;

parameter: type TK_IDENTIFICADOR
    {
        if (current_type != SYMBOL_DATATYPE_STRING)
        {
            if (!opening_function)
            {
                opening_function = 1;
                open_scope();
                params_types = queue_create();

                // allocate for return addr, rsp and rfp
                add_disp_symbol_table(12);
            }
            add_symbol($2->text, $2, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type, NULL, ADD_DISP);
            symbol_datatype_t* datatype = calloc(1, sizeof(symbol_datatype_t));
            *datatype = current_type;
            queue_push(params_types, (void*)datatype);
        }
        else
        {
            fprintf(stderr, "Semantic Error: cannot have parameters of type `string` in functions (line %d)\n", $2->line);
            exit(ERR_FUNCTION_STRING);
        }
    }
    | TK_PR_CONST type TK_IDENTIFICADOR
    {
        if (current_type != SYMBOL_DATATYPE_STRING)
        {
            if (!opening_function)
            {
                opening_function = 1;
                open_scope();

                // allocate for return addr, rsp and rfp
                add_disp_symbol_table(12);
            }
            add_symbol($3->text, $3, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type, NULL, ADD_DISP);
            symbol_datatype_t* datatype = calloc(1, sizeof(symbol_datatype_t));
            *datatype = current_type;
            queue_push(params_types, (void*)datatype);
        }
        else
        {
            fprintf(stderr, "Semantic Error: cannot have parameters of type `string` in functions (line %d)\n", $3->line);
            exit(ERR_FUNCTION_STRING);
        }
    }
    ;

commandblock: openscope closescope           { $$ = NULL; }
    | openscope commandssequence closescope
    {
        $$ = $2;
    }
    ;

openscope: '{' 
    {
        if (!opening_function || is_argless_function)
        {
            open_scope();
            if (is_argless_function)
            {
                // allocate for return addr, rsp, rfp and return value
                add_disp_symbol_table(16);
            }
        }
        opening_function = 0;
        is_argless_function = 0;
    }
    ;

closescope: '}' { close_scope(); }
    ;

commandssequence: command ';' commandssequence { if ($1 != NULL) { $$ = node_link($1, $3); $$->code = iloc_join($1->code, $3->code); } else { $$ = $3; }; }
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

localidentifierslist: localidentifier ',' localidentifierslist { if ($1 != NULL) { $$ = node_link($1, $3); $$->code = iloc_join($1->code, $3->code); } else { $$ = $3; }; }
    | localidentifier { $$ = $1; }
    | localidentifierdeclaration { $$ = $1; }
    | localidentifierdeclaration ',' localidentifierslist { $$ = $3; }
    ;

localidentifier: TK_IDENTIFICADOR TK_OC_LE TK_IDENTIFICADOR 
    {
        $$ = node_create($2->text, node_create_leaf($1), node_create_leaf($3), NULL, NULL, NULL);
        symbol_item_t* second = get_symbol($3->text);
        if (second != NULL)
        {
            if (second->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
            {
                if (current_type == second->datatype)
                {
                    add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type, NULL, ADD_DISP);
                    if (current_type == SYMBOL_DATATYPE_STRING)
                    {
                        symbol_item_t* first = get_symbol($1->text);
                        first->size = second->size;
                    }
                }
                else
                {
                    check_implicit_conversion(current_type, second->datatype, $2->line, 0);
                }
                $$->type = current_type;
            }
            else
            {
                if (second->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a function and not a variable (line %d)\n", second->token->text, $1->line);
                    exit(ERR_FUNCTION);
                }
                else if (second->type == SYMBOL_TYPE_IDENTIFIER_VECTOR)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a vector and not a variable (line %d)\n", second->token->text, $1->line);
                    exit(ERR_VECTOR);
                }
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined identifier `%s` in line %d\n", $3->text, $3->line);
            exit(ERR_UNDECLARED);
        }
    }
    | TK_IDENTIFICADOR TK_OC_LE literal 
    { 
        $$ = node_create($2->text, node_create_leaf($1), $3, NULL, NULL, NULL);
        symbol_item_t* second = NULL;

        if ($3->type == NODE_TYPE_CHAR)
        {
            second = get_symbol_lit_char($3->token->text);
        }
        else if ($3->type == NODE_TYPE_STRING)
        {
            second = get_symbol_lit_string($3->token->text);
        }
        else
        {
            second = get_symbol($3->token->text);
        }

        if (second != NULL)
        {
            if (current_type == second->datatype)
            {
                add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type, NULL, ADD_DISP);
                if (current_type == SYMBOL_DATATYPE_STRING)
                {
                    symbol_item_t* first = get_symbol($1->text);
                    first->size = second->size;
                }
            }
            else
            {
                check_implicit_conversion(current_type, second->datatype, $2->line, 0);
            }
            $$->type = current_type;

            iloc_argument_t rfp = {ILOC_ARG_TYPE_RFP, 0};
            iloc_argument_t address = {ILOC_ARG_TYPE_NUMBER, get_symbol($1->text)->address};
            iloc_argument_t value = {ILOC_ARG_TYPE_NUMBER, second->token->value.v_integer};
            $$->code = generate_attribution(rfp, address, value);
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined identifier `%s` in line %d\n", $3->token->text, $3->token->line);
            exit(ERR_UNDECLARED);
        }
    }
    ;

localidentifierdeclaration: TK_IDENTIFICADOR 
    { 
        $$ = NULL;
        add_symbol($1->text, $1, SYMBOL_TYPE_IDENTIFIER_VARIABLE, current_type, NULL, ADD_DISP);
    }
    ;

literal: TK_LIT_CHAR
    { 
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_CHAR;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_CHAR, SYMBOL_DATATYPE_CHAR, NULL, IGNORE_DISP);
    }
    | TK_LIT_STRING
    { 
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_STRING;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_STRING, SYMBOL_DATATYPE_STRING, NULL, IGNORE_DISP);
    }
    | literalboolean { $$ = $1; }
    | literalnumber { $$ = $1; }
    ;

literalnumber: TK_LIT_FLOAT
    { 
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_FLOAT;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_FLOAT, SYMBOL_DATATYPE_FLOAT, NULL, IGNORE_DISP);
    }
    | TK_LIT_INT
    { 
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_INT;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_INT, SYMBOL_DATATYPE_INT, NULL, IGNORE_DISP);
    }
    ;

literalboolean: TK_LIT_FALSE
    { 
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_BOOL;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_BOOL, SYMBOL_DATATYPE_BOOL, NULL, IGNORE_DISP);
    }
    | TK_LIT_TRUE
    { 
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_BOOL;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_BOOL, SYMBOL_DATATYPE_BOOL, NULL, IGNORE_DISP);
    }
    ;

varassignment: varname '=' expression
    {
        $$ = node_create("=", $1, $3, NULL, NULL, NULL);
        $$->type = $1->type;

        check_implicit_conversion($1->type, $3->type, $2->line, 0);

        symbol_item_t* first = NULL;
        symbol_item_t* second = NULL;
        if($1->token != NULL)
        {
            first = get_symbol($1->token->text);
        }
        else
        {
            // in this case, the node isn't leaf because it is a vector, so it doesn't have a token
            // so it gets the token with the varname in the first child
            first = get_symbol($1->child1->token->text);
        }

        if ($3->type == NODE_TYPE_STRING)
        {
            second = get_symbol($3->token->text);
        }
        if (first != NULL && second != NULL)
        {
            if (first->datatype == SYMBOL_DATATYPE_STRING && first->size < second->size)
            {
                fprintf(stderr, "Semantic Error: identifier `%s` has length `%ld` and cannot be assigned with lengths greater than it (line %d)\n", first->token->text, first->size, $1->token->line);
                exit(ERR_STRING_MAX);
            }
        }
    }
    ;

input: TK_PR_INPUT TK_IDENTIFICADOR
    { 
        $$ = node_create("input", node_create_leaf($2), NULL, NULL, NULL, NULL);
        symbol_item_t* identifier = get_symbol($2->text);
        if (identifier != NULL)
        {
            if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
            {
                if (identifier->datatype == SYMBOL_DATATYPE_INT || identifier->datatype == SYMBOL_DATATYPE_FLOAT)
                {
                    $$->type = (node_type_t)identifier->datatype;
                }
                else
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` used with `input` must be an `int` or `float` and it's not (line %d)\n", identifier->token->text, $2->line);
                    exit(ERR_WRONG_PAR_INPUT);
                }
            }
            else
            {
                if (identifier->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a function and not a variable (line %d)\n", identifier->token->text, $2->line);
                    exit(ERR_FUNCTION);
                }
                else if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VECTOR)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a vector and not a function (line %d)\n", identifier->token->text, $2->line);
                    exit(ERR_VECTOR);
                }
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined identifier `%s` in line %d\n", $2->text, $2->line);
            exit(ERR_UNDECLARED);
        }
    }
    ;

output: TK_PR_OUTPUT TK_IDENTIFICADOR
    { 
        $$ = node_create("output", node_create_leaf($2), NULL, NULL, NULL, NULL);
        symbol_item_t* identifier = get_symbol($2->text);
        if (identifier != NULL)
        {
            if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
            {
                if (identifier->datatype == SYMBOL_DATATYPE_INT || identifier->datatype == SYMBOL_DATATYPE_FLOAT)
                {
                    $$->type = (node_type_t)identifier->datatype;
                }
                else
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` used with `output` must be an `int` or `float` and it's not (line %d)\n", identifier->token->text, $2->line);
                    exit(ERR_WRONG_PAR_OUTPUT);
                }
            }
            else
            {
                if (identifier->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a function and not a variable (line %d)\n", identifier->token->text, $2->line);
                    exit(ERR_FUNCTION);
                }
                else if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VECTOR)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a vector and not a function (line %d)\n", identifier->token->text, $2->line);
                    exit(ERR_VECTOR);
                }
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined identifier `%s` in line %d\n", $2->text, $2->line);
            exit(ERR_UNDECLARED);
        }
    }
    | TK_PR_OUTPUT literal 
    { 
        $$ = node_create("output", $2, NULL, NULL, NULL, NULL);
        symbol_item_t* literal = NULL;

        if ($2->type == NODE_TYPE_CHAR)
        {
            literal = get_symbol_lit_char($2->token->text);
        }
        else if ($2->type == NODE_TYPE_STRING)
        {
            literal = get_symbol_lit_string($2->token->text);
        }
        else
        {
            literal = get_symbol($2->token->text);
        }

        if (literal->datatype == SYMBOL_DATATYPE_INT || literal->datatype == SYMBOL_DATATYPE_FLOAT)
        {
            $$->type = (node_type_t)literal->datatype;
        }
        else
        {
            fprintf(stderr, "Semantic Error: literal `%s` used with `output` must be an `int` or `float` and it's not (line %d)\n", literal->token->text, $2->token->line);
            exit(ERR_WRONG_PAR_OUTPUT);
        }
    }
    ;

funccall: TK_IDENTIFICADOR '(' ')' 
    { 
        char* label = calloc(strlen($1->text) + strlen("call ") + 1, sizeof(char));
        sprintf(label, "call %s", $1->text);
        $$ = node_create(label, NULL, NULL, NULL, NULL, NULL);
        free(label);

        symbol_item_t* identifier = get_symbol($1->text);
        if (identifier != NULL)
        {
            if (identifier->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
            {
                if (identifier->params_queue == NULL)
                {
                    $$->type = identifier->datatype;
                }
                else
                {
                    fprintf(stderr, "Semantic Error: function `%s` needs %ld arguments but didn't receive any in line %d\n", identifier->token->text, identifier->params_queue->size, $1->line);
                    exit(ERR_MISSING_ARGS);
                }
            }
            else
            {
                if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a variable and not a function (line %d)\n", identifier->token->text, $1->line);
                    exit(ERR_VARIABLE);
                }
                else if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VECTOR)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a vector and not a function (line %d)\n", identifier->token->text, $1->line);
                    exit(ERR_VECTOR);
                }
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined function identifier `%s` in line %d\n", $1->text, $1->line);
            exit(ERR_UNDECLARED);
        }
    }
    | TK_IDENTIFICADOR '(' argslist ')' 
    { 
        char* label = calloc(strlen($1->text) + strlen("call ") + 1, sizeof(char));
        sprintf(label, "call %s", $1->text);
        $$ = node_create(label, $3, NULL, NULL, NULL, NULL); 
        free(label);

        symbol_item_t* identifier = get_symbol($1->text);
        if (identifier != NULL)
        {
            if (identifier->params_queue != NULL)
            {
                if (identifier->params_queue->size == args_types->size)
                {
                    for (size_t i = 0; i < identifier->params_queue->size; i++)
                    {
                        // Iterate over params_queue without removing items but removing items from args_types to clean it
                        symbol_datatype_t* expected = (symbol_datatype_t*)queue_at(identifier->params_queue, i);
                        symbol_datatype_t* got = (symbol_datatype_t*)stack_pop(args_types);
                        check_implicit_conversion((node_type_t)*expected, (node_type_t)*got, $1->line, 1);
                        free(got);
                    }
                    stack_destroy(args_types);

                    $$->type = identifier->datatype;
                }
                else
                {
                    fprintf(stderr, "Semantic Error: function `%s` needs %ld arguments but received %ld in line %d\n", identifier->token->text, identifier->params_queue->size, args_types->size, $1->line);
                    if (identifier->params_queue->size < args_types->size)
                    {
                        exit(ERR_EXCESS_ARGS);
                    }
                    else
                    {
                        exit(ERR_MISSING_ARGS);
                    }
                }
            }
            else
            {
                fprintf(stderr, "Semantic Error: function `%s` needs 0 arguments but received %ld in line %d\n", identifier->token->text, args_types->size, $1->line);
                exit(ERR_EXCESS_ARGS);
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined function identifier `%s` in line %d\n", $1->text, $1->line);
            exit(ERR_UNDECLARED);
        }
    }
    ;

argslist: expression ',' argslist 
    { 
        if ($1 != NULL) 
        { 
            $$ = node_link($1, $3);
            if (args_types == NULL)
                args_types = stack_create();

            symbol_datatype_t* datatype = calloc(1, sizeof(symbol_datatype_t));
            *datatype = (symbol_datatype_t)$1->type;
            stack_push(args_types, datatype);
        } 
        else 
        { 
            $$ = $3; 
        }; 
    }
    | expression 
    { 
        $$ = $1;

        if (args_types == NULL)
            args_types = stack_create();

        symbol_datatype_t* datatype = calloc(1, sizeof(symbol_datatype_t));
        *datatype = (symbol_datatype_t)$1->type;
        stack_push(args_types, datatype);
    }
    ;

shiftcommand: varname TK_OC_SL TK_LIT_INT
    {
        $$ = node_create($2->text, $1, node_create_leaf($3), NULL, NULL, NULL);
        symbol_item_t* identifier = get_symbol($1->token->text);
        $$->type = identifier->type;
        if (atoi($3->text) > 16)
        {
            fprintf(stderr, "Semantic Error: you can shift at most 16 positions, in line %d\n",$2->line);
            exit(ERR_WRONG_PAR_SHIFT);
        }
    }
    | varname TK_OC_SR TK_LIT_INT
    {
        $$ = node_create($2->text, $1, node_create_leaf($3), NULL, NULL, NULL);
        symbol_item_t* identifier = get_symbol($1->token->text);
        $$->type = identifier->type;
        if (atoi($3->text) > 16)
        {
            fprintf(stderr, "Semantic Error: you can shift at most 16 positions, in line %d\n",$2->line);
            exit(ERR_WRONG_PAR_SHIFT);
        }
    }
    ;

return: TK_PR_RETURN expression 
    { 
        if (type_coercion(current_function_type, $2->type, $2->token->line) == current_function_type)
        {
            $$ = node_create("return", $2, NULL, NULL, NULL, NULL);
            $$->type = current_function_type;
        }
        else
        {
            fprintf(stderr, "Semantic Error: return must be compatible with function type (line %d)\n", $2->token->line);
            exit(ERR_WRONG_PAR_RETURN);
        }
    }
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

arithmeticexpression: '+' arithmeticexpression
    {
        $$ = node_create("+", $2, NULL, NULL, NULL, NULL);
        $$->type = $2->type;

        iloc_argument_t temp = make_temp();
        $$->code = iloc_join($2->code, generate_arithmetic_unary_expression($1, $2->temp, temp));
        $$->temp = temp;

        // iloc_recursive_print($$->code);
    }
    | '-' arithmeticexpression
    {
        $$ = node_create("-", $2, NULL, NULL, NULL, NULL);
        $$->type = $2->type;

        iloc_argument_t temp = make_temp();
        $$->code = iloc_join($2->code, generate_arithmetic_unary_expression($1, $2->temp, temp));
        $$->temp = temp;
    }
    | '*' arithmeticexpression
    {
        $$ = node_create("*", $2, NULL, NULL, NULL, NULL);
        $$->type = $2->type;
    }
    | '&' arithmeticexpression
    {
        $$ = node_create("&", $2, NULL, NULL, NULL, NULL);
        $$->type = $2->type;
    }
    | '#' arithmeticexpression
    {
        $$ = node_create("#", $2, NULL, NULL, NULL, NULL);
        $$->type = $2->type;
    }
    | arithmeticexpression '+' arithmeticexpression
    {
        $$ = node_create("+", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);

        iloc_argument_t temp = make_temp();
        $$->code = iloc_join(iloc_join($1->code, $3->code), generate_arithmetic_binary_expression(ILOC_INS_ADD, $1->temp, $3->temp, temp));
        $$->temp = temp;

        // iloc_recursive_print($$->code);
    }
    | arithmeticexpression '-' arithmeticexpression
    {
        $$ = node_create("-", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);

        iloc_argument_t temp = make_temp();
        $$->code = iloc_join(iloc_join($1->code, $3->code), generate_arithmetic_binary_expression(ILOC_INS_SUB, $1->temp, $3->temp, temp));
        $$->temp = temp;
    }
    | arithmeticexpression '*' arithmeticexpression
    {
        $$ = node_create("*", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);

        iloc_argument_t temp = make_temp();
        $$->code = iloc_join(iloc_join($1->code, $3->code), generate_arithmetic_binary_expression(ILOC_INS_MULT, $1->temp, $3->temp, temp));
        $$->temp = temp;
    }
    | arithmeticexpression '/' arithmeticexpression
    {
        $$ = node_create("/", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);

        iloc_argument_t temp = make_temp();
        $$->code = iloc_join(iloc_join($1->code, $3->code), generate_arithmetic_binary_expression(ILOC_INS_DIV, $1->temp, $3->temp, temp));
        $$->temp = temp;
    }
    | arithmeticexpression '%' arithmeticexpression
    {
        $$ = node_create("%", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);
    }
    | arithmeticexpression '|' arithmeticexpression
    {
        $$ = node_create("|", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);
    }
    | arithmeticexpression '&' arithmeticexpression
    {
        $$ = node_create("&", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);
    }
    | arithmeticexpression '^' arithmeticexpression
    {
        $$ = node_create("^", $1, $3, NULL, NULL, NULL);
        $$->type = infer_type_nodes($1, $3, $2->line);
    }
    | TK_LIT_INT
    {
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_INT;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_INT, SYMBOL_DATATYPE_INT, NULL, IGNORE_DISP);

        iloc_argument_t temp = make_temp();
        iloc_argument_t number = {ILOC_ARG_TYPE_NUMBER, $1->value.v_integer};
        $$->code = generate_load_constant(temp, number);
        $$->temp = temp;
    }
    | TK_LIT_FLOAT
    {
        $$ = node_create_leaf($1);
        $$->type = NODE_TYPE_FLOAT;
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_FLOAT, SYMBOL_DATATYPE_FLOAT, NULL, IGNORE_DISP);
    }
    | varname                                       { $$ = $1; }
    | funccall                                      { $$ = $1; }
    | '(' arithmeticexpression ')'                  { $$ = $2; }
    ;

logicexpression: '!' logicexpression
    {
        $$ = node_create("!", $2, NULL, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | '!' varname
    {
        $$ = node_create("!", $2, NULL, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | '?' logicexpression
    {
        $$ = node_create("?", $2, NULL, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | '?' varname
    {
        $$ = node_create("?", $2, NULL, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | arithmeticexpression TK_OC_EQ arithmeticexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | arithmeticexpression TK_OC_NE arithmeticexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | arithmeticexpression TK_OC_GE arithmeticexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | arithmeticexpression TK_OC_LE arithmeticexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | arithmeticexpression '>' arithmeticexpression
    {
        $$ = node_create(">", $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | arithmeticexpression '<' arithmeticexpression
    {
        $$ = node_create("<", $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | logicexpression TK_OC_AND logicexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | varname TK_OC_AND logicexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | logicexpression TK_OC_AND varname
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | varname TK_OC_AND varname
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | logicexpression TK_OC_OR logicexpression { $$ = node_create($2->text, $1, $3, NULL, NULL, NULL); }
    | varname TK_OC_OR logicexpression
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | logicexpression TK_OC_OR varname
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | varname TK_OC_OR varname
    {
        $$ = node_create($2->text, $1, $3, NULL, NULL, NULL);
        $$->type = NODE_TYPE_BOOL;
    }
    | TK_LIT_TRUE
    {
        $$ = node_create_leaf($1);
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_BOOL, SYMBOL_DATATYPE_BOOL, NULL, IGNORE_DISP);
        $$->type = NODE_TYPE_BOOL;
    }
    | TK_LIT_FALSE
    {
        $$ = node_create_leaf($1);
        add_symbol($1->text, $1, SYMBOL_TYPE_LITERAL_BOOL, SYMBOL_DATATYPE_BOOL, NULL, IGNORE_DISP);
        $$->type = NODE_TYPE_BOOL;
    }
    ;

thernaryoperator: expression '?' expression ':' expression
    {
        $$ = node_create("?:", $1, $3, $5, NULL, NULL);
        $$->type = infer_type_nodes($3, $5, $4->line);
    }
    ;

varname: TK_IDENTIFICADOR
    {
        symbol_item_t* identifier = get_symbol($1->text);
        if (identifier != NULL)
        {
            if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
            {
                $$ = node_create_leaf($1);
                $$->type = identifier->datatype;

                iloc_argument_t rfp = {ILOC_ARG_TYPE_RFP, 0};
                iloc_argument_t address = {ILOC_ARG_TYPE_NUMBER, identifier->address};
                $$->temp = make_temp();
                $$->code = generate_load(rfp, address, $$->temp);
            }
            else
            {
                if (identifier->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a function and not a variable (line %d)\n", identifier->token->text, $1->line);
                    exit(ERR_FUNCTION);
                }
                else if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VECTOR)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a vector and not a variable (line %d)\n", identifier->token->text, $1->line);
                    exit(ERR_VECTOR);
                }
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined identifier `%s` in line %d\n", $1->text, $1->line);
            exit(ERR_UNDECLARED);
        }
    }
    | TK_IDENTIFICADOR '[' expression ']'
    {
        symbol_item_t* identifier = get_symbol($1->text);
        if (identifier != NULL)
        {
            if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VECTOR)
            {
                $$ = node_create("[]", node_create_leaf($1), $3, NULL, NULL, NULL);
                $$->type = identifier->datatype;
            }
            else
            {
                if (identifier->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a function and not a vector (line %d)\n", identifier->token->text, $1->line);
                    exit(ERR_FUNCTION);
                }
                else if (identifier->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
                {
                    fprintf(stderr, "Semantic Error: identifier `%s` is a variable and not a vector (line %d)\n", identifier->token->text, $1->line);
                    exit(ERR_VARIABLE);
                }
            }
        }
        else
        {
            fprintf(stderr, "Semantic Error: undefined identifier `%s` in line %d\n", $1->text, $1->line);
            exit(ERR_UNDECLARED);
        }
    }
    ;

%%

int yyerror(const char *s)
{
    fprintf(stderr,"Syntax Error: %s in line %d\n", s, num_lines);

    return 1;
}