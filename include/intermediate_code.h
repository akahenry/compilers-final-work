/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef INTERMEDIATE_CODE_H
#define INTERMEDIATE_CODE_H

#include "iloc_instruction.h"
#include "symbol_table.h"
#include "node.h"

iloc_argument_t make_temp();
iloc_argument_t make_label();

iloc_instruction_t* generate_attribution(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t expression);
iloc_instruction_t* generate_attribution_vector(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t offset, iloc_argument_t expression);

#endif