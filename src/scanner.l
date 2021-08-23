%{
#include "tokens.h"

int num_lines = 1;
int get_line_number(void);
%}

%x COMMENT

white [ \t]+
digit [0-9]
integer {digit}+
exponent [eE][+-]?{integer}
real {integer}"."{integer}?{exponent}?
single_comment "//".*
multi_comment "/"\*
letter [a-zA-Z]
alphabetical "_"|{letter}
identifier {alphabetical}+({digit}|{alphabetical})*

int [+-]?{integer}
float [+-]?{real}
false "false"
true "true"
char '.'
string \".*\"

%%

"\n" { ++num_lines; }

{white} { ; }

"int" { return TK_PR_INT; }
"float" {return TK_PR_FLOAT; }
"bool" { return TK_PR_BOOL; } 
"char" { return TK_PR_CHAR; } 
"string" { return TK_PR_STRING; } 
"if" { return TK_PR_IF; } 
"then" { return TK_PR_THEN; } 
"else" { return TK_PR_ELSE; } 
"while" { return TK_PR_WHILE; } 
"do" { return TK_PR_DO; } 
"input" { return TK_PR_INPUT; } 
"output" { return TK_PR_OUTPUT; } 
"return" { return TK_PR_RETURN; } 
"const" { return TK_PR_CONST; } 
"static" { return TK_PR_STATIC; } 
"foreach" { return TK_PR_FOREACH; } 
"for" { return TK_PR_FOR; } 
"switch" { return TK_PR_SWITCH; } 
"case" { return TK_PR_CASE; } 
"break" { return TK_PR_BREAK; } 
"continue" { return TK_PR_CONTINUE; } 
"class" { return TK_PR_CLASS; } 
"private" { return TK_PR_PRIVATE; } 
"public" { return TK_PR_PUBLIC; } 
"protected" { return TK_PR_PROTECTED; } 
"end" { return TK_PR_END; } 
"default" { return TK_PR_DEFAULT; } 

"," { return (int)','; }
";" { return (int)';'; } 
":" { return (int)':'; } 
"(" { return (int)'('; } 
")" { return (int)')'; } 
"[" { return (int)'['; } 
"]" { return (int)']'; } 
"{" { return (int)'{'; }
"}" { return (int)'}'; } 
"+" { return (int)'+'; } 
"-" { return (int)'-'; } 
"|" { return (int)'|'; }
"*" { return (int)'*'; } 
"/" { return (int)'/'; } 
"<" { return (int)'<'; } 
">" { return (int)'>'; } 
"=" { return (int)'='; } 
"!" { return (int)'!'; } 
"&" { return (int)'&'; } 
"%" { return (int)'%'; } 
"#" { return (int)'#'; } 
"ˆ" { return (int)'^'; }
"." { return (int)'.'; } 
"$" { return (int)'$'; }

"<=" { return TK_OC_LE; }
">=" { return TK_OC_GE; }
"==" { return TK_OC_EQ; }
"!=" { return TK_OC_NE; }
"&&" { return TK_OC_AND; }
"||" { return TK_OC_OR; }
">>" { return TK_OC_SL; }
"<<" { return TK_OC_SR; }

{int} { return TK_LIT_INT; }
{float} { return TK_LIT_FLOAT; }
{false} { return TK_LIT_FALSE; }
{true} { return TK_LIT_TRUE; }
{char} { return TK_LIT_CHAR; }
{string} { return TK_LIT_STRING; }

{identifier} { return TK_IDENTIFICADOR; }

{single_comment} { ; }
{multi_comment} BEGIN(COMMENT);

. { return TOKEN_ERRO; }

<COMMENT>.
<COMMENT>"\n" ++num_lines;
<COMMENT>"*/" BEGIN(INITIAL);
%%

int get_line_number(void) 
{
    return num_lines;
}
