#include "tree.h"

void free_tree(Tree* tree) 
{
    if (!tree) return;
    free_tree(tree->left);
    free_tree(tree->right);
    free(tree);
    tree = NULL;
}

status_code insert_tree(Tree** root, const char data, Tree* left, Tree* right) 
{
    *root = (Tree*)malloc(sizeof(Tree));
    if (!*root) return ALLOCATE_ERROR;
    (*root)->data = data;
    (*root)->left = left;
    (*root)->right = right;
    return SUCCESS;
}