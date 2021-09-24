#include <stdio.h>
#include <stdlib.h>

typedef struct _node{
    int id;
    char* label;
    size_t numChildren;
    struct _node** children;
} node;

node* createLeaf(int _id);
node* createNode(int _id, int _numChildren, node** _children);
void deleteNode(node* _node);