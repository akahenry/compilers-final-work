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
iloc_instruction_t* generate_arithmetic_binary_expression(iloc_opcode_t opcode, iloc_argument_t register1, iloc_argument_t register2, iloc_argument_t result);
iloc_instruction_t* generate_load_constant(iloc_argument_t reg, iloc_argument_t constant);
iloc_instruction_t* generate_load(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t reg);
iloc_instruction_t* generate_arithmetic_unary_expression(token_t* token, iloc_argument_t operand, iloc_argument_t result);
#endif