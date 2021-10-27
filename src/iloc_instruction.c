/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "iloc_instruction.h"

void sprint_iloc_instruction(char *str, iloc_instruction_t *ins)
{
    const char* opcode = opcode_string(ins);
    const char* arg1_prefix = prefix_for_argument_type(ins->arg1.type);
    const char* arg2_prefix = prefix_for_argument_type(ins->arg2.type);
    const char* arg3_prefix = prefix_for_argument_type(ins->arg3.type);

    switch (ins->opcode)
    {
        case ILOC_INS_NOP:
            sprintf(str, "%s", opcode);
            break;
        
        case ILOC_INS_LOADI:
        case ILOC_INS_LOAD:
        case ILOC_INS_CLOAD:
        case ILOC_INS_STORE:
        case ILOC_INS_CSTORE:
        case ILOC_INS_I2I:
        case ILOC_INS_C2C:
        case ILOC_INS_C2I:
        case ILOC_INS_I2C:
            sprintf(str, "%s %s%d => %s%d",
                    opcode,
                    arg1_prefix, ins->arg1.number,
                    arg2_prefix, ins->arg2.number);
            break;
        
        case ILOC_INS_ADD:
        case ILOC_INS_SUB:
        case ILOC_INS_MULT:
        case ILOC_INS_DIV:
        case ILOC_INS_ADDI:
        case ILOC_INS_SUBI:
        case ILOC_INS_RSUBI:
        case ILOC_INS_MULTI:
        case ILOC_INS_DIVI:
        case ILOC_INS_RDIVI:
        case ILOC_INS_LSHIFT:
        case ILOC_INS_LSHIFTI:
        case ILOC_INS_RSHIFT:
        case ILOC_INS_RSHIFTI:
        case ILOC_INS_AND:
        case ILOC_INS_ANDI:
        case ILOC_INS_OR:
        case ILOC_INS_ORI:
        case ILOC_INS_XOR:
        case ILOC_INS_XORI:
        case ILOC_INS_LOADAI:
        case ILOC_INS_LOADA0:
        case ILOC_INS_CLOADAI:
        case ILOC_INS_CLOADA0:
            sprintf(str, "%s %s%d %s%d => %s%d",
                    opcode,
                    arg1_prefix, ins->arg1.number,
                    arg2_prefix, ins->arg2.number,
                    arg3_prefix, ins->arg3.number);
            break;
        
        case ILOC_INS_STOREAI:
        case ILOC_INS_STOREAO:
        case ILOC_INS_CSTOREAI:
        case ILOC_INS_CSTOREAO:
            sprintf(str, "%s %s%d => %s%d %s%d",
                    opcode,
                    arg1_prefix, ins->arg1.number,
                    arg2_prefix, ins->arg2.number,
                    arg3_prefix, ins->arg3.number);
            break;
        
        case ILOC_INS_JUMPI:
        case ILOC_INS_JUMP:
            sprintf(str, "%s -> %s%d",
                    opcode,
                    arg1_prefix, ins->arg1.number);
            break;
        
        case ILOC_INS_CBR:
            sprintf(str, "%s %s%d -> %s%d %s%d",
                    opcode,
                    arg1_prefix, ins->arg1.number,
                    arg2_prefix, ins->arg2.number,
                    arg3_prefix, ins->arg3.number);
            break;
        
        case ILOC_INS_CMP_LT:
        case ILOC_INS_CMP_LE:
        case ILOC_INS_CMP_EQ:
        case ILOC_INS_CMP_GE:
        case ILOC_INS_CMP_GT:
        case ILOC_INS_CMP_NE:
            sprintf(str, "%s %s%d %s%d -> %s%d",
                    opcode,
                    arg1_prefix, ins->arg1.number,
                    arg2_prefix, ins->arg2.number,
                    arg3_prefix, ins->arg3.number);
            break;
        
        default:
            sprintf(str, "UNRECOGNIZED OPCODE");
            break;
    }
}

const char* prefix_for_argument_type(iloc_arg_type_t type)
{
    switch (type)
    {
        case ILOC_ARG_TYPE_REGISTER:
            return "r";
            break;
        case ILOC_ARG_TYPE_LABEL:
            return "L";
            break;
        case ILOC_ARG_TYPE_NUMBER:
            return "";
            break;
        // anything else returns an empty prefix
        default:
            return "";
            break;
    }
}

const char* opcode_string(iloc_instruction_t *ins)
{
    switch (ins->opcode)
    {
        case ILOC_INS_NOP:
            return "nop";
        case ILOC_INS_LOADI:
            return "loadI";
        case ILOC_INS_LOAD:
            return "load";
        case ILOC_INS_CLOAD:
            return "cload";
        case ILOC_INS_STORE:
            return "store";
        case ILOC_INS_CSTORE:
            return "cstore";
        case ILOC_INS_I2I:
            return "i2i";
        case ILOC_INS_C2C:
            return "c2c";
        case ILOC_INS_C2I:
            return "c2i";
        case ILOC_INS_I2C:
            return "i2c";
        case ILOC_INS_ADD:
            return "add";
        case ILOC_INS_SUB:
            return "sub";
        case ILOC_INS_MULT:
            return "mult";
        case ILOC_INS_DIV:
            return "div";
        case ILOC_INS_ADDI:
            return "addI";
        case ILOC_INS_SUBI:
            return "subI";
        case ILOC_INS_RSUBI:
            return "rsubI";
        case ILOC_INS_MULTI:
            return "multI";
        case ILOC_INS_DIVI:
            return "divI";
        case ILOC_INS_RDIVI:
            return "rdivI";
        case ILOC_INS_LSHIFT:
            return "lshift";
        case ILOC_INS_LSHIFTI:
            return "lshiftI";
        case ILOC_INS_RSHIFT:
            return "rshift";
        case ILOC_INS_RSHIFTI:
            return "rshiftI";
        case ILOC_INS_AND:
            return "and";
        case ILOC_INS_ANDI:
            return "andI";
        case ILOC_INS_OR:
            return "or";
        case ILOC_INS_ORI:
            return "orI";
        case ILOC_INS_XOR:
            return "xor";
        case ILOC_INS_XORI:
            return "xorI";
        case ILOC_INS_LOADAI:
            return "loadAI";
        case ILOC_INS_LOADA0:
            return "loadA0";
        case ILOC_INS_CLOADAI:
            return "cloadAI";
        case ILOC_INS_CLOADA0:
            return "cloadA0";
        case ILOC_INS_STOREAI:
            return "storeAI";
        case ILOC_INS_STOREAO:
            return "storeAO";
        case ILOC_INS_CSTOREAI:
            return "cstoreAI";
        case ILOC_INS_CSTOREAO:
            return "cstoreAO";
        case ILOC_INS_JUMPI:
            return "jumpI";
        case ILOC_INS_JUMP:
            return "jump";
        case ILOC_INS_CBR:
            return "cbr";
        case ILOC_INS_CMP_LT:
            return "cmp_LT";
        case ILOC_INS_CMP_LE:
            return "cmp_LE";
        case ILOC_INS_CMP_EQ:
            return "cmp_EQ";
        case ILOC_INS_CMP_GE:
            return "cmp_GE";
        case ILOC_INS_CMP_GT:
            return "cmp_GT";
        case ILOC_INS_CMP_NE:
            return "cmp_NE";
        default:
            break;
    }

    return "panik";
}