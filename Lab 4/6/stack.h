#ifndef STACK_H
#define STACK_H

#include "utils.h"

typedef struct Node 
{
    void* data;
    struct Node* prev;
} Node;

typedef struct Stack 
{
    Node* head;
} Stack;

void free_stack(Stack* stack);
Stack* init_stack();
Node* create_node(void* data);
status_code push(Stack* stack, void* data);
void* pop(Stack* stack);
bool is_empty(Stack* stack);

#endif