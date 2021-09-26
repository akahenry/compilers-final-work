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
    struct _node_t* child1;
    struct _node_t* child2;
    struct _node_t* child3;
    struct _node_t* child4;
} node_t;

node_t* create_leaf(token_t* _token);
node_t *create_node(char *_label, node_t *_child1, node_t *_child2, node_t *_child3, node_t *_child4);
void delete_node(node_t* _node);
node_t* link_nodes(node_t *_node_parent, node_t *_node_child);

#endif