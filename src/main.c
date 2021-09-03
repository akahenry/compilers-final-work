/*
Função principal para impressão de tokens.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include <stdio.h>
#include "parser.tab.h"

extern int yylex(void);
extern int yylex_destroy(void);

int yyparse();

extern FILE *yyin;
extern char *yytext;
extern int get_line_number (void);
#define print_nome(TOKEN) \
    printf("%d " #TOKEN " [%s]\n", get_line_number(), yytext);
#define print_nome2(TOKEN) \
    printf("%d TK_ESPECIAL [%c]\n", get_line_number(), TOKEN);


int main(int argc, char **argv) {
  int ret = yyparse();
  return ret;
}