/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "asm_converter.h"

const asm_argument_t RIP = {ASM_ARG_TYPE_RIP, 1, 0, NULL};
const asm_argument_t RBP = {ASM_ARG_TYPE_RBP, 0, 0, NULL};
const asm_argument_t RSP = {ASM_ARG_TYPE_RSP, 0, 0, NULL};
const asm_argument_t RSP_REF = {ASM_ARG_TYPE_RSP, 1, 0, NULL};
const asm_argument_t EAX = {ASM_ARG_TYPE_EAX, 0, 0, NULL};
const asm_argument_t POINTER_RAX = {ASM_ARG_TYPE_POINTER_RAX, 0, 0, NULL};
const asm_argument_t EBX = {ASM_ARG_TYPE_EBX, 0, 0, NULL};
const asm_argument_t EBX_REF = {ASM_ARG_TYPE_EBX, 1, 0, NULL};
const asm_argument_t RBX = {ASM_ARG_TYPE_RBX, 0, 0, NULL};
const asm_argument_t RBX_REF = {ASM_ARG_TYPE_RBX, 1, 0, NULL};
const asm_argument_t NONE = {ASM_ARG_TYPE_NONE, 0, 0, NULL};
const asm_argument_t ZERO = {ASM_ARG_TYPE_IMM, 0, 0, NULL};
const asm_argument_t ONE = {ASM_ARG_TYPE_IMM, 0, 1, NULL};
const asm_argument_t OBJECT = {ASM_ARG_TYPE_OBJECT, 0, 0, NULL};
const asm_argument_t FUNCTION = {ASM_ARG_TYPE_FUNCTION, 0, 0, NULL};

asm_instruction_t* iloc_to_asm_recursive(iloc_instruction_t* ref)
{
    asm_instruction_t* ret = NULL;
    asm_argument_t arg1 = NONE;
    asm_argument_t arg2 = NONE;
    asm_argument_t arg3 = NONE;
    asm_argument_t arg4 = NONE;

    if(ref != NULL)
    {
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
        
        case ILOC_INS_LOAD: 
            if (arg1.type == ASM_ARG_TYPE_REGISTER)
            {
                ret = asm_create(ASM_INS_MOV, arg1, RIP, RBX, NONE);
            }
            else
            {
                ret = asm_create(ASM_INS_MOV, arg1, RBX, NONE, NONE);
            }
            ret = asm_join(ret, asm_create(ASM_INS_MOV, RBX_REF, EAX, NONE, NONE));
            ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg2, RIP, NONE));
            break;

        case ILOC_INS_LOADAI:
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
            {
                if (arg1.type == ASM_ARG_TYPE_RIP || arg1.type == ASM_ARG_TYPE_RBP)
                {
                    arg1.isReference = 1;
                    arg2.number = arg1.type == ASM_ARG_TYPE_RBP ? arg2.number * -4 : arg2.number;
                    ret = asm_join(ret, asm_create(ASM_INS_MOV, arg2, arg1, EAX, NONE));
                    ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
                }
                else
                {
                    if (arg1.type == ASM_ARG_TYPE_REGISTER)
                    {
                        ret = asm_create(ASM_INS_MOV, arg1, RIP, RBX, NONE);
                    }
                    else
                    {
                        ret = asm_create(ASM_INS_MOV, arg1, RBX, NONE, NONE);
                    }
                    ret = asm_join(ret, asm_create(ASM_INS_MOV, arg2, RBX_REF, EAX, NONE));
                    ret = asm_join(ret, asm_create(ASM_INS_MOV, EAX, arg3, RIP, NONE));
                }
            }
            else
            {
                if (arg1.type == ASM_ARG_TYPE_REGISTER)
                {
                    ret = asm_create(ASM_INS_MOV, arg1, RIP, RBX, NONE);
                    ret = asm_join(ret, asm_create(ASM_INS_MOV, arg2, RBX_REF, arg3, NONE));
                }
                else
                {
                    arg1.isReference = 1;
                    ret = asm_join(ret, asm_create(ASM_INS_MOV, arg2, arg1, arg3, NONE));
                }
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
        
        case ILOC_INS_JUMP:
            ret = asm_create(ASM_INS_MOV, arg1, RIP, EAX, NONE);
            ret = asm_join(ret, asm_create(ASM_INS_JMP, POINTER_RAX, NONE, NONE, NONE));
            break;
        
        case ILOC_INS_CBR:
            if (arg1.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_create(ASM_INS_CMP, ZERO, arg1, RIP, NONE);
            else
                ret = asm_create(ASM_INS_CMP, ZERO, arg1, NONE, NONE);

            ret = asm_join(ret, asm_create(ASM_INS_JE, arg3, NONE, NONE, NONE));
            ret = asm_join(ret, asm_create(ASM_INS_JNE, arg2, NONE, NONE, NONE));
            break;

        case ILOC_LABEL:
            ret = asm_create_label(ref->number);
            break;

        case ILOC_INS_HALT:;
            asm_argument_t sixteen = {ASM_ARG_TYPE_IMM, 0, 16, NULL};
            ret = asm_join(asm_join(asm_join(asm_create(ASM_INS_MOV, sixteen, RSP_REF, EAX, NONE), asm_create(ASM_INS_MOV, RBP, RSP, NONE, NONE)), asm_create(ASM_INS_RET, NONE, NONE, NONE, NONE)), asm_create(ASM_INS_HALT, NONE, NONE, NONE, NONE));
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
                ret = asm_join(ret, asm_create(ASM_INS_MOVL, ONE, arg3, RIP, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_MOVL, ONE, arg3, NONE, NONE));
            
            // jmp <LABELEND>
            ret = asm_join(ret, asm_create(ASM_INS_JMP, arg_label_end, NONE, NONE, NONE));

            // <LABELFALSE>:
            ret = asm_join(ret, label_false);

            // mov $0, r3
            if (arg3.type == ASM_ARG_TYPE_REGISTER)
                ret = asm_join(ret, asm_create(ASM_INS_MOVL, ZERO, arg3, RIP, NONE));
            else
                ret = asm_join(ret, asm_create(ASM_INS_MOVL, ZERO, arg3, NONE, NONE));

            // <LABELEND>:
            ret = asm_join(ret, label_end);
            break;
        
        default:
            ret = asm_create(ASM_INS_NOP, NONE, NONE, NONE, NONE);
            break;
        }
        ret->previous = asm_join(iloc_to_asm_recursive(ref->previous), ret->previous);
    }

    return ret;
}

