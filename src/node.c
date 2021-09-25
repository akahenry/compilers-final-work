#include "node.h"

node_t* createLeaf(int _id)
{
    return createNodeWithList(_id, 0, NULL);
}

node_t* createNodeWithList(int _id, int _numChildren, node_t** _children)
{
    node_t* result = calloc(1, sizeof(node_t));
    result->id = _id;
    result->children = _children;
    result->numChildren = _numChildren;

    return result;
}

node_t* createNode(int _id, node_t* child, ...)
{
    va_list ap;
    node_t* i;
    node_t** children = NULL;
    int count = 0, length = 0;

    va_start(ap, child); 
    for (i = child; i != NULL; i = va_arg(ap, node_t*))
    {
        length++;
    }
    va_end(ap);

    children = calloc(length, sizeof(node_t*));

    va_start(ap, child); 
    for (i = child, count = 0; i != NULL; i = va_arg(ap, node_t*), count++)
    {
        children[count] = i;
    }
    va_end(ap);

    return createNodeWithList(_id, count, children);
}

void deleteNode(node_t* _node)
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