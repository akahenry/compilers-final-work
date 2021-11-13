/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "assembly.h"
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

char* asm_instruction_string(asm_instruction_t *ins)
{
    const char* opcode = asm_opcode_string(ins);
    char* arg1 = asm_arg_string(ins->arg1);
    char* arg2 = asm_arg_string(ins->arg2);
    char* arg3 = asm_arg_string(ins->arg3);
    char* str = NULL;

    switch (ins->opcode)
    {
        case ASM_INS_HALT:
        case ASM_INS_NOP:
        case ASM_INS_LAHF:
        case ASM_INS_CLTD:
            str = calloc(strlen(opcode) + 1, sizeof(char));
            sprintf(str, "%s", opcode);
            break;

        case ASM_INS_JMP:
        case ASM_INS_JE:
        case ASM_INS_JNE:
        case ASM_INS_IMUL:
        case ASM_INS_IDIV:
            str = calloc(strlen(opcode) + strlen(arg1) + 1, sizeof(char));
            sprintf(str, "%s %s", opcode, arg1);
            break;

        case ASM_INS_MOV:
        case ASM_INS_ADD:
        case ASM_INS_SUB:
        case ASM_INS_CMP:
        case ASM_INS_AND:
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + 1, sizeof(char));
            sprintf(str, "%s %s, %s", opcode, arg1, arg2);
            break;
        
        case ASM_LABEL:
            str = calloc(count_digits(ins->number) + 4, sizeof(char));
            sprintf(str, "L%d: ", ins->number);
            break;
        
        default:
            break;
    }

    free(arg1);
    free(arg2);
    free(arg3);
    return str;
}

const char* asm_prefix_for_argument_type(asm_arg_type_t type)
{
    switch (type)
    {
        case ASM_ARG_TYPE_REGISTER:
            return "r";
            break;
        case ASM_ARG_TYPE_LABEL:
            return "L";
            break;
        case ASM_ARG_TYPE_NUMBER:
            return "$";
            break;
        // anything else returns an empty prefix
        default:
            return "";
            break;
    }
}

const char* asm_opcode_string(asm_instruction_t *ins)
{
    switch (ins->opcode)
    {
        case ASM_INS_NOP:
            return "nop";
        case ASM_INS_HALT:
            return "halt";
        case ASM_INS_MOV:
            return "mov";
        case ASM_INS_ADD:
            return "add";
        case ASM_INS_SUB:
            return "sub";
        case ASM_INS_IMUL:
            return "imul";
        case ASM_INS_CLTD:
            return "cltd";
        case ASM_INS_IDIV:
            return "idiv";
        case ASM_INS_JMP:
            return "jmp";
        case ASM_INS_CMP:
            return "cmp";
        case ASM_INS_JE:
            return "je";
        case ASM_INS_JNE:
            return "jne";
        case ASM_INS_LAHF:
            return "lahf";
        case ASM_INS_AND:
            return "and";
        default:
            break;
    }

    return "panik";
}

char* asm_arg_string(asm_argument_t arg)
{
    char* string = NULL;

    switch (arg.type)
    {
    case ASM_ARG_TYPE_EBP:
        string = arg.isReference ? strdup("(ebp)") : strdup("ebp");        
        break;
    case ASM_ARG_TYPE_EAX:
        string = arg.isReference ? strdup("(eax)") : strdup("eax");
        break;
    case ASM_ARG_TYPE_EBX:
        string = arg.isReference ? strdup("(ebx)") : strdup("ebx");
        break;
    case ASM_ARG_TYPE_IPC:
        string = arg.isReference ? strdup("(ipc)") : strdup("ipc");
        break;
    case ASM_ARG_TYPE_NONE:
        string = NULL;
        break;
    
    default:
        
        string = calloc(strlen(asm_prefix_for_argument_type(arg.type)) + count_digits(arg.number) + 1, sizeof(char));
        sprintf(string, "%s%d", asm_prefix_for_argument_type(arg.type), arg.number);
        break;
    }

    return string;
}

asm_instruction_t* asm_create(asm_opcode_t opcode, asm_argument_t arg1, asm_argument_t arg2, asm_argument_t arg3)
{
    asm_instruction_t* ins = calloc(1, sizeof(asm_instruction_t));
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

asm_instruction_t* asm_join(asm_instruction_t* ins1, asm_instruction_t* ins2)
{
    if (!ins1)
        return ins2;
    if (!ins2)
        return ins1;

    asm_instruction_t* ins = ins2;
    while (ins->previous)
        ins = ins->previous;

    ins->previous = ins1;

    return ins2;
}

void asm_recursive_print(asm_instruction_t* ins)
{
    if (ins == NULL)
    {
        return;
    }

    asm_recursive_print(ins->previous);
    char* str = asm_instruction_string(ins);

    printf("%s\n", str);
    free(str);
}

void asm_clean()
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

asm_instruction_t* asm_create_label(int number)
{
    asm_instruction_t* result = calloc(1, sizeof(asm_instruction_t));
    result->opcode = ASM_LABEL;
    result->number = number;

    return result;
}