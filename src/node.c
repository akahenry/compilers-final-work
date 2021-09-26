#include "node.h"

node_t* create_leaf(token_t* _token)
{
    node_t* node = create_node(_token->text, NULL, NULL, NULL, NULL);
    node->token = _token;

    if (_token != NULL)
        node->label = _token->text;

    return node;
}

node_t *create_node(char *_label, node_t *_child1, node_t *_child2, node_t *_child3, node_t *_child4)
{
    node_t* result = calloc(1, sizeof(node_t));

    if (_label != NULL)
        result->label = _label;
    else
        result->label = NULL;

    result->child1 = _child1;
    result->child2 = _child2;
    result->child3 = _child3;
    result->child4 = _child4;

    return result;
}

void delete_node(node_t* _node)
{
    if (_node != NULL)
    {
        if (_node->child1 != NULL)
        {
            delete_node(_node->child1);
        }
        if (_node->child2 != NULL)
        {
            delete_node(_node->child2);
        }
        if (_node->child3 != NULL)
        {
            delete_node(_node->child3);
        }
        if (_node->child4 != NULL)
        {
            delete_node(_node->child4);
        }

        free(_node);
    }
}

node_t* link_nodes(node_t* _node_parent, node_t* _node_child)
{
    // REVISAR - próximo na lista sempre é o child 4
    _node_parent->child4 = _node_child;
    return _node_parent;
}