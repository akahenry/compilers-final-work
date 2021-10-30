/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef ILOC_INSTRUCTION_H
#define ILOC_INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _iloc_opcode_t
{
    ILOC_INS_NOP,
    ILOC_INS_ADD,
    ILOC_INS_SUB,
    ILOC_INS_MULT,
    ILOC_INS_DIV,
    ILOC_INS_ADDI,
    ILOC_INS_SUBI,
    ILOC_INS_RSUBI,
    ILOC_INS_MULTI,
    ILOC_INS_DIVI,
    ILOC_INS_RDIVI,
    ILOC_INS_LSHIFT,
    ILOC_INS_LSHIFTI,
    ILOC_INS_RSHIFT,
    ILOC_INS_RSHIFTI,
    ILOC_INS_AND,
    ILOC_INS_ANDI,
    ILOC_INS_OR,
    ILOC_INS_ORI,
    ILOC_INS_XOR,
    ILOC_INS_XORI,
    ILOC_INS_LOADI,
    ILOC_INS_LOAD,
    ILOC_INS_LOADAI,
    ILOC_INS_LOADA0,
    ILOC_INS_CLOAD,
    ILOC_INS_CLOADAI,
    ILOC_INS_CLOADA0,
    ILOC_INS_STORE,
    ILOC_INS_STOREAI,
    ILOC_INS_STOREAO,
    ILOC_INS_CSTORE,
    ILOC_INS_CSTOREAI,
    ILOC_INS_CSTOREAO,
    ILOC_INS_I2I,
    ILOC_INS_C2C,
    ILOC_INS_C2I,
    ILOC_INS_I2C,
    ILOC_INS_JUMPI,
    ILOC_INS_JUMP,
    ILOC_INS_CBR,
    ILOC_INS_CMP_LT,
    ILOC_INS_CMP_LE,
    ILOC_INS_CMP_EQ,
    ILOC_INS_CMP_GE,
    ILOC_INS_CMP_GT,
    ILOC_INS_CMP_NE,
    ILOC_LABEL
} iloc_opcode_t;

typedef enum _iloc_arg_type_t
{
    ILOC_ARG_TYPE_REGISTER,
    ILOC_ARG_TYPE_LABEL,
    ILOC_ARG_TYPE_NUMBER,
    ILOC_ARG_TYPE_RFP,
    ILOC_ARG_TYPE_RSP,
    ILOC_ARG_TYPE_RBSS,
    ILOC_ARG_TYPE_RPC,
    ILOC_ARG_TYPE_NONE
} iloc_arg_type_t;

typedef struct _iloc_argument_t
{
    iloc_arg_type_t type;
    int number;

} iloc_argument_t;

typedef struct _iloc_instruction_t
{
    iloc_opcode_t opcode;
    iloc_argument_t arg1;
    iloc_argument_t arg2;
    iloc_argument_t arg3;
    int number;
    struct _iloc_instruction_t* previous;
} iloc_instruction_t;

iloc_instruction_t* iloc_create(iloc_opcode_t opcode, iloc_argument_t arg1, iloc_argument_t arg2, iloc_argument_t arg3);
iloc_instruction_t* iloc_create_label(int number);
iloc_instruction_t* iloc_join(iloc_instruction_t* ins1, iloc_instruction_t* ins2);
char* iloc_instruction_string(iloc_instruction_t *ins);
const char* iloc_opcode_string(iloc_instruction_t* ins);
const char* iloc_prefix_for_argument_type(iloc_arg_type_t type);
char* iloc_arg_string(iloc_argument_t arg);
void iloc_recursive_print(iloc_instruction_t* ins);
void iloc_clean();

#endif