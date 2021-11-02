#include "intermediate_code.h"

int temp_count = 0;
int label_count = 1;
int max_register_count = 0;

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

void allocate_funccall_registers(int register_count)
{
    if (register_count > max_register_count)
    {
        int extra_registers = register_count - max_register_count;
        max_register_count = register_count;

        // only adds the extra registers since the last funccall
        add_disp_symbol_table(REGISTER_SIZE * extra_registers);
    }
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
    disp.number += 4;

    return iloc_join(iloc_join(iloc_create_label(label.number), iloc_create(ILOC_INS_I2I, rsp, rfp, none)), iloc_create(ILOC_INS_ADDI, rsp, disp, rsp));
}

iloc_instruction_t* generate_attribution_from_address(iloc_argument_t reference_address_register1, iloc_argument_t address1, iloc_argument_t reference_address_register2, iloc_argument_t address2)
{
    iloc_argument_t temp = make_temp();
    return iloc_join(iloc_create(ILOC_INS_LOADAI, reference_address_register2, address2, temp), generate_attribution(reference_address_register1, address1, temp));
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

iloc_instruction_t* generate_funccall(token_t* funcname_token, node_t* arguments_node, iloc_argument_t reg)
{
    iloc_argument_t rfp = {ILOC_ARG_TYPE_RFP, 0};
    iloc_argument_t rsp = {ILOC_ARG_TYPE_RSP, 0};
    iloc_argument_t rpc = {ILOC_ARG_TYPE_RPC, 0};
    iloc_argument_t zero = {ILOC_ARG_TYPE_NUMBER, 0};
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};
    iloc_instruction_t* code = NULL;

    node_t *arg_node_iterator = arguments_node;
    while (arg_node_iterator != NULL)
    {
        code = iloc_join(code, arg_node_iterator->code);
        arg_node_iterator = arg_node_iterator->child5;
    }

    // calcula e salva o endereço de retorno
    symbol_item_t* symbol = get_symbol(funcname_token->text);
    int num_parameters = symbol->params_queue == NULL ? 0 : symbol->params_queue->size;
    iloc_argument_t return_address_arg = {ILOC_ARG_TYPE_NUMBER, 6 + num_parameters + temp_count};
    iloc_argument_t return_address = make_temp();
    code = iloc_join(code, iloc_create(ILOC_INS_ADDI, rpc, return_address_arg, return_address));
    code = iloc_join(code, iloc_create(ILOC_INS_STOREAI, return_address, rsp, zero));

    // salva rsp e rfp
    iloc_argument_t four = {ILOC_ARG_TYPE_NUMBER, 4};
    code = iloc_join(code, iloc_create(ILOC_INS_STOREAI, rsp, rsp, four));
    iloc_argument_t eight = {ILOC_ARG_TYPE_NUMBER, 8};
    code = iloc_join(code, iloc_create(ILOC_INS_STOREAI, rfp, rsp, eight));

    // empilha os parâmetros
    arg_node_iterator = arguments_node;
    iloc_argument_t parameter_address_iter = {ILOC_ARG_TYPE_NUMBER, 0};
    int i = 0;
    while (arg_node_iterator != NULL)
    {
        parameter_address_iter.number = CONSTANT_FRAME_SIZE + ARGUMENT_SIZE * i++;
        code = iloc_join(code, iloc_create(ILOC_INS_STOREAI, arg_node_iterator->temp, rsp, parameter_address_iter));

        arg_node_iterator = arg_node_iterator->child5;
    }

    // empilha os registradores
    iloc_argument_t register_iter = { ILOC_ARG_TYPE_NUMBER, 0 };
    iloc_argument_t reg_iter = { ILOC_ARG_TYPE_REGISTER, 0 };
    int arg_number = i;
    for (int j = 0; j < temp_count; j++)
    {
        reg_iter.number = j;
        register_iter.number = CONSTANT_FRAME_SIZE + ARGUMENT_SIZE * arg_number + REGISTER_SIZE * j;
        code = iloc_join(code, iloc_create(ILOC_INS_STOREAI, reg_iter, rsp, register_iter));
    }
    allocate_funccall_registers(temp_count);

    // salta para o inicio da função
    iloc_argument_t func_label = symbol->label;
    code = iloc_join(code, iloc_create(ILOC_INS_JUMPI, func_label, none, none));

    // carrega retorno
    iloc_argument_t minus_four = {ILOC_ARG_TYPE_NUMBER, -4};
    code = iloc_join(code, iloc_create(ILOC_INS_LOADAI, rsp, minus_four, reg));

    // Obtém rsp (SP) salvo
    code = iloc_join(code, iloc_create(ILOC_INS_LOADAI, rfp, four, rsp));
    // Obtém rfp (RFP) salvo
    code = iloc_join(code, iloc_create(ILOC_INS_LOADAI, rfp, eight, rfp));

    // desempilha os registradores
    register_iter.number = 0;
    reg_iter.number = 0;
    for (int j = 0; j < temp_count; j++)
    {
        reg_iter.number = j;
        register_iter.number = CONSTANT_FRAME_SIZE + ARGUMENT_SIZE * arg_number + REGISTER_SIZE * j;
        code = iloc_join(code, iloc_create(ILOC_INS_LOADAI, rsp, register_iter, reg_iter));
    }
    allocate_funccall_registers(temp_count);

    return code;
}

