#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef enum status_code 
{
    SUCCESS,
    ALLOCATE_ERROR,
    INVALID_BRACKETS_BALANCE,
} status_code;

typedef enum tree_node_type
{
    OPERAND,
    OPERATOR,
} tree_node_type;

typedef struct Tree 
{
    union 
    {
        int int_data;
        char char_data;
    };
    tree_node_type type;
    struct Tree* left;
    struct Tree* right;
} Tree;

typedef struct Node 
{
    union 
    {
        int int_data;
        char char_data;
        Tree* tree_ptr_data;
    };
    struct Node* prev;
} Node;

typedef struct Stack 
{
    Node* head;
} Stack;

Tree* insert_tree_int(Tree* left, Tree* right, const int data) 
{
    Tree* root = (Tree*)malloc(sizeof(Tree));
    if (!root) return NULL;
    root->left = left;
    root->right = right;
    root->int_data = data;
    root->type = OPERAND;
    return root;
}

Tree* insert_tree_char(Tree* left, Tree* right, const char data) 
{
    Tree* root = (Tree*)malloc(sizeof(Tree));
    if (!root) return NULL;
    root->left = left;
    root->right = right;
    root->char_data = data;
    root->type = OPERATOR;
    return root;
}

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

Node* create_node_int(const int data) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->int_data = data;
    node->prev = NULL;
    return node;
}

Node* create_node_char(const char data)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->char_data = data;
    node->prev = NULL;
    return node;
}

Node* create_node_tree_ptr(Tree* data) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->tree_ptr_data = data;
    node->prev = NULL;
    return node;
}

status_code push_int(Stack* stack, const int data) 
{
    Node* node = create_node_int(data);
    if (!node) return ALLOCATE_ERROR;
    if (stack->head) 
    {
        node->prev = stack->head;
    }
    stack->head = node;
    return SUCCESS;
}

status_code push_char(Stack* stack, const char data) 
{
    Node* node = create_node_char(data);
    if (!node) return ALLOCATE_ERROR;
    if (stack->head) 
    {
        node->prev = stack->head;
    }
    stack->head = node;
    return SUCCESS;
}

status_code push_tree_ptr(Stack* stack, Tree* data) 
{
    Node* node = create_node_tree_ptr(data);
    if (!node) return ALLOCATE_ERROR;
    if (stack->head) 
    {
        node->prev = stack->head;
    }
    stack->head = node;
    return SUCCESS;
}

int pop_int(Stack* stack) 
{
    const int data = stack->head->int_data;
    Node* node_to_remove = stack->head;
    stack->head = stack->head->prev;
    free(node_to_remove);
    return data;
}

char pop_char(Stack* stack) 
{
    const char data = stack->head->char_data;
    Node* node_to_remove = stack->head;
    stack->head = stack->head->prev;
    free(node_to_remove);
    return data;
}

Tree* pop_tree_ptr(Stack* stack) 
{
    Tree* data = stack->head->tree_ptr_data;
    Node* node_to_remove = stack->head;
    stack->head = stack->head->prev;
    free(node_to_remove);
    return data;
}

bool is_empty(Stack* stack) 
{
    return stack->head == NULL;
}

bool is_operator(const char c) 
{
    return (
        c == '+' ||
        c == '-' ||
        c == '/' ||
        c == '*' ||
        c == '^' ||
        c == '~'
    );
}

int get_priority(const char c) 
{
    switch (c)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '~':
        case '%':
            return 2;
        case '^':
            return 3;
        default:
            return -1;
    }
}

int val_by_char(const char c, const int op1, const int op2) 
{
    return (
        c == '+' ? op1 + op2 :
        c == '-' ? op1 - op2 :
        c == '*' ? op1 * op2 :
        c == '/' ? op1 / op2 :
        c == '^' ? pow(op1, op2):
        op1 % op2
    );
}

/* O(n), n - количество символов в инфиксной форме

выделяем память под постфикс (по необходимости будем реаллочить)
инициализируем стек

итерируемся по всем символам инфикса:
    если пробел => continue

    если цифра => 
        записываем в постфиксную строку
        если цифра - последняя цифра числа, то дописываем пробел

    если открывающаяся скобка => 
        пушим в стек

    если оператор оп1 => 
        если встретили минус, проверяем на унарность и если че то заменяем например на ~
        пока стек не пуст и на верхушке стека лежит оператор оп2 с приоритетом не меньшим чем приоритет оп1
            вынимаем чары из стека и кидаем в строку
        добавляем оп1 в стек

    если закрывающаяся скобка => 
        пока стек не пуст или пока не встретили открывающуюся скобку
            вынимаем чары из стека и кидаем в строку
        если стек кончился раньше чем встретили открыв скобку => ошибка в скобках инфикса

пока стек не пуст => 
    если встретили открывающуюся скобку => ошибка в скобках инфикса
    вынимаем чары из стека в строка

*/

status_code sorting_station(const char* infix, char** postfix) 
{
    const int infix_len = strlen(infix);
    *postfix = (char*)malloc(sizeof(char) * (2 * infix_len + 1));
    if (!*postfix) return ALLOCATE_ERROR;

    Stack* st = init_stack();
    if (!st) 
    {
        free(*postfix);
        return ALLOCATE_ERROR;
    }

    int postfix_index = 0;

    for (int i = 0; i < infix_len; ++i) 
    {
        if (isspace(infix[i])) continue;
        if (isdigit(infix[i])) 
        {
            (*postfix)[postfix_index++] = infix[i];
            if (i != infix_len - 1 && isspace(infix[i + 1])) 
            {
                (*postfix)[postfix_index++] = ' ';
            }
        }
        if (is_operator(infix[i])) 
        {
            const int cur_priority = get_priority(infix[i]);
            while (!is_empty(st) && get_priority(st->head->char_data) >= cur_priority) 
            {
                (*postfix)[postfix_index++] = pop_char(st);
            }
            if (push_char(st, infix[i]) == ALLOCATE_ERROR) 
            {
                free(*postfix);
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
        if (infix[i] == '(') 
        {
            if (push_char(st, '(') == ALLOCATE_ERROR) 
            {
                free(*postfix);
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
        if (infix[i] == ')') 
        {
            bool is_open_bracket = false;
            while (!is_empty(st)) 
            {
                const char val = pop_char(st);
                if (val == '(') 
                {
                    is_open_bracket = true;
                    break;
                }
                (*postfix)[postfix_index++] = val;
            }
            if (!is_open_bracket) 
            {
                free(*postfix);
                free_stack(st);
                return INVALID_BRACKETS_BALANCE;
            }
        }
    }
    
    while (!is_empty(st)) 
    {
        const char val = pop_char(st);
        if (val == '(') 
        {
            free(*postfix);
            free_stack(st);
            return INVALID_BRACKETS_BALANCE;
        }
        (*postfix)[postfix_index++] = val;
    }
    
    return SUCCESS;
}

/* O(n), n - длина постфикса

инициализируем строку, которая будет представлять число и стек

итерируемся по всем символам постфикса
    если пробел => скип

    если число =>
        пока не вышли за границу и пока символ число
            добавляем цифру в строку с числом
        пушим инт в стек и очищаем строку с числом

    если оператор =>
        достаем операнд операнд1 из стека
        если оператор - унарный минус 
            пушим в стек (-1 * оператор)
        иначе 
            достаем второй операнд операнд2
            и пушим в стек результат (операнд2 ? операнд1)

достаем результат из стека

*/

status_code calculate_postfix(const char* postfix, int* total) 
{
    char* number = (char*)malloc(sizeof(char) * 2);
    if (!number) return ALLOCATE_ERROR;

    Stack* st = init_stack();
    if (!st) 
    {
        free(number);
        return ALLOCATE_ERROR;
    }

    const int postfix_len = strlen(postfix);
    int number_size = 0, number_capacity = 2;

    for (int i = 0; i < postfix_len; ++i) 
    {
        if (isspace(postfix[i])) continue;
        if (isdigit(postfix[i])) 
        {
            while (i < postfix_len && isdigit(postfix[i])) 
            {
                number[number_size++] = postfix[i++];
                if (number_size >= number_capacity) 
                {
                    number_capacity *= 2;
                    char* tmp = (char*)realloc(number, sizeof(char) * number_capacity);
                    if (!tmp) 
                    {
                        free(number);
                        free_stack(st);
                        return ALLOCATE_ERROR;
                    }
                    number = tmp;
                }
            }
            --i;
            number[number_size] = '\0';

            free(number);
            number = (char*)malloc(sizeof(char) * 2);
            number_size = 0, number_capacity = 2;

            if (push_int(st, atoi(number)) == ALLOCATE_ERROR) 
            {
                free(number);
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
        if (is_operator(postfix[i])) 
        {
            int res;
            int operand_1 = pop_int(st);
            if (postfix[i] == '~') 
            {
                res = -operand_1;
            }
            else 
            {
                int operand_2 = pop_int(st);
                res = val_by_char(postfix[i], operand_2, operand_1);
            }
            if (push_int(st, res) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
    }
    *total = pop_int(st);

    return SUCCESS;
}

/* O(n), n - длина постфикса 

инициализируем строку, которая будет представлять число и стек

итерируемся по символам постфикса
    если пробел => скип

    если число =>
        пока не вышли за границу и пока символ число
            добавляем цифру в строку с числом
        пушим в стек вершину типа операнд с лефт и райт null и значением полученного числа
        очищаем строку с числом
    
    если оператор =>
        достаем вершину операнд1 из стека
        если оператор - унарный минус 
            пушим в стек вершину типа оператор с лефт = операнд1 и райт null и символом ~
        иначе 
            достаем второй вешину операнд2
            и пушим в стек вершину типа оператор с лефт = операнд2 и райт = операнд1 и символом операции

вынимаем вершину из стека, она будет являться финальным корнем

*/

status_code build_tree(Tree** root, const char* postfix) 
{
    char* number = (char*)malloc(sizeof(char) * 2);
    if (!number) return ALLOCATE_ERROR;

    Stack* st = init_stack();
    if (!st) 
    {
        free(number);
        return ALLOCATE_ERROR;
    }

    int number_size = 0, number_capacity = 2;
    const int postfix_len = strlen(postfix);

    for (int i = 0; i < postfix_len; ++i) 
    {
        if (isspace(postfix[i])) continue;
        if (isdigit(postfix[i])) 
        {
            while (i < postfix_len && isdigit(postfix[i])) 
            {
                number[number_size++] = postfix[i++];
                if (number_size >= number_capacity) 
                {
                    number_capacity *= 2;
                    char* tmp = (char*)realloc(number, sizeof(char) * number_capacity);
                    if (!tmp) 
                    {
                        free(number);
                        free_stack(st);
                        return ALLOCATE_ERROR;
                    }
                    number = tmp;
                }
            }
            number[number_size] = '\0';

            free(number);
            number = (char*)malloc(sizeof(char) * 2);
            number_size = 0, number_capacity = 2;

            Tree* new_node = insert_tree_int(NULL, NULL, atoi(number));
            if (!new_node) 
            {
                free(number);
                free_stack(st);
                return ALLOCATE_ERROR;
            }
            if (push_tree_ptr(st, new_node) == ALLOCATE_ERROR) 
            {
                free(number);
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }
        if (is_operator(postfix[i])) 
        {
            Tree* operand_1 = pop_tree_ptr(st);
            Tree* new_node = NULL;
            if (postfix[i] == '~') 
            {
                new_node = insert_tree_char(operand_1, NULL, postfix[i]);
                if (!new_node) 
                {
                    free(number);
                    free_stack(st);
                    return ALLOCATE_ERROR;
                }
            }
            else 
            {
                Tree* operand_2 = pop_tree_ptr(st);
                new_node = insert_tree_char(operand_2, operand_1, postfix[i]);
                if (!new_node) 
                {
                    free(number);
                    free_stack(st);
                    return ALLOCATE_ERROR;
                }
            }
            if (push_tree_ptr(st, new_node) == ALLOCATE_ERROR) 
            {
                free(number);
                free_stack(st);
                return ALLOCATE_ERROR;
            }
        }        
    }

    *root = pop_tree_ptr(st);

    return SUCCESS;
}

/* O(n), n - количество узлов

Если корень пуст => return 0
Если корень операнд => возвращаем значение операнда
Иначе возвращаем значение в зависимости от операции для левого и правого поддеревьев

*/

int calculate_tree(Tree* root) 
{
    if (!root) return 0;
    if (root->type == OPERAND) return root->int_data;
    return val_by_char(
        root->char_data, 
        calculate_tree(root->left), 
        calculate_tree(root->right)
    );
}

int main() 
{
    const char* infix = "12 - 5 * 10";

    char* postfix = NULL;
    printf("sc: %d\n", sorting_station(infix, &postfix));
    printf("postfix: %s\n", postfix);
    
    int res;
    printf("sc: %d, res: %d\n", calculate_postfix(postfix, &res), res);

    Tree* root = NULL;
    printf("sc: %d\n", build_tree(&root, postfix));

    printf("res: %d\n", calculate_tree(root));
}