/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>

#include "node.h"
#include "iloc_instruction.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse();
  // exporta (arvore);

  iloc_recursive_print(((node_t*)arvore)->code);

  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}
