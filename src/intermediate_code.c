#include "intermediate_code.h"

int temp_count = 0;
int label_count = 0;

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