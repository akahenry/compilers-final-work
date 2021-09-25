#include "node.h"

node_t* create_leaf(char* _label)
{
    return create_node_with_list(_label, 0, NULL);
}

node_t* create_node_with_list(char* _label, int _numChildren, node_t** _children)
{
    node_t* result = calloc(1, sizeof(node_t));
    result->label = _label;
    result->children = _children;
    result->num_children = _numChildren;

    return result;
}

node_t* create_node(char* _label, node_t* child, ...)
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

    return create_node_with_list(_label, count, children);
}

void delete_node(node_t* _node)
{
    if (_node != NULL)
    {
        if (_node->num_children > 0)
        {
            for (size_t i = 0; i < _node->num_children; i++)
            {
                delete_node(_node->children[i]);
            }
            free(_node->children);
        }
        free(_node);
    }
}