/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/

#include <stdio.h>

#include "node.h"
#include "iloc_instruction.h"
#include "asm_converter.h"

extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int is_optimized = 0;

  if (argc > 2)
  {
    printf("too many arguments.\n");
    exit(EXIT_FAILURE);
  }
  else if (argc == 2)
  {
    if (strcmp(argv[1], "-O") == 0)
    {
      is_optimized = 1;
    }
    else
    {
      printf("unrecognized argument \"%s\", use without argument or with the optional argument -O.\n", argv[1]);
    }
  }

  int ret = yyparse(); // A geração de código intermediário foi realizada na mesma passagem em que feita a análise sintática/semântica
  
  // iloc_recursive_print(((node_t*)arvore)->code);

  asm_instruction_t* asm_code = iloc_to_asm(((node_t*)arvore)->code);

  if (is_optimized)
  {
    asm_code = optimize_asm_code(asm_code);
  }

  asm_recursive_print(asm_code);

  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}
