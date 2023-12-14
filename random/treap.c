#include <stdio.h>
#include <stdlib.h>

typedef struct TreapNode 
{
    int key;
    int priority;
    struct TreapNode* left;
    struct TreapNode* right;
} TreapNode;

void free_treap(TreapNode* root) 
{
    if (!root) return;
    free_treap(root->left);
    free_treap(root->right);
    free(root);
    root = NULL;
}

TreapNode* create_treapnode(const int key) 
{
    TreapNode* new_node = (TreapNode*)malloc(sizeof(TreapNode));
    if (!new_node) return NULL;
    new_node->key = key;
    new_node->priority = rand();
    new_node->left = NULL, new_node->right = NULL;
    return new_node;
}

void merge(TreapNode** res, TreapNode* left, TreapNode* right) 
{
    if (!left || !right) *res = left ? left : right;
    else if (left->priority > right->priority) 
    {
        *res = left;
        merge(&((*res)->right), left->right, right);
    }
    else 
    {
        *res = right;
        merge(&((*res)->left), left, right->left);
    }
}

void split(TreapNode* root, TreapNode** left, TreapNode** right, const int key) 
{
    if (!root) *left = NULL, *right = NULL;
    else if (root->key > key) 
    {
        *right = root;
        split(root->left, left, &((*right)->left), key);
    }
    else 
    {
        *left = root;
        split(root->right, &((*left)->right), right, key);
    }
}

void insert(TreapNode** root, TreapNode* new_node) 
{
    if (!*root) 
    {
        *root = new_node;
    }
    else if ((*root)->priority > new_node->priority) 
    {
        insert((*root)->key > new_node->key ? &((*root)->left) : &((*root)->right), new_node);
    }
    else 
    {
        split(*root, &(new_node->left), &(new_node->right), new_node->key);
        *root = new_node;
    }
}

void erase(TreapNode** root, const int key) 
{
    if ((*root)->key == key) 
    {
        merge(root, (*root)->left, (*root)->right);
    }
    else 
    {
        erase((*root)->key > key ? &((*root)->left) : &((*root)->right), key);
    }
}

void inorder_traverse(TreapNode* root) 
{
    if (!root) return;
    inorder_traverse(root->left);
    printf("%d ", root->key);
    inorder_traverse(root->right);
}

TreapNode* search_treapnode(TreapNode* root, const int key) 
{
    if (!root || root->key == key) return root;
    if (root->key > key) 
    {
        return search_treapnode(root->left, key);
    }
    return search_treapnode(root->right, key);
}

int main() 
{
    TreapNode* root = NULL;
    TreapNode* node1 = create_treapnode(12);
    insert(&root, node1);
    TreapNode* node2 = create_treapnode(3);
    insert(&root, node2);
    TreapNode* node3 = create_treapnode(434);
    insert(&root, node3);
    inorder_traverse(root);
    printf("\n");
}