asm_instruction_t* asm_header()
{
    symbol_table_t* table = get_table();
    asm_instruction_t* data = asm_create(ASM_DATA_SECTION, NONE, NONE, NONE, NONE);
    asm_instruction_t* text = asm_create(ASM_TEXT_SECTION, NONE, NONE, NONE, NONE);
    int maxTemp = get_temp_count();

    hash_table_t* global_scope = (hash_table_t*)stack_at(table->scopes, 0);
    for (size_t i = 0; i < global_scope->size; i++)
    {
        if (global_scope->items[i] != NULL)
        {
            symbol_item_t* item = (symbol_item_t*)global_scope->items[i];
            if (item->type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
            {
                asm_argument_t arg = {ASM_ARG_TYPE_IDENTIFIER, 0, 0, item->token->text};
                asm_argument_t size = {ASM_ARG_TYPE_IMM, 0, item->size, NULL};
                data = asm_join(data, asm_create(ASM_GLOBL, arg, NONE, NONE, NONE));
                data = asm_join(data, asm_create(ASM_TYPE, arg, OBJECT, NONE, NONE));
                data = asm_join(data, asm_create(ASM_SIZE, arg, size, NONE, NONE));
                data = asm_join(data, asm_create(ASM_IDENTIFIER_LABEL, arg, NONE, NONE, NONE));
                data = asm_join(data, asm_create(ASM_LONG, ZERO, NONE, NONE, NONE));
            }
            else if (item->type == SYMBOL_TYPE_IDENTIFIER_FUNCTION)
            {
                asm_argument_t arg = {ASM_ARG_TYPE_IDENTIFIER_FUNCTION, 0, item->label.number, NULL};
                if (strcmp("main", item->token->text) == 0)
                {
                    arg.type = ASM_ARG_TYPE_IDENTIFIER;
                    arg.identifier = item->token->text;
                    set_main_label(item->label.number);
                }
                text = asm_join(text, asm_create(ASM_GLOBL, arg, NONE, NONE, NONE));
                text = asm_join(text, asm_create(ASM_TYPE, arg, FUNCTION, NONE, NONE));
            }
        }
    }

    for (size_t i = 0; i < maxTemp; i++)
    {
        asm_argument_t arg = {ASM_ARG_TYPE_REGISTER, 0, i, NULL};
        asm_argument_t size = {ASM_ARG_TYPE_IMM, 0, 4, NULL};
        data = asm_join(data, asm_create(ASM_GLOBL, arg, NONE, NONE, NONE));
        data = asm_join(data, asm_create(ASM_TYPE, arg, OBJECT, NONE, NONE));
        data = asm_join(data, asm_create(ASM_SIZE, arg, size, NONE, NONE));
        data = asm_join(data, asm_create(ASM_IDENTIFIER_LABEL, arg, NONE, NONE, NONE));
        data = asm_join(data, asm_create(ASM_LONG, ZERO, NONE, NONE, NONE));
    }
    
    return asm_join(data, text);
}

asm_instruction_t* iloc_to_asm(iloc_instruction_t* ref)
{
    asm_instruction_t* ret = NULL;

    ret = asm_header();

    ret = asm_join(ret, iloc_to_asm_recursive(ref));

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