/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "assembly.h"
#include "queue.h"

queue_t* q_alloc_ins_asm = NULL;
asm_argument_t main_label;

int cmp_label_count = 0;

int asm_count_digits(int number)
{
    int count = 0;
    do {
        number /= 10;
        ++count;
    } while (number != 0);

    return count;
}

void set_main_label(int number)
{
    asm_argument_t arg = {ASM_ARG_TYPE_IDENTIFIER, 0, number, "main"};
    main_label = arg;
}

char* asm_instruction_string(asm_instruction_t *ins)
{
    const char* opcode = asm_opcode_string(ins);
    char* arg1 = asm_arg_string(ins->arg1);
    char* arg2 = asm_arg_string(ins->arg2);
    char* arg3 = asm_arg_string(ins->arg3);
    char* arg4 = asm_arg_string(ins->arg4);
    char* str = NULL;

    switch (ins->opcode)
    {
        case ASM_INS_HALT:
        case ASM_INS_NOP:
        case ASM_INS_LAHF:
        case ASM_INS_CLTD:
        case ASM_INS_RET:
        case ASM_DATA_SECTION:
        case ASM_TEXT_SECTION:
            str = calloc(strlen(opcode) + 1, sizeof(char));
            sprintf(str, "%s", opcode);
            break;

        case ASM_INS_JMP:
        case ASM_INS_JE:
        case ASM_INS_JNE:
        case ASM_INS_JL:
        case ASM_INS_JG:
        case ASM_INS_JLE:
        case ASM_INS_JGE:
        case ASM_INS_IMUL:
        case ASM_INS_IDIV:
            if (arg2 == NULL)
            {
                str = calloc(strlen(opcode) + strlen(arg1) + 1, sizeof(char));
                sprintf(str, "%s %s", opcode, arg1);
            }
            else if (ins->arg1.type == ASM_ARG_TYPE_IMM && ins->arg2.isReference)
            {
                char* arg1Fixed = &(arg1[1]);
                str = calloc(strlen(opcode) + strlen(arg1Fixed) + strlen(arg2) + 1, sizeof(char));
                sprintf(str, "%s %s%s", opcode, arg1Fixed, arg2);
            }
            else if (ins->arg1.type == ASM_ARG_TYPE_REGISTER && ins->arg2.isReference)
            {
                str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + 1, sizeof(char));
                sprintf(str, "%s %s%s", opcode, arg1, arg2);
            }
            break;

        case ASM_INS_MOV:
        case ASM_INS_MOVL:
        case ASM_INS_ADD:
        case ASM_INS_SUB:
        case ASM_INS_CMP:
        case ASM_INS_AND:
        case ASM_INS_XOR:
            if (arg3 == NULL)
            {
                str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + 1, sizeof(char));
                sprintf(str, "%s %s, %s", opcode, arg1, arg2);
            }
            else
            {                
                if (ins->arg1.type == ASM_ARG_TYPE_IMM && ins->arg2.isReference)
                {
                    char* arg1Fixed = &(arg1[1]);
                    str = calloc(strlen(opcode) + strlen(arg1Fixed) + strlen(arg2) + strlen(arg3) + 1, sizeof(char));
                    sprintf(str, "%s %s%s, %s", opcode, arg1Fixed, arg2, arg3);
                }
                else if (ins->arg2.type == ASM_ARG_TYPE_IMM && ins->arg3.isReference)
                {
                    char* arg2Fixed = &(arg2[1]);
                    str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2Fixed) + strlen(arg3) + 1, sizeof(char));
                    sprintf(str, "%s %s, %s%s", opcode, arg1, arg2Fixed, arg3);
                }
                else if(ins->arg2.isReference)
                {
                    str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + strlen(arg3) + 1, sizeof(char));
                    sprintf(str, "%s %s%s, %s", opcode, arg1, arg2, arg3);
                }
                else if(ins->arg3.isReference)
                {
                    str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2) + strlen(arg3) + 1, sizeof(char));
                    sprintf(str, "%s %s, %s%s", opcode, arg1, arg2, arg3);
                }
            }
            
            break;
        
        case ASM_LABEL:
            if (ins->number == main_label.number)
            {
                str = strdup("main:");
            }
            else
            {
                str = calloc(asm_count_digits(ins->number) + 4, sizeof(char));
                sprintf(str, "L%d: ", ins->number);
            }
            break;
        
        case ASM_LABEL_CMP:
            str = calloc(asm_count_digits(ins->number) + 7, sizeof(char));
            sprintf(str, "LCMP%d: ", ins->number);
            break;

        case ASM_GLOBL:
        case ASM_LONG:;
            char* arg1Fixed = ins->arg1.type == ASM_ARG_TYPE_IMM ? &(arg1[1]) : arg1;
            str = calloc(strlen(opcode) + strlen(arg1Fixed) + 2, sizeof(char));
            sprintf(str, "%s %s", opcode, arg1Fixed);
            break;
        case ASM_SIZE:
        case ASM_TYPE:;
            char* arg2Fixed = ins->arg2.type == ASM_ARG_TYPE_IMM ? &(arg2[1]) : arg2;
            str = calloc(strlen(opcode) + strlen(arg1) + strlen(arg2Fixed) + 4, sizeof(char));
            sprintf(str, "%s %s, %s", opcode, arg1, arg2Fixed);
            break;
        case ASM_IDENTIFIER_LABEL:
            str = calloc(strlen(arg1) + 2, sizeof(char));
            sprintf(str, "%s:", arg1);
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
        case ASM_ARG_TYPE_IDENTIFIER_FUNCTION:
        case ASM_ARG_TYPE_LABEL:
            return "L";
            break;
        case ASM_ARG_TYPE_CMP_LABEL:
            return "LCMP";
            break;
        case ASM_ARG_TYPE_IMM:
            return "$";
            break;
        case ASM_ARG_TYPE_POINTER_RAX:
            return "*";
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
            return "hlt";
        case ASM_INS_MOV:
            return "mov";
        case ASM_INS_MOVL:
            return "movl";
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
        case ASM_INS_JL:
            return "jl";
        case ASM_INS_JG:
            return "jg";
        case ASM_INS_JLE:
            return "jle";
        case ASM_INS_JGE:
            return "jge";
        case ASM_INS_LAHF:
            return "lahf";
        case ASM_INS_AND:
            return "and";
        case ASM_INS_RET:
            return "ret";
        case ASM_INS_XOR:
            return "xor";
        case ASM_GLOBL:
            return ".globl";
        case ASM_TYPE:
            return ".type";
        case ASM_SIZE:
            return ".size";
        case ASM_LONG:
            return ".long";
        case ASM_DATA_SECTION:
            return ".data";
        case ASM_TEXT_SECTION:
            return ".text";
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
    case ASM_ARG_TYPE_RBP:
        string = arg.isReference ? strdup("(\%rbp)") : strdup("\%rbp");        
        break;
    case ASM_ARG_TYPE_RSP:
        string = arg.isReference ? strdup("(\%rsp)") : strdup("\%rsp");        
        break;
    case ASM_ARG_TYPE_RIP:
        string = arg.isReference ? strdup("(\%rip)") : strdup("\%rip");        
        break;
    case ASM_ARG_TYPE_POINTER_RAX:
        string = strdup("*\%rax");
        break;
    case ASM_ARG_TYPE_RAX:
        string = arg.isReference ? strdup("(\%rax)") : strdup("\%rax");
        break;
    case ASM_ARG_TYPE_RBX:
        string = arg.isReference ? strdup("(\%rbx)") : strdup("\%rbx");
        break;
    case ASM_ARG_TYPE_EAX:
        string = arg.isReference ? strdup("(\%eax)") : strdup("\%eax");
        break;
    case ASM_ARG_TYPE_EBX:
        string = arg.isReference ? strdup("(\%ebx)") : strdup("\%ebx");
        break;
    case ASM_ARG_TYPE_IPC:
        string = arg.isReference ? strdup("(\%ipc)") : strdup("\%ipc");
        break;
    case ASM_ARG_TYPE_NONE:
        string = NULL;
        break;
    case ASM_ARG_TYPE_REGISTER:
        if (arg.isReference)
        {
            string = calloc(strlen(asm_prefix_for_argument_type(arg.type)) + asm_count_digits(arg.number) + 3, sizeof(char));
            sprintf(string, "(%s%d)", asm_prefix_for_argument_type(arg.type), arg.number);
        }
        else
        {
            string = calloc(strlen(asm_prefix_for_argument_type(arg.type)) + asm_count_digits(arg.number) + 1, sizeof(char));
            sprintf(string, "%s%d", asm_prefix_for_argument_type(arg.type), arg.number);
        }
        break;
    case ASM_ARG_TYPE_OBJECT:
        string = strdup("@object");
        break;
    case ASM_ARG_TYPE_FUNCTION:
        string = strdup("@function");
        break;
    case ASM_ARG_TYPE_IDENTIFIER:
        string = strdup(arg.identifier);
        break;
    case ASM_ARG_TYPE_LABEL:
        if (arg.number == main_label.number)
        {
            string = strdup("main");
        }
        else
        {
            string = calloc(strlen(asm_prefix_for_argument_type(arg.type)) + asm_count_digits(arg.number) + 1, sizeof(char));
            sprintf(string, "%s%d", asm_prefix_for_argument_type(arg.type), arg.number);
        }
        break;
    
    default:
        
        string = calloc(strlen(asm_prefix_for_argument_type(arg.type)) + asm_count_digits(arg.number) + 1, sizeof(char));
        sprintf(string, "%s%d", asm_prefix_for_argument_type(arg.type), arg.number);
        break;
    }

    return string;
}

