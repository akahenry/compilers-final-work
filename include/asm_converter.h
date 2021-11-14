/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef ASM_CONVERTER_H
#define ASM_CONVERTER_H

#include "assembly.h"
#include "iloc_instruction.h"

asm_instruction_t* iloc_to_asm(iloc_instruction_t* ref);
asm_argument_t iloc_arg_to_asm_arg(iloc_argument_t ref);

#endif