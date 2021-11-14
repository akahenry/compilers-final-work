/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "asm_converter.h"

asm_instruction_t* iloc_to_asm(iloc_instruction_t* ref)
{
    asm_instruction_t* ret = NULL;

    return ret;
}

asm_argument_t iloc_arg_to_asm_arg(iloc_argument_t ref)
{
    asm_argument_t ret;
    ret.number = ref.number;

    switch (ref.type)
    {
        case ILOC_ARG_TYPE_REGISTER:
            ret.type = ASM_ARG_TYPE_REGISTER;
            break;
        case ILOC_ARG_TYPE_LABEL:
            ret.type = ASM_ARG_TYPE_LABEL;
            break;
        case ILOC_ARG_TYPE_NUMBER:
            ret.type = ASM_ARG_TYPE_IMM;
            break;
        case ILOC_ARG_TYPE_RPC:
            ret.type = ASM_ARG_TYPE_IPC;
            break;
        case ILOC_ARG_TYPE_RFP:
            ret.type = ASM_ARG_TYPE_RBP;
            break;
        case ILOC_ARG_TYPE_RSP:
            ret.type = ASM_ARG_TYPE_RSP;
            break;
        case ILOC_ARG_TYPE_RBSS:
            ret.type = ASM_ARG_TYPE_RIP;
            break;
        
        default:
            break;
    }

    return ret;
}