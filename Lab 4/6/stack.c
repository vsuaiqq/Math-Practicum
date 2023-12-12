#include "stack.h"

void free_stack(Stack* stack) 
{
    if (!stack) return;
    if (stack->head) 
    {
        Node* tmp = stack->head;
        Node* tmp_next;
        while (tmp) 
        {
            tmp_next = tmp;
            tmp = tmp->prev;
            tmp_next->prev = NULL;
            free(tmp_next);
            tmp_next = NULL;
        }
    }
    free(stack);
    stack = NULL;
}

Stack* init_stack() 
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->head = NULL;
    return stack;
}

Node* create_node(void* data) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->prev = NULL;
    return node;
}

status_code push(Stack* stack, void* data) 
{
    Node* node = create_node(data);
    if (!node) return ALLOCATE_ERROR;
    if (stack->head) 
    {
        node->prev = stack->head;
    }
    stack->head = node;
    return SUCCESS;
}

void* pop(Stack* stack) 
{
    void* data = stack->head->data;
    Node* node_to_remove = stack->head;
    stack->head = stack->head->prev;
    free(node_to_remove);
    return data;
}

bool is_empty(Stack* stack) 
{
    return stack->head == NULL;
}