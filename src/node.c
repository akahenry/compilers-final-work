/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "node.h"

node_t* node_create_leaf(token_t* _token)
{
    if (_token != NULL && (_token->type == TOKEN_TYPE_LITERAL_STRING ) || _token->type == TOKEN_TYPE_LITERAL_CHAR)
    {
        size_t len = strlen(_token->text);
        if ((_token->text[0] == '"' && _token->text[len - 1] == '"') ||
            (_token->text[0] == '\'' && _token->text[len - 1] == '\''))
        {
            _token->text[len - 1] = '\0';
            memmove(_token->text, _token->text + 1, len - 1);
        }
    }
    node_t* node = node_create(_token->text, NULL, NULL, NULL, NULL, NULL);
    node->token = _token;

    return node;
}

node_t *node_create(char *_label, node_t *_child1, node_t *_child2, node_t *_child3, node_t *_child4, node_t *_child5)
{
    node_t* result = calloc(1, sizeof(node_t));

    if (_label != NULL)
        result->label = strdup(_label);
    else
        result->label = NULL;

    result->child1 = _child1;
    result->child2 = _child2;
    result->child3 = _child3;
    result->child4 = _child4;
    result->child5 = _child5;

    return result;
}

void node_destroy(node_t* _node)
{
    if (_node != NULL)
    {
        if (_node->child1 != NULL)
        {
            node_destroy(_node->child1);
        }
        if (_node->child2 != NULL)
        {
            node_destroy(_node->child2);
        }
        if (_node->child3 != NULL)
        {
            node_destroy(_node->child3);
        }
        if (_node->child4 != NULL)
        {
            node_destroy(_node->child4);
        }
        if (_node->child5 != NULL)
        {
            node_destroy(_node->child5);
        }

        free(_node->label);
        free(_node);
    }
}

node_t* node_link(node_t* _node_parent, node_t* _node_child)
{
    if (_node_parent != NULL)
    {
        // REVISAR - próximo na lista sempre é o child 5
        _node_parent->child5 = _node_child;
    }
    return _node_parent;
}

// type infer according to the specification, for bool, int and float
node_type_t infer_type(node_type_t type1, node_type_t type2)
{
    if (type1 == type2)
    {
        return type1;
    }
    else if (type1 == NODE_TYPE_FLOAT || type2 == NODE_TYPE_FLOAT)
    {
        return NODE_TYPE_FLOAT;
    }
    else
    {
        return NODE_TYPE_INT;
    }
}

// Uses infer_type for nodes, but first checks if you're trying to infer types for char or string
node_type_t infer_type_nodes(node_t *node1, node_t *node2, int line)
{
    if (node1->type != node2->type)
    {
        if (node1->type == NODE_TYPE_STRING || node2->type == NODE_TYPE_STRING)
        {
            fprintf(stderr, "Semantic Error: cannot convert datatype `string` to another type in line %d\n", line);
            exit(ERR_STRING_TO_X);
        }
        else if (node1->type == NODE_TYPE_CHAR || node2->type == NODE_TYPE_CHAR)
        {
            fprintf(stderr, "Semantic Error: cannot convert datatype `char` to another type in line %d\n", line);
            exit(ERR_CHAR_TO_X);
        }
    }

    return infer_type(node1->type, node2->type);
}