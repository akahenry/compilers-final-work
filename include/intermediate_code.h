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
int get_temp_count();

iloc_instruction_t* generate_attribution(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t expression);
iloc_instruction_t* generate_attribution_vector(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t offset, iloc_argument_t expression);
iloc_instruction_t* generate_attribution_from_address(iloc_argument_t reference_address_register1, iloc_argument_t address1, iloc_argument_t reference_address_register2, iloc_argument_t address2);
iloc_instruction_t* generate_arithmetic_binary_expression(iloc_opcode_t opcode, iloc_argument_t register1, iloc_argument_t register2, iloc_argument_t result);
iloc_instruction_t* generate_load_constant(iloc_argument_t reg, iloc_argument_t constant);
iloc_instruction_t* generate_load(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t reg);
iloc_instruction_t* generate_arithmetic_unary_expression(token_t* token, iloc_argument_t operand, iloc_argument_t result);
iloc_instruction_t* generate_funcdec(iloc_argument_t label, iloc_argument_t disp);
iloc_instruction_t* generate_load_vector(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t offset, iloc_argument_t reg);
iloc_instruction_t* generate_attribution_vector_from_address(iloc_argument_t reference_address_register, iloc_argument_t address1, iloc_argument_t offset, iloc_argument_t address2);
iloc_instruction_t* generate_return(iloc_argument_t expression, iloc_argument_t declared_variables_count);
iloc_instruction_t* generate_jump_halt();
iloc_instruction_t* generate_funccall(token_t* funcname_token, node_t* arguments_node, iloc_argument_t reg);
iloc_instruction_t* generate_relational_comparison(iloc_opcode_t opcode, iloc_argument_t register1, iloc_argument_t register2, iloc_argument_t result);
iloc_instruction_t* generate_if(iloc_argument_t boolean_expression, iloc_instruction_t* true_code, iloc_instruction_t* false_code);
iloc_instruction_t* generate_while(iloc_argument_t boolean_expression, iloc_instruction_t* boolean_code, iloc_instruction_t* code);
iloc_instruction_t* generate_for(iloc_argument_t var_init, iloc_argument_t boolean_expression, iloc_instruction_t* boolean_code, iloc_argument_t var_assignment, iloc_instruction_t* var_assignment_code, iloc_instruction_t* code);
iloc_instruction_t* generate_not_operator(iloc_argument_t reg);
iloc_instruction_t* generate_logic_and_expression(iloc_argument_t register1, iloc_instruction_t* code1, iloc_argument_t register2, iloc_instruction_t* code2, iloc_argument_t result);
iloc_instruction_t* generate_logic_or_expression(iloc_argument_t register1, iloc_instruction_t* code1, iloc_argument_t register2, iloc_instruction_t* code2, iloc_argument_t result);

#endif