asm_instruction_t* asm_create(asm_opcode_t opcode, asm_argument_t arg1, asm_argument_t arg2, asm_argument_t arg3, asm_argument_t arg4)
{
    asm_instruction_t* ins = calloc(1, sizeof(asm_instruction_t));
    ins->opcode = opcode;
    ins->arg1 = arg1;
    ins->arg2 = arg2;
    ins->arg3 = arg3;
    ins->arg4 = arg4;
    ins->previous = NULL;

    if (q_alloc_ins_asm == NULL)
        q_alloc_ins_asm = queue_create();

    queue_push(q_alloc_ins_asm, ins);

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

    if (str != NULL)
    {
        printf("%s\n", str);
        free(str);
    }
    else
    {
        printf("\n");
    }
}

void asm_clean()
{
    if (q_alloc_ins_asm != NULL)
    {
        while (!queue_empty(q_alloc_ins_asm))
        {
            free(queue_pop(q_alloc_ins_asm));
        }
        queue_destroy(q_alloc_ins_asm);
    }
}

asm_instruction_t* asm_create_label(int number)
{
    asm_instruction_t* result = calloc(1, sizeof(asm_instruction_t));
    result->opcode = ASM_LABEL;
    result->number = number;

    return result;
}

asm_instruction_t* asm_create_cmp_label()
{
    cmp_label_count += 1;
    asm_instruction_t* result = calloc(1, sizeof(asm_instruction_t));
    result->opcode = ASM_LABEL_CMP;
    result->number = cmp_label_count;

    return result;
}

asm_argument_t asm_create_label_arg_from_label_ins(asm_instruction_t* ins)
{
    asm_argument_t ret;
    ret.number = ins->number;
    ret.isReference = 0;
    ret.type = ASM_ARG_TYPE_CMP_LABEL;

    return ret;
}