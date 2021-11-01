/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
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
  int ret = yyparse(); // A geração de código intermediário foi realizada na mesma passagem em que feita a análise sintática/semântica
  
  iloc_recursive_print(((node_t*)arvore)->code);

  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}
