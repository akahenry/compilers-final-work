/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "iloc_instruction.h"
#include "queue.h"

queue_t* q_alloc_instructions = NULL;

int count_digits(int number)
{
    int count = 0;
    do {
        number /= 10;
        ++count;
    } while (number != 0);

    return count;
}

char* iloc_instruction_string(iloc_instruction_t *ins)
{
    const char* opcode = iloc_opcode_string(ins);
    char* arg1 = iloc_arg_string(ins->arg1);
    char* arg2 = iloc_arg_string(ins->arg2);
    char* arg3 = iloc_arg_string(ins->arg3);
    char* str = NULL;

    switch (ins->opcode)
    {
        case ILOC_INS_HALT:
        case ILOC_INS_NOP:
            str = calloc(strlen(opcode) + 1, sizeof(char));
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
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + 5 + 1, sizeof(char));
            sprintf(str, "%s %s => %s",
                    opcode,
                    arg1,
                    arg2);
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
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + strlen(arg3) + 7 + 1, sizeof(char));
            sprintf(str, "%s %s, %s => %s",
                    opcode,
                    arg1,
                    arg2,
                    arg3);
            break;
        
        case ILOC_INS_STOREAI:
        case ILOC_INS_STOREAO:
        case ILOC_INS_CSTOREAI:
        case ILOC_INS_CSTOREAO:
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + strlen(arg3) + 7 + 1, sizeof(char));
            sprintf(str, "%s %s => %s, %s",
                    opcode,
                    arg1,
                    arg2,
                    arg3);
            break;
        
        case ILOC_INS_JUMPI:
        case ILOC_INS_JUMP:
            str = calloc(strlen(opcode) + strlen(arg1) + 4 + 1, sizeof(char));
            sprintf(str, "%s -> %s",
                    opcode,
                    arg1);
            break;
        
        case ILOC_INS_CBR:
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + strlen(arg3) + 7 + 1, sizeof(char));
            sprintf(str, "%s %s -> %s, %s",
                    opcode,
                    arg1,
                    arg2,
                    arg3);
            break;
        
        case ILOC_INS_CMP_LT:
        case ILOC_INS_CMP_LE:
        case ILOC_INS_CMP_EQ:
        case ILOC_INS_CMP_GE:
        case ILOC_INS_CMP_GT:
        case ILOC_INS_CMP_NE:
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + strlen(arg3) + 7 + 1, sizeof(char));
            sprintf(str, "%s %s, %s -> %s",
                    opcode,
                    arg1,
                    arg2,
                    arg3);
            break;
        case ILOC_LABEL:
            str = calloc(count_digits(ins->number) + 4, sizeof(char));
            sprintf(str, "L%d: ", ins->number);
            break;

        case ILOC_NO_CODE_RET_VALUE:
        case ILOC_NO_CODE_RET:
            str = strdup("");
            break;
        
        default:
            break;
    }

    free(arg1);
    free(arg2);
    free(arg3);
    return str;
}

const char* iloc_prefix_for_argument_type(iloc_arg_type_t type)
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

const char* iloc_opcode_string(iloc_instruction_t *ins)
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
        case ILOC_INS_HALT:
            return "halt";
        default:
            break;
    }

    return "panik";
}

char* iloc_arg_string(iloc_argument_t arg)
{
    char* string = NULL;

    switch (arg.type)
    {
    case ILOC_ARG_TYPE_RFP:
        string = strdup("rfp");        
        break;
    case ILOC_ARG_TYPE_RSP:
        string = strdup("rsp");
        break;
    case ILOC_ARG_TYPE_RBSS:
        string = strdup("rbss");
        break;
    case ILOC_ARG_TYPE_RPC:
        string = strdup("rpc");
        break;
    case ILOC_ARG_TYPE_NONE:
        string = NULL;
        break;
    
    default:
        
        string = calloc(strlen(iloc_prefix_for_argument_type(arg.type)) + count_digits(arg.number) + 1, sizeof(char));
        sprintf(string, "%s%d", iloc_prefix_for_argument_type(arg.type), arg.number);
        break;
    }

    return string;
}

iloc_instruction_t* iloc_create(iloc_opcode_t opcode, iloc_argument_t arg1, iloc_argument_t arg2, iloc_argument_t arg3)
{
    iloc_instruction_t* ins = calloc(1, sizeof(iloc_instruction_t));
    ins->opcode = opcode;
    ins->arg1 = arg1;
    ins->arg2 = arg2;
    ins->arg3 = arg3;
    ins->previous = NULL;

    if (q_alloc_instructions == NULL)
        q_alloc_instructions = queue_create();

    queue_push(q_alloc_instructions, ins);

    return ins;
}

iloc_instruction_t* iloc_join(iloc_instruction_t* ins1, iloc_instruction_t* ins2)
{
    if (!ins1)
        return ins2;
    if (!ins2)
        return ins1;

    iloc_instruction_t* ins = ins2;
    while (ins->previous)
        ins = ins->previous;

    ins->previous = ins1;

    return ins2;
}

void iloc_recursive_print(iloc_instruction_t* ins)
{
    if (ins == NULL)
    {
        return;
    }

    iloc_recursive_print(ins->previous);
    char* str = iloc_instruction_string(ins);

    printf("%s\n", str);
    free(str);
}

void iloc_clean()
{
    if (q_alloc_instructions != NULL)
    {
        while (!queue_empty(q_alloc_instructions))
        {
            free(queue_pop(q_alloc_instructions));
        }
        queue_destroy(q_alloc_instructions);
    }
}

iloc_instruction_t* iloc_create_label(int number)
{
    iloc_instruction_t* result = calloc(1, sizeof(iloc_instruction_t));
    result->opcode = ILOC_LABEL;
    result->number = number;

    return result;
}