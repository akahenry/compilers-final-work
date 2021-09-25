#include "utils.h"

queue_t* q_alloc_token = NULL;

int char_to_bool(char* str)
{
    if(!strcmp(str, "false"))
        return 0;
    else if(!strcmp(str, "true"))
        return 1;
    else
        return -1;	
}

void create_token_queue()
{
    if (q_alloc_token == NULL)
        q_alloc_token = start_queue();
}

void push_token_queue(void* token)
{
    queue_push(q_alloc_token, token);
}

void clear_token_queue()
{
    free(q_alloc_token);
}

void exporta(void* tree)
{
    if (tree == NULL)
    {
        return;
    }
    
    node_t* node_ptr = (node_t*) tree;
    printf("%p [label=\"%s\"];\n", node_ptr, node_ptr->label); // #TODO: add label

    for (int i = 0; i < node_ptr->num_children; i++)
    {
        printf("%p, %p\n", node_ptr, node_ptr->children[i]);
        exporta(node_ptr->children[i]);
    }
}

void libera(void *tree)
{
    node_t* node_ptr = (node_t*) tree;
    delete_node(node_ptr);

    while(!is_empty_queue(q_alloc_token)) {
        queue_item_t* item = (queue_item_t*)queue_pop(q_alloc_token);

        free(((token_t*)item->value)->value.v_string);
        free(item->value);
        free(item);
    }
    clear_queue(q_alloc_token);
}