#ifndef STACK_H
#define STACK_H

#include "utils.h"

typedef struct Node 
{
    int data;
    struct Node* prev;
} Node;

typedef struct Stack 
{
    Node* head;
} Stack;

void free_stack(Stack* stack);
Stack* init_stack();
Node* create_node(const int data);
status_code push(Stack* stack, const int data);
int pop(Stack* stack);
bool is_empty(Stack* stack);

#endif