#include <stdio.h>
#include <stdlib.h>

typedef struct BST
{
    int key;
    struct BST* left;
    struct BST* right;
} BST;


/* O(log(n)), n - количество узлов

Если корень - null => return null

Если key корня > key удаляемого => вызываем рекурсивно от левого поддерева
Если key корня < key удаляемого => вызываем рекурсивно от правого поддерева

Остались ситуации когда key корня равен key удаляемого

    Нет одного из поддеревьев:
        Если нет левого поддерева то итоговым корнем станет правое поддерево, free старый корень
        Если нет правого поддерева то итоговым корнем станет левое поддерево, free старый корень

    Есть есть оба поддерева:
        Обьявим переменные parent = root, child = root->right

        От правого поддерева корня спускаемся влево пока не null, обновляя переменные

        Если после цикла parent != root, то parent->left = child->right
        Иначе parent->right = child->right

        Делаем root->key = child->key

        free(child)
*/

void delete_node(BST** root, const int key) 
{
    if (!root) return root;

    if ((*root)->key > key) 
    {
        delete_node((*root)->left, key);
    }
    else if ((*root)->key < key) 
    {
        delete_node((*root)->right, key);
    }
    else if (!(*root)->left) 
    {
        BST* new_root = (*root)->right;
        free(*root);
        return new_root;
    }
    else if (!(*root)->right) 
    {
        BST* new_root = (*root)->left;
        free(*root);
        return new_root;
    }
    else 
    {
        BST* parent = *root;
        BST* child = parent->right;

        while (child->left) 
        {
            parent = child;
            child = parent->left;
        }

        if (parent != root) 
        {
            parent->left = child->right;
        }
        else
        {
            parent->right = child->right;
        }
        
        (*root)->key = child->key;

        free(child);
    }
}  

int main() {}