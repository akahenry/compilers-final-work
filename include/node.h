#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "token.h"

typedef struct _token_t token_t;

typedef struct _node_t{
    char* label;
    token_t* token;
    size_t num_children;
    struct _node_t** children;
} node_t;

node_t* create_leaf(token_t* _token);
node_t* create_node(token_t* _token, int _num_children, node_t* child, ...);
node_t* create_node_with_list(token_t* _label, int _num_children, node_t** _children);
void delete_node(node_t* _node);
node_t* link_nodes(node_t *_node_parent, node_t *_node_child);

#endif