#include "node.h"

node_t* create_leaf(token_t* _token)
{
    return create_node_with_list(_token, _token->text, 0, NULL);
}

node_t* create_node_with_list(token_t* _token, char* _label, int _num_children, node_t** _children)
{
    node_t* result = calloc(1, sizeof(node_t));

    if (_label != NULL)
        result->label = _label;
    else if (_token != NULL)
        result->label = _token->text;
    else
        result->label = NULL;
        
    result->children = _children;
    result->num_children = _num_children;
    result->token = _token;

    return result;
}

node_t* create_node(char* _label, int _num_children, node_t* child, ...)
{
    va_list ap;
    node_t* i;
    node_t** children = NULL;
    int count = 0;

    children = calloc(_num_children, sizeof(node_t*));

    va_start(ap, child); 
    for (i = child, count = 0; count < _num_children; i = va_arg(ap, node_t*), count++)
    {
        children[count] = i;
    }
    va_end(ap);

    return create_node_with_list(NULL, _label, _num_children, children);
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

node_t* link_nodes(node_t* _node_parent, node_t* _node_child)
{
    // REVISAR - linka com o próximo na lista através do ultimo node, que deve ser NULL
    _node_parent->children[_node_parent->num_children-1] = _node_child;
    return _node_parent;
}