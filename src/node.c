#include "node.h"

node* createLeaf(int _id)
{
    return createNode(_id, 0, NULL);
}

node* createNode(int _id, int _numChildren, node** _children)
{
    node* result = malloc(sizeof(node));
    result->id = _id;
    result->children = _children;
    result->numChildren = _numChildren;

    return result;
}

void deleteNode(node* _node)
{
    if (_node != NULL)
    {
        if (_node->numChildren > 0)
        {
            for (size_t i = 0; i < _node->numChildren; i++)
            {
                deleteNode(_node->children[i]);
            }
        }
        free(_node);
    }
}