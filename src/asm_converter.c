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
const asm_argument_t zero = {ASM_ARG_TYPE_IMM, 0, 0};
const asm_argument_t one = {ASM_ARG_TYPE_IMM, 0, 1};

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

        case ILOC_INS_ADD:
        case ILOC_INS_SUB:
        case ILOC_INS_MULT:
        case ILOC_INS_DIV:
            if (arg1.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE);
            else
                ret = asm_create(ASM_INS_MOV, arg1, EAX, NONE, NONE);
            
            if (arg2.type == ASM_ARG_TYPE_REGISTER)
            {
                if (ref->opcode == ILOC_INS_ADD)
                    ret = asm_join(ret, asm_create(ASM_INS_ADD, arg2, RIP, EAX, NONE));
                else if (ref->opcode == ILOC_INS_SUB)
                    ret = asm_join(ret, asm_create(ASM_INS_SUB, arg2, RIP, EAX, NONE));
                else if (ref->opcode == ILOC_INS_MULT)
                    ret = asm_join(ret, asm_create(ASM_INS_IMUL, arg2, RIP, NONE, NONE));
                else if (ref->opcode == ILOC_INS_DIV)
                    ret = asm_join(ret, asm_join(asm_create(ASM_INS_CLTD, NONE, NONE, NONE, NONE), asm_create(ASM_INS_IDIV, arg2, RIP, NONE, NONE)));
            }
            else
            {
                if (ref->opcode == ILOC_INS_ADD)
                    ret = asm_join(ret, asm_create(ASM_INS_ADD, arg2, EAX, NONE, NONE));
                else if (ref->opcode == ILOC_INS_SUB)
                    ret = asm_join(ret, asm_create(ASM_INS_SUB, arg2, EAX, NONE, NONE));
                else if (ref->opcode == ILOC_INS_MULT)
                    ret = asm_join(ret, asm_create(ASM_INS_IMUL, arg2, NONE, NONE, NONE));
                else if (ref->opcode == ILOC_INS_DIV)
                    ret = asm_join(ret, asm_join(asm_create(ASM_INS_CLTD, NONE, NONE, NONE, NONE), asm_create(ASM_INS_IDIV, arg2, NONE, NONE, NONE)));
            }
            
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg3, NONE, NONE));
            break;
        
        case ILOC_INS_RSUBI:
            ret = asm_create(ASM_INS_MOV, arg2, EAX, NONE, NONE);

            if (arg1.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(ret, asm_create(ASM_INS_ADD, arg1, RIP, EAX, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_ADD, arg1, EAX, NONE, NONE));
            
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg3, NONE, NONE));
            break;

        case ILOC_INS_JUMPI:
            ret = asm_create(ASM_INS_JMP, arg1, NONE, NONE, NONE);
            break;
        
        case ILOC_INS_CBR:
            if (arg1.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_create(ASM_INS_CMP, zero, arg1, RIP, NONE);
            else
                ret = asm_create(ASM_INS_CMP, zero, arg1, NONE, NONE);

            ret = asm_join(ret, asm_create(ASM_INS_JE, arg3, NONE, NONE, NONE));
            ret = asm_join(ret, asm_create(ASM_INS_JNE, arg2, NONE, NONE, NONE));
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
        
        case ILOC_INS_CMP_EQ:
        case ILOC_INS_CMP_NE:
        case ILOC_INS_CMP_GE:
        case ILOC_INS_CMP_LE:
        case ILOC_INS_CMP_GT:
        case ILOC_INS_CMP_LT:
            // cmp r1, r2
            if (arg1.type == ASM_ARG_TYPE_REGISTER && arg2.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(asm_create(ASM_INS_MOV, arg2, RIP, EAX, NONE), asm_create(ASM_INS_CMP, arg1, RIP, EAX, NONE));
            else if (arg1.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_create(ASM_INS_CMP, arg1, RIP, arg2, NONE);
            else if (arg2.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_create(ASM_INS_CMP, arg1, arg2, RIP, NONE);
            else if (arg1.type != ASM_ARG_TYPE_REGISTER && arg2.type != ASM_ARG_TYPE_REGISTER)
                ret = asm_create(ASM_INS_CMP, arg1, arg2, NONE, NONE);
            
            asm_instruction_t* label_false = asm_create_cmp_label();
            asm_instruction_t* label_end = asm_create_cmp_label();
            asm_argument_t arg_label_false = asm_create_label_arg_from_label_ins(label_false);
            asm_argument_t arg_label_end = asm_create_label_arg_from_label_ins(label_end);

            // <jmp com teste negado> <LABELFALSE>
            switch (ref->opcode)
            {
            case ILOC_INS_CMP_EQ:
                ret = asm_join(ret, asm_create(ASM_INS_JNE, arg_label_false, NONE, NONE, NONE));
                break;
            case ILOC_INS_CMP_NE:
                ret = asm_join(ret, asm_create(ASM_INS_JE, arg_label_false, NONE, NONE, NONE));
                break;
            case ILOC_INS_CMP_GE:
                ret = asm_join(ret, asm_create(ASM_INS_JL, arg_label_false, NONE, NONE, NONE));
                break;
            case ILOC_INS_CMP_LE:
                ret = asm_join(ret, asm_create(ASM_INS_JG, arg_label_false, NONE, NONE, NONE));
                break;
            case ILOC_INS_CMP_GT:
                ret = asm_join(ret, asm_create(ASM_INS_JLE, arg_label_false, NONE, NONE, NONE));
                break;
            case ILOC_INS_CMP_LT:
                ret = asm_join(ret, asm_create(ASM_INS_JGE, arg_label_false, NONE, NONE, NONE));
                break;
            default:
                break;
            }

            // mov $1, r3
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(ret, asm_create(ASM_INS_MOV, one, arg3, RIP, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_MOV, one, arg3, NONE, NONE));
            
            // jmp <LABELEND>
            ret = asm_join(ret, asm_create(ASM_INS_JMP, arg_label_end, NONE, NONE, NONE));

            // <LABELFALSE>:
            ret = asm_join(ret, label_false);

            // mov $1, r3
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(ret, asm_create(ASM_INS_MOV, zero, arg3, RIP, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_MOV, zero, arg3, NONE, NONE));

            // <LABELEND>:
            ret = asm_join(ret, label_end);
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