iloc_instruction_t* generate_relational_comparison(iloc_opcode_t opcode, iloc_argument_t register1, iloc_argument_t register2, iloc_argument_t result)
{
    return iloc_create(opcode, register1, register2, result);
}

iloc_instruction_t* generate_if(iloc_argument_t boolean_expression, iloc_instruction_t* true_code, iloc_instruction_t* false_code)
{
    iloc_argument_t true_label = make_label();
    iloc_argument_t false_label = make_label();
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};

    if (false_code == NULL)
    {
        return iloc_join(iloc_join(iloc_create(ILOC_INS_CBR, boolean_expression, true_label, false_label), iloc_join(iloc_create_label(true_label.number), true_code)), iloc_join(iloc_create_label(false_label.number), false_code));
    }
    else
    {
        iloc_argument_t out_label = make_label();
        return iloc_join(iloc_join(iloc_join(iloc_create(ILOC_INS_CBR, boolean_expression, true_label, false_label), iloc_join(iloc_create_label(true_label.number), iloc_join(true_code, iloc_create(ILOC_INS_JUMPI, out_label, none, none)))), iloc_join(iloc_create_label(false_label.number), false_code)), iloc_join(iloc_create_label(out_label.number), iloc_create(ILOC_INS_NOP, none, none, none)));
    }
}

iloc_instruction_t* generate_while(iloc_argument_t boolean_expression, iloc_instruction_t* boolean_code, iloc_instruction_t* code)
{
    iloc_argument_t in_label = make_label();
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};

    return iloc_join(iloc_create_label(in_label.number), iloc_join(boolean_code, generate_if(boolean_expression, iloc_join(code, iloc_create(ILOC_INS_JUMPI, in_label, none, none)), NULL)));
}

iloc_instruction_t* generate_for(iloc_argument_t var_init, iloc_argument_t boolean_expression, iloc_instruction_t* boolean_code, iloc_argument_t var_assignment, iloc_instruction_t* var_assignment_code, iloc_instruction_t* code)
{
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};

    return iloc_join(iloc_create(ILOC_INS_I2I, var_init, boolean_expression, none), generate_while(boolean_expression, iloc_join(iloc_create(ILOC_INS_I2I, var_assignment, boolean_expression, none), boolean_code), iloc_join(code, var_assignment_code)));
}

iloc_instruction_t* generate_not_operator(iloc_argument_t reg)
{
    iloc_argument_t one = { ILOC_ARG_TYPE_NUMBER, 1 };
    // xorI r1, c2 => r3 // r3 = r1 xor c2
    return iloc_create(ILOC_INS_XORI, reg, one, reg);
}

iloc_instruction_t* generate_logic_and_expression(iloc_argument_t register1, iloc_instruction_t* code1, iloc_argument_t register2, iloc_instruction_t* code2, iloc_argument_t result)
{
    iloc_argument_t false_constant = {ILOC_ARG_TYPE_NUMBER, 0};
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};
    
    return iloc_join(code1, generate_if(register1, iloc_join(code2, iloc_create(ILOC_INS_I2I, register2, result, none)), iloc_create(ILOC_INS_LOADI, false_constant, result, none)));
}

iloc_instruction_t* generate_logic_or_expression(iloc_argument_t register1, iloc_instruction_t* code1, iloc_argument_t register2, iloc_instruction_t* code2, iloc_argument_t result)
{
    iloc_argument_t true_constant = {ILOC_ARG_TYPE_NUMBER, 1};
    iloc_argument_t none = {ILOC_ARG_TYPE_NONE, 0};
    
    return iloc_join(code1, generate_if(register1, iloc_create(ILOC_INS_LOADI, true_constant, result, none), iloc_join(code2, iloc_create(ILOC_INS_I2I, register2, result, none))));
}