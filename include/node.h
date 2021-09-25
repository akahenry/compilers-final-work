#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct _node_t{
    char* label;
    size_t numChildren;
    struct _node_t** children;
} node_t;

node_t* createLeaf(char* _label);
node_t* createNode(char* _label, node_t* child, ...);
node_t* createNodeWithList(char* _label, int _numChildren, node_t** _children);\
void deleteNode(node_t* _node);

#endif