#ifndef TREE_H
#define TREE_H

#include "utils.h"

typedef struct Tree 
{
    char data;
    struct Tree* left;
    struct Tree* right;
} Tree;

void free_tree(Tree* tree);
status_code insert_tree(Tree** root, const char data, Tree* left, Tree* right);

#endif