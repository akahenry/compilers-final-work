/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _asm_opcode_t
{
    ASM_INS_NOP,
    ASM_INS_HALT,
    ASM_INS_MOV,
    ASM_INS_MOVL,
    ASM_INS_ADD,
    ASM_INS_SUB,
    ASM_INS_IMUL,
    ASM_INS_CLTD,
    ASM_INS_IDIV,
    ASM_INS_JMP,
    ASM_INS_CMP,
    ASM_INS_JE,
    ASM_INS_JNE,
    ASM_INS_LAHF,
    ASM_INS_AND,
    ASM_INS_RET,
    ASM_INS_CALL,
    ASM_INS_XOR,
    ASM_LABEL
} asm_opcode_t;

typedef enum _asm_arg_type_t
{
    ASM_ARG_TYPE_REGISTER,
    ASM_ARG_TYPE_LABEL,
    ASM_ARG_TYPE_IMM,
    ASM_ARG_TYPE_AH,
    ASM_ARG_TYPE_EAX,
    ASM_ARG_TYPE_EBX,
    ASM_ARG_TYPE_RBP,
    ASM_ARG_TYPE_RSP,
    ASM_ARG_TYPE_RIP,
    ASM_ARG_TYPE_IPC,
    ASM_ARG_TYPE_NONE
} asm_arg_type_t;

typedef struct _asm_argument_t
{
    asm_arg_type_t type;
    int isReference;
    int number;
} asm_argument_t;

typedef struct _asm_instruction_t
{
    asm_opcode_t opcode;
    asm_argument_t arg1;
    asm_argument_t arg2;
    asm_argument_t arg3;
    asm_argument_t arg4;
    int number;
    struct _asm_instruction_t* previous;
} asm_instruction_t;

asm_instruction_t* asm_create(asm_opcode_t opcode, asm_argument_t arg1, asm_argument_t arg2, asm_argument_t arg3, asm_argument_t arg4);
asm_instruction_t* asm_create_label(int number);
asm_instruction_t* asm_join(asm_instruction_t* ins1, asm_instruction_t* ins2);
char* asm_instruction_string(asm_instruction_t *ins);
const char* asm_opcode_string(asm_instruction_t* ins);
const char* asm_prefix_for_argument_type(asm_arg_type_t type);
char* asm_arg_string(asm_argument_t arg);
void asm_recursive_print(asm_instruction_t* ins);
void asm_clean();

#endif