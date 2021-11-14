/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "asm_converter.h"

const asm_argument_t RIP = {ASM_ARG_TYPE_RIP, 1, 0};
const asm_argument_t EAX = {ASM_ARG_TYPE_EAX, 0, 0};
const asm_argument_t EBX = {ASM_ARG_TYPE_EBX, 0, 0};
const asm_argument_t EBX_REF = {ASM_ARG_TYPE_EBX, 1, 0};
const asm_argument_t NONE = {ASM_ARG_TYPE_NONE, 0, 0};

asm_instruction_t* iloc_to_asm_recursive(iloc_instruction_t* ref)
{
    asm_instruction_t* ret = NULL;
    asm_argument_t arg1 = NONE;
    asm_argument_t arg2 = NONE;
    asm_argument_t arg3 = NONE;
    asm_argument_t arg4 = NONE;

    if(ref != NULL)
    {
        ret = calloc(1, sizeof(asm_instruction_t));
        arg1 = iloc_arg_to_asm_arg(ref->arg1);
        arg2 = iloc_arg_to_asm_arg(ref->arg2);
        arg3 = iloc_arg_to_asm_arg(ref->arg3);

        switch (ref->opcode)
        {
        case ILOC_INS_I2I:
            if (arg1.type == ASM_ARG_TYPE_REGISTER && arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE), asm_create(ASM_INS_MOV, EAX, arg2, RIP, NONE));
            }
            else if(arg1.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_create(ASM_INS_MOV, arg1, RIP, arg2, NONE);
            }
            else if (arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_create(ASM_INS_MOV, arg1, arg2, RIP, NONE);
            }
            else if (arg1.type != ASM_ARG_TYPE_REGISTER && arg2.type != ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_create(ASM_INS_MOV, arg1, arg2, NONE, NONE);
            }
            break;

        case ILOC_INS_ADDI:
            if (arg1.type != ASM_ARG_TYPE_IPC)
            {
                arg2.number *= -4;
            }
            
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_join(asm_create(ASM_INS_MOV, arg1, EAX, NONE, NONE), asm_create(ASM_INS_ADD, arg2, EAX, NONE, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
            }
            else if ((arg1.type == arg3.type && arg1.isReference == arg3.isReference) && arg2.type == ASM_ARG_TYPE_IMM)
            {
                ret = asm_create(ASM_INS_ADD, arg2, arg1, NONE, NONE);
            }
            else
            {
                ret = asm_join(asm_join(asm_create(ASM_INS_MOV, arg1, EAX, NONE, NONE), asm_create(ASM_INS_ADD, arg2, EAX, NONE, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, NONE, NONE));
            }
            break;

        case ILOC_INS_STOREAI:
            if (arg2.type != ASM_ARG_TYPE_RIP)
            {
                arg3.number *= -4;
            }
                
            if (arg1.type == ASM_ARG_TYPE_REGISTER && arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE), asm_create(ASM_INS_MOV, arg2, RIP, EBX, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, EBX_REF, NONE);
            }
            else if (arg1.type == ASM_ARG_TYPE_REGISTER)
            {
                arg2.isReference = 1;
                ret = asm_join(asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE), asm_create(ASM_INS_MOV, EAX, arg3, arg2, NONE));
            }
            else if (arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_create(ASM_INS_MOV, arg2, RIP, EBX, NONE), asm_create(ASM_INS_MOV, arg1, arg3, EBX_REF, NONE));
            }
            else
            {
                arg2.isReference = 1;
                ret = asm_create(ASM_INS_MOV, arg1, arg3, arg2, NONE);
            }
            break;

        case ILOC_INS_LOADI:
            if (arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_create(ASM_INS_MOVL, arg1, arg2, RIP, NONE);
            }
            else
            {
                ret = asm_create(ASM_INS_MOV, arg1, arg2, NONE, NONE);
            }
            break;

        case ILOC_INS_JUMPI:
            ret = asm_create(ASM_INS_JMP, arg1, NONE, NONE, NONE);
            break;

        case ILOC_LABEL:
            ret = asm_create_label(ref->number);
            break;

        case ILOC_INS_HALT:
            ret = asm_join(asm_create(ASM_INS_RET, NONE, NONE, NONE, NONE), asm_create(ASM_INS_HALT, NONE, NONE, NONE, NONE));
            break;

        case ILOC_INS_XORI:
            if (arg1.type == ASM_ARG_TYPE_REGISTER && arg3.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_join(asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE), asm_create(ASM_INS_XOR, arg2, EAX, NONE, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
            }
            else if (arg1.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_join(asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE), asm_create(ASM_INS_XOR, arg2, EAX, NONE, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, NONE, NONE));
            }
            else if (arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_join(asm_create(ASM_INS_MOV, arg1, EAX, NONE, NONE), asm_create(ASM_INS_XOR, arg2, EAX, NONE, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
            }
            else if (arg1.type != ASM_ARG_TYPE_REGISTER && arg2.type != ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_join(asm_join(asm_create(ASM_INS_MOV, arg1, EAX, NONE, NONE), asm_create(ASM_INS_XOR, arg2, EAX, NONE, NONE)), asm_create(ASM_INS_MOV, EAX, arg3, NONE, NONE));
            }
            break;
        
        default:
            break;
        }
        ret->previous = asm_join(iloc_to_asm_recursive(ref->previous), ret->previous);
    }

    return ret;
}

asm_instruction_t* iloc_to_asm(iloc_instruction_t* ref)
{
    asm_instruction_t* ret = NULL;

    ret = iloc_to_asm_recursive(ref);

    return ret;
}

asm_argument_t iloc_arg_to_asm_arg(iloc_argument_t ref)
{
    asm_argument_t ret;
    ret.number = ref.number;
    ret.isReference = 0;

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