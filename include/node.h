#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "token.h"

typedef struct _token_t token_t;
typedef enum _token_type_t token_type_t;

typedef struct _node_t{
    char* label;
    token_t* token;
    size_t num_children;
    struct _node_t** children;
} node_t;

node_t* create_leaf(token_t* _token, token_type_t _token_type);
node_t* create_node(token_t* _token, token_type_t _token_type, int _num_children, node_t* child, ...);
node_t* create_node_with_list(token_t* _label, token_type_t _token_type, int _num_children, node_t** _children);
void delete_node(node_t* _node);

#endif