#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct _node_t{
    int id;
    char* label;
    size_t numChildren;
    struct _node_t** children;
} node_t;

node_t* createLeaf(int _id);
node_t* createNode(int _id, node_t* child, ...);
node_t* createNodeWithList(int _id, int _numChildren, node_t** _children);\
void deleteNode(node_t* _node);

#endif