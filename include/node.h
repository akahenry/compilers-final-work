#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct _node_t{
    char* label;
    size_t num_children;
    struct _node_t** children;
} node_t;

node_t* create_leaf(char* _label);
node_t* create_node(char* _label, int _num_children, node_t* child, ...);
node_t* create_node_with_list(char* _label, int _num_children, node_t** _children);\
void delete_node(node_t* _node);

#endif