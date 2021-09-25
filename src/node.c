#include "node.h"

node_t* create_leaf(token_t* _token, token_type_t _token_type)
{
    return create_node_with_list(_token, _token_type, 0, NULL);
}

node_t* create_node_with_list(token_t* _token, token_type_t _token_type, int _num_children, node_t** _children)
{
    node_t* result = calloc(1, sizeof(node_t));

    if (_token != NULL)
        result->label = _token->text;
    else
        result->label = NULL;
    result->children = _children;
    result->num_children = _num_children;
    result->token = _token;

    return result;
}

node_t* create_node(token_t* _token, token_type_t _token_type, int _num_children, node_t* child, ...)
{
    va_list ap;
    node_t* i;
    node_t** children = NULL;
    int count = 0;

    children = calloc(_num_children, sizeof(node_t*));

    va_start(ap, child); 
    for (i = child, count = 0; i != NULL, count < _num_children; i = va_arg(ap, node_t*), count++)
    {
        children[count] = i;
    }
    va_end(ap);

    return create_node_with_list(_token, _token_type, _num_children, children);
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