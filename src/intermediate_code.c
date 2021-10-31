#include "intermediate_code.h"

int temp_count = 0;
int label_count = 1;

iloc_argument_t make_temp()
{
    iloc_argument_t arg = {ILOC_ARG_TYPE_REGISTER, temp_count};
    temp_count++;
    return arg;
}

iloc_argument_t make_label()
{
    iloc_argument_t arg = {ILOC_ARG_TYPE_LABEL, label_count};
    label_count++;
    return arg;
}

iloc_instruction_t* generate_attribution(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t expression)
{
    iloc_argument_t temp = make_temp();
    iloc_instruction_t* instruction = iloc_create(ILOC_INS_STOREAI, expression, reference_address_register, address);

    return instruction;
}

iloc_instruction_t* generate_attribution_vector(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t offset, iloc_argument_t expression)
{
    iloc_argument_t temp = make_temp();
    iloc_argument_t address_temp = make_temp();
    iloc_instruction_t* address_instruction = iloc_create(ILOC_INS_ADD, address, offset, address_temp);
    iloc_instruction_t* instruction = iloc_create(ILOC_INS_STOREAI, expression, reference_address_register, address_temp);

    return iloc_join(address_instruction, instruction);
}

iloc_instruction_t* generate_arithmetic_binary_expression(iloc_opcode_t opcode, iloc_argument_t register1, iloc_argument_t register2, iloc_argument_t result)
{
    return iloc_create(opcode, register1, register2, result);
}

iloc_instruction_t* generate_load_constant(iloc_argument_t reg, iloc_argument_t constant)
{
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};
    return iloc_create(ILOC_INS_LOADI, constant, reg, none);
}

iloc_instruction_t* generate_arithmetic_unary_expression(token_t *token, iloc_argument_t operand, iloc_argument_t result)
{
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};
    iloc_argument_t zero = {ILOC_ARG_TYPE_NUMBER, 0 };

    if (strcmp(token->text, "-") == 0)
    {
        // result = 0 - operand
        return iloc_create(ILOC_INS_RSUBI, operand, zero, result);
    }
    else if (strcmp(token->text, "+") == 0)
    {
        // copy from operand register to result
        return iloc_create(ILOC_INS_I2I, operand, result, none);
    }
}

iloc_instruction_t* generate_load(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t reg)
{
    return iloc_create(ILOC_INS_LOADAI, reference_address_register, address, reg);
}

iloc_instruction_t* generate_load_vector(iloc_argument_t reference_address_register, iloc_argument_t address, iloc_argument_t offset, iloc_argument_t reg)
{
    iloc_argument_t temp = make_temp();
    iloc_argument_t address_temp = make_temp();
    iloc_instruction_t* address_instruction = iloc_create(address.type == ILOC_ARG_TYPE_NUMBER ? ILOC_INS_ADDI : ILOC_INS_ADD, offset, address, address_temp);
    iloc_instruction_t* instruction = iloc_create(ILOC_INS_LOAD, reference_address_register, address_temp, temp);

    return iloc_join(address_instruction, instruction);
}

iloc_instruction_t* generate_funcdec(iloc_argument_t label, iloc_argument_t disp)
{
    iloc_argument_t rfp = {ILOC_ARG_TYPE_RFP, 0};
    iloc_argument_t rsp = {ILOC_ARG_TYPE_RSP, 0};
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};

    return iloc_join(iloc_join(iloc_create_label(label.number), iloc_create(ILOC_INS_I2I, rsp, rfp, none)), iloc_create(ILOC_INS_ADDI, rsp, disp, rsp));
}

iloc_instruction_t* generate_attribution_from_address(iloc_argument_t reference_address_register, iloc_argument_t address1, iloc_argument_t address2)
{
    iloc_argument_t temp = make_temp();
    return iloc_join(iloc_create(ILOC_INS_LOADAI, reference_address_register, address2, temp), generate_attribution(reference_address_register, address1, temp));
}

iloc_instruction_t* generate_attribution_vector_from_address(iloc_argument_t reference_address_register, iloc_argument_t address1, iloc_argument_t offset, iloc_argument_t address2)
{
    iloc_argument_t temp = make_temp();
    return iloc_join(iloc_create(ILOC_INS_LOADAI, reference_address_register, address2, temp), generate_attribution_vector(reference_address_register, address1, offset, temp));
}

iloc_instruction_t* generate_return(iloc_argument_t expression)
{
    iloc_argument_t rfp = {ILOC_ARG_TYPE_RFP, 0};
    iloc_argument_t rsp = {ILOC_ARG_TYPE_RSP, 0};
    iloc_argument_t address_response = {ILOC_ARG_TYPE_NUMBER, -4};
    iloc_argument_t address_return = make_temp();
    iloc_argument_t zero = {ILOC_ARG_TYPE_NUMBER, 0 };
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};

    return iloc_join(iloc_join(iloc_create(ILOC_INS_STOREAI, expression, rsp, address_response), iloc_create(ILOC_INS_LOADAI, rfp, zero, address_return)), iloc_create(ILOC_INS_JUMP, address_return, none, none));
}

iloc_instruction_t* generate_jump_halt()
{
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};
    return iloc_join(iloc_create_label(0), iloc_create(ILOC_INS_HALT, none, none, none));
}