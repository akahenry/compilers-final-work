/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "utils.h"
#include "iloc_instruction.h"

typedef struct _token_t token_t;

typedef enum _node_type_t {
    NODE_TYPE_INT,
    NODE_TYPE_FLOAT,
    NODE_TYPE_BOOL,
    NODE_TYPE_CHAR,
    NODE_TYPE_STRING
} node_type_t;

typedef struct _node_t{
    char* label;
    node_type_t type;
    token_t* token;
    iloc_instruction_t* code;
    iloc_argument_t temp;
    int disp;
    struct _node_t* child1;
    struct _node_t* child2;
    struct _node_t* child3;
    struct _node_t* child4;
    struct _node_t* child5;
} node_t;

node_t* node_create_leaf(token_t* _token);
node_t *node_create(char *_label, node_t *_child1, node_t *_child2, node_t *_child3, node_t *_child4, node_t *_child5);
void node_destroy(node_t* _node);
node_t* node_link(node_t *_node_parent, node_t *_node_child);
node_type_t infer_type(node_type_t type1, node_type_t type2);
node_type_t infer_type_nodes(node_t *node1, node_t *node2, int line);
node_type_t check_implicit_conversion(node_type_t type1, node_type_t type2, int line, int is_function);
node_type_t type_coercion(node_type_t type1, node_type_t type2);

#endif