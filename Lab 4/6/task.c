#include "task.h"

status_code to_postfix(char** postfix, const char* infix) 
{
    const int infix_len = strlen(infix);
    *postfix = (char*)malloc(sizeof(char) * infix_len);
    if (!*postfix) return ALLOCATE_ERROR;
    int postfix_len = 0;

    Stack* st = init_stack();
    if (!st) 
    {
        free(*postfix);
        return ALLOCATE_ERROR;
    }

    for (int i = 0; i < infix_len; ++i) 
    {
        if (isspace(infix[i])) continue;
        if (isalpha(infix[i]) || is_bool_value(infix[i])) 
        {
            (*postfix)[postfix_len] = infix[i];
            ++postfix_len;
        }
        else 
        {
            if (infix[i] == ')') 
            {
                while (true) 
                {
                    if (is_empty(st)) 
                    {
                        free(*postfix);
                        free_stack(st);
                        return INVALID_BRACKETS_BALANCE;
                    }
                    const char val = *(char*)pop(st);
                    if (val == '(') break;
                    (*postfix)[postfix_len] = val;
                    ++postfix_len;
                }
            }
            else if (infix[i] == '(')
            {
                if (push(st, (void*)&infix[i]) == ALLOCATE_ERROR) 
                {
                    free(*postfix);
                    free_stack(st);
                    return ALLOCATE_ERROR;
                }
            }
            else 
            {
                if (infix[i] == '+' || infix[i] == '-' || infix[i] == '<') 
                {
                    if (infix[i + 1] != '>') return UNEXPECTED_CHARACTER;
                }
                if (infix[i] == '>') 
                {
                    if (infix[i - 1] != '+' && infix[i - 1] != '-' && infix[i - 1] != '<') return UNEXPECTED_CHARACTER;
                    continue;
                }
                const int cur_priority = get_priority(infix[i]);
                if (cur_priority == -1) 
                {
                    free_stack(st);
                    free(*postfix);
                    return UNEXPECTED_CHARACTER;
                }
                while (!is_empty(st) && get_priority(*(char*)(st->head)) > cur_priority) 
                {
                    if (is_empty(st)) break;
                    const char val = *(char*)pop(st);
                    if (val == '(') break;
                    (*postfix)[postfix_len] = val;
                    ++postfix_len;
                }
                if (push(st, (void*)&infix[i]) == ALLOCATE_ERROR) 
                {
                    free_stack(st);
                    free(*postfix);
                    return ALLOCATE_ERROR;
                }
            }
        }
    }
    while (!is_empty(st)) 
    {
        const char val = *(char *)pop(st);
        if (val == '(') 
        {
            free_stack(st);
            free(*postfix);
            return INVALID_BRACKETS_BALANCE;
        }
        (*postfix)[postfix_len] = val;
        ++postfix_len;
    }
    (*postfix)[postfix_len] = '\0';

    free_stack(st);
    return SUCCESS;
}

status_code make_tree(Tree** root, const char* postfix) 
{
    Stack* st = init_stack();
    if (!st) return ALLOCATE_ERROR;

    for (int i = 0; i < strlen(postfix); ++i) 
    {
        if (isalpha(postfix[i]) || is_bool_value(postfix[i])) 
        {
            Tree* node = NULL;
            if (insert_tree(&node, postfix[i], NULL, NULL) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                return ALLOCATE_ERROR;
            }
            if (push(st, (void*)node) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
        else 
        {
            if (is_empty(st)) 
            {
                free_stack(st);
                return UNUSED_CHARACTER;
            }
            Tree* fst_node = (Tree*)pop(st);
            Tree* res = NULL;
            if (postfix[i] == '~') 
            {
                if (insert_tree(&res, postfix[i], fst_node, NULL) == ALLOCATE_ERROR) 
                {
                    free_stack(st);
                    return ALLOCATE_ERROR;
                }
            }
            else 
            {
                if (is_empty(st)) 
                {
                    free_stack(st);
                    return UNEXPECTED_CHARACTER;
                }
                if (insert_tree(&res, postfix[i], (Tree*)pop(st), fst_node) == ALLOCATE_ERROR) 
                {
                    free_stack(st);
                    return ALLOCATE_ERROR;
                }
            }
            if (push(st, (Tree*)res) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
    }

    if (is_empty(st)) 
    {
        free_stack(st);
        return UNUSED_CHARACTER;
    }

    *root = (Tree*)pop(st);

    if (!is_empty(st)) 
    {
        free_stack(st);
        return UNUSED_CHARACTER;
    }

    return SUCCESS;
}

int calculate_tree(Tree *root, char* vars, int* vars_values, const int num_of_vars) 
{
    if (!root) return 0;
    if (isalpha(root->data)) 
    {
        for (int i = 0; i < num_of_vars; ++i) 
        {
            if (root->data == vars[i]) return vars_values[i];
        }
    }
    if (is_bool_value(root->data)) return root->data - '0';
    return res_by_char(
        calculate_tree(root->left, vars, vars_values, num_of_vars), 
        calculate_tree(root->right, vars, vars_values, num_of_vars), 
        root->data
    );
}

void make_table(FILE* stream, char* vars, const int vars_size, const char* infix, Tree* root)  
{
    print_info(stream, vars, vars_size, infix);
    int vars_values[vars_size];
    const int margin = strlen(infix) / 2 + 1;
    for (long long int i = 0; i < get_binary_permutations(vars_size); ++i) 
    {
        for (int j = 0; j < vars_size; ++j) vars_values[j] = get_binary_value(i, j);
        print_line(
            stream, 
            vars_values, 
            vars_size, 
            calculate_tree(root, vars, vars_values, vars_size), margin
        );
    }
}

status_code solve(const char* input_path) 
{
    status_code st;

    FILE *input = fopen(input_path, "r");
    if (!input) return FILE_ERROR;

    char* infix = NULL;
    getline(&infix, &(size_t){0}, input);
    if (is_empty_str(infix)) 
    {
        fclose(input);
        return EMPTY_STRING;
    }

    fclose(input);

    char* postfix = NULL;
    st = to_postfix(&postfix, infix);
    if (st != SUCCESS) 
    {
        free(infix);
        return st;
    }

    Tree* root = NULL;
    st = make_tree(&root, postfix);
    if (st != SUCCESS) 
    {
        free_tree(root);
        free(infix);
        free(postfix);
        return st;
    }

    free(postfix);

    char* output_path = get_rand_filename();
    if (!output_path) 
    {
        free_tree(root);
        free(infix);
        return ALLOCATE_ERROR;
    }
    FILE* output = fopen(output_path, "w");
    if (!output) 
    {
        free_tree(root);
        free(infix);
        free(output_path);
        return FILE_ERROR;
    }

    free(output_path);

    char* vars = NULL;
    int vars_size;
    st = get_vars_from_expr(&vars, &vars_size, infix);
    if (st != SUCCESS) 
    {
        fclose(output);
        free_tree(root);
        free(infix);
        return st;
    }

    make_table(output, vars, vars_size, infix, root);

    free(infix);
    free(vars);
    free_tree(root);
    fclose(output);

    return SUCCESS;
}