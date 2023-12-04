#include "task.h"

status_code to_postfix(char** postfix, const char* infix)
{
    const int infix_len = strlen(infix);
    (*postfix) = (char*)malloc(sizeof(char) * infix_len);
    if (!*postfix) return ALLOCATE_ERROR;
    int postfix_len = 0;

    Stack* st = init_stack();
    if (!st) 
    {
        free(*postfix);
        return ALLOCATE_ERROR;
    }

    int depth = 0;
    bool is_digit_inside = false;

    for (int i = 0; i < infix_len; ++i) 
    {
        if (isspace(infix[i])) continue;
        if (!is_operator(infix[i]) && !is_bracket(infix[i])) 
        {
            (*postfix)[postfix_len] = infix[i];
            ++postfix_len;
            if (is_operator(infix[i + 1]) || is_bracket(infix[i + 1]) || isspace(infix[i + 1])) 
            {
                (*postfix)[postfix_len] = ' ';
                ++postfix_len;
            }
            is_digit_inside = depth ? true : false;
        }
        else if (infix[i] == ')') 
        {
            bool operator_flag = false;
            --depth;
            while (true) 
            {
                if (is_empty(st)) 
                {
                    free_stack(st);
                    free(*postfix);
                    return INVALID_BRACKETS_BALANCE;
                }
                const int val = pop(st);
                if (val == '(') break;
                operator_flag = true;
                (*postfix)[postfix_len] = val;
                ++postfix_len;
            }
            if (!operator_flag && !is_digit_inside) 
            {
                free_stack(st);
                free(*postfix);
                return EMPTY_SPACE_INSIDE_BRACKETS;
            }
        }
        else if (infix[i] == '(') 
        {
            const int val = infix[i];
            if (push(st, val) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                free(*postfix);
                return ALLOCATE_ERROR;
            }
            ++depth;
        }
        else 
        {
            char c = infix[i];
            if (c == '-') 
            {
                bool is_binary = false;
                int j = i - 1;
                while (j >= 0) 
                {
                    if (is_bracket(infix[j]) && infix[j] != ')') break;
                    if (isdigit(infix[j])) 
                    {
                        is_binary = true;
                        break;
                    }
                    --j;
                }
                if (!is_binary) c = 'd';
            }
            const int priority = get_priority(c);
            while (!is_empty(st) && (get_priority(st->head->data) >= priority)) 
            {
                if (is_empty(st)) break;
                const int val = pop(st);
                (*postfix)[postfix_len] = val;
                ++postfix_len;
            }
            const int val = c;
            if (push(st, val) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                free(*postfix);
                return ALLOCATE_ERROR;
            }
        }
    }
    while (!is_empty(st)) 
    {
        const int val = pop(st);
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

status_code calculate_postfix(const char* postfix, int* result) 
{
    const int postfix_len = strlen(postfix);
    if (!postfix || !postfix_len) return EMPTY_STRING;

    int number_len = 0, number_capacity = 2;
    char* number = (char*)malloc(sizeof(char) * number_capacity);
    if (!number) return ALLOCATE_ERROR;

    Stack* st = init_stack();
    if (!st) 
    {
        free(number);
        return ALLOCATE_ERROR;
    }
    for (int i = 0; i < postfix_len; ++i) 
    {
        if (isspace(postfix[i])) continue;
        if (!isdigit(postfix[i]) && !is_operator(postfix[i]) && !is_bracket(postfix[i])) 
        {
            free_stack(st);
            free(number);
            return UNEXPECTED_CHARACTER;
        }
        if (isdigit(postfix[i])) 
        {
            while ((isdigit(postfix[i])) && i < postfix_len) 
            {
                number[number_len] = postfix[i];
                ++number_len;
                if (number_len >= number_capacity) 
                {
                    number_capacity *= 2;
                    char* tmp = (char*)realloc(number, sizeof(char) * number_capacity);
                    if (!tmp) 
                    {
                        free_stack(st);
                        free(number);
                        return ALLOCATE_ERROR;
                    }
                    number = tmp;
                }
                ++i;
            }
            --i;
            number[number_len] = '\0';
            if (push(st, atoi(number)) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                free(number);
                return ALLOCATE_ERROR;
            }
            number_len = 0;
            number_capacity = 2;
            free(number);
            number = NULL;
            number = (char*)malloc(sizeof(char) * number_capacity);
            if (!number)
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
                free(number);
                return UNUSED_CHARACTER;
            }
            const int fst_val = pop(st);
            int res = 0;
            if (postfix[i] == 'd') res = -fst_val;
            else 
            {
                if (is_empty(st)) 
                {
                    free_stack(st);
                    free(number);
                    return UNUSED_CHARACTER;
                }
                const int snd_val = pop(st);
                switch (postfix[i]) 
                {
                    case '+':
                        res = snd_val + fst_val;
                        break;
                    case '-':
                        res = snd_val - fst_val;
                        break;
                    case '*':
                        res = snd_val * fst_val;
                        break;
                    case '/':
                        if (!fst_val) 
                        {
                            free_stack(st);
                            free(number);
                            return ZERO_DIVISION;
                        }
                        res = snd_val / fst_val;
                        break;
                    case '%':
                        if (!fst_val) 
                        {
                            free_stack(st);
                            free(number);
                            return ZERO_DIVISION;
                        }
                        if (fst_val < 0 || snd_val < 0) 
                        {
                            free_stack(st);
                            free(number);
                            return NEGATIVE_MOD;
                        }
                        res = snd_val % fst_val;
                        break;
                    case '^':
                        if (fst_val < 0) 
                        {
                            free_stack(st);
                            free(number);
                            return NEGATIVE_POWER;
                        }
                        if (!fst_val && !snd_val) 
                        {
                            free_stack(st);
                            free(number);
                            return NEGATIVE_POWER;
                        }
                        res = pow(snd_val, fst_val);
                        break;
                    case 'd':
                        break;
                    default:
                        free_stack(st);
                        free(number);
                        return UNEXPECTED_CHARACTER;
                }
            }
            if (push(st, res) == ALLOCATE_ERROR) 
            {
                free_stack(st);
                free(number);
                return ALLOCATE_ERROR;
            }
        }
    }
    if (is_empty(st)) 
    {
        free_stack(st);
        free(number);
        return UNUSED_CHARACTER;
    }

    *result = pop(st);
    
    if (!is_empty(st)) 
    {
        free_stack(st);
        free(number);
        return UNUSED_CHARACTER;
    } 

    free(number);
    free_stack(st);

    return SUCCESS;
}