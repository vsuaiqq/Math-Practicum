#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum status_code
{
    success,
    allocate_error,
} status_code;

typedef enum char_type 
{
    letter,
    open_bracket,
    close_bracket,
} char_type;

typedef struct Tnode
{
    char data;
    struct Tnode* parent;
    struct Tnode* children;
    struct Tnode* next_brother;
    struct Tnode* prev_brother;
} Tnode;

void free_tree(Tnode* tree_root) 
{
    Tnode* tmp = tree_root;
    if (tmp == NULL) return;
    if (tmp->children != NULL) 
    {
        tmp = tmp->children;
        free(tmp->parent);
        while (tmp->next_brother != NULL) 
        {
            tmp = tmp->next_brother;
            free_tree(tmp->prev_brother);
        }
        free(tmp);
    }
}

void print_tree_to_file(Tnode* tree_root, FILE* output_file, const int depth)  
{
    if (tree_root == NULL) return;

    for (int i = 0; i < depth; ++i) 
    {
        fprintf(output_file, "  ");
    }
    fprintf(output_file, "%c\n", tree_root->data);

    if (tree_root->children != NULL) 
    {
        print_tree_to_file(tree_root->children, output_file, depth + 1);
    }
    if (tree_root->next_brother != NULL) 
    {
        print_tree_to_file(tree_root->next_brother, output_file, depth);
    }
}

status_code insert_child(Tnode* parent_node, const char value) 
{
    Tnode* new_node = (Tnode*)malloc(sizeof(Tnode));
    if (new_node == NULL) return allocate_error;

    new_node->data = value;
    new_node->parent = parent_node;
    new_node->children = NULL;
    new_node->next_brother = NULL;
    new_node->prev_brother = NULL;
    parent_node->children = new_node;

    return success;
}

status_code insert_brother(Tnode* prev_brother_node, const char value) 
{
    Tnode* new_node = (Tnode*)malloc(sizeof(Tnode));
    if (new_node == NULL) return allocate_error;

    new_node->data = value;
    new_node->parent = prev_brother_node->parent;
    new_node->children = NULL;
    new_node->next_brother = NULL;
    new_node->prev_brother = prev_brother_node;
    prev_brother_node->next_brother = new_node;

    return success;
}

status_code create_tree_by_str(Tnode** tree_root, const char* expression_str) 
{
    *tree_root = (Tnode*)malloc(sizeof(Tnode));
    if (*tree_root == NULL) return allocate_error;
    (*tree_root)->data = expression_str[0];
    (*tree_root)->parent = NULL;
    (*tree_root)->children = NULL;
    (*tree_root)->next_brother = NULL;
    (*tree_root)->prev_brother = NULL;

    Tnode* tmp = *tree_root;
    char_type last_char_type = letter;
    for (int i = 1; i < strlen(expression_str); ++i)
    {
        const char cur_symbol = expression_str[i];
        if (isalpha(cur_symbol)) 
        {
            status_code status;
            switch (last_char_type)
            {
                case letter:
                    while (tmp->next_brother != NULL) 
                    {
                        tmp = tmp->next_brother;
                    }
                    status = insert_brother(tmp, cur_symbol);
                    if (status == allocate_error) 
                    {
                        free_tree(*tree_root);
                        return allocate_error;
                    }
                    tmp = tmp->next_brother;
                    break;

                case open_bracket:
                    status = insert_child(tmp, cur_symbol);
                    if (status == allocate_error) 
                    {
                        free_tree(*tree_root);
                        return allocate_error;
                    }
                    tmp = tmp->children;
                    break;          

                case close_bracket:
                    while (tmp->next_brother != NULL) 
                    {
                        tmp = tmp->next_brother;
                    }
                    status_code status = insert_brother(tmp, cur_symbol);
                    if (status == allocate_error) 
                    {
                        free_tree(*tree_root);
                        return allocate_error;
                    }
                    tmp = tmp->next_brother;
                    break;
            }
            last_char_type = letter;
        }
        else if (cur_symbol == '(') 
        {
            last_char_type = open_bracket;
        }
        else if (cur_symbol == ')') 
        {
            tmp = tmp->parent;
            last_char_type = close_bracket;
        }
    }
    
    return success;
}

status_code solve(FILE* input_file, FILE* output_file) 
{
    char* expression = NULL;
    int getline_res;
    size_t buff_size = 0;
    while ((getline_res = getline(&expression, &buff_size, input_file)) != -1) 
    {
        if (getline_res == -1) 
        {
            free(expression);
            return success;
        }

        Tnode* tree_root;
        switch (create_tree_by_str(&tree_root, expression))
        {
            case success:
                break;
            case allocate_error:
                return allocate_error;
        }
        
        fprintf(output_file, "--------------------------\n");
        print_tree_to_file(tree_root, output_file, 1);
        fprintf(output_file, "--------------------------\n");

        free_tree(tree_root);
        free(expression);
        expression = NULL;
    }

    free(expression);
    return success;
}

int main(int argc, char* argv[]) 
{
    if (argc < 3) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    
    if (input_file == NULL || output_file == NULL) 
    {
        printf("file error detected!\n");
        return 1;
    }

    switch (solve(input_file, output_file))
    {
        case success:
            break;
        case allocate_error:
            printf("allocate error detected!\n");
            return 1;
    }
    
    fclose(input_file);
    fclose(output_file);
    
    return 0;
}