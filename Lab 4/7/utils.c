#include "utils.h"

bool is_num(const char* str) 
{
    for (int i = 0; i < strlen(str); ++i) 
    {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

cmd_type get_cmd_type(char* cmd) 
{
    int cmd_len = strlen(cmd);
    char tmp[BUFSIZ];

    if (cmd[cmd_len - 1] == '\n') cmd[cmd_len-- - 1] = '\0';

    if (!strcmp(cmd, "print;")) return print_all;
    if (sscanf(cmd, "print %s;", tmp) == 1) return print_single;
    for (int i = 0; i < strlen(cmd); ++i) 
    {
        if (cmd[i] == '+') return plus;
        if (cmd[i] == '-') return minus;
        if (cmd[i] == '/') return division;
        if (cmd[i] == '%') return remainder;
        if (cmd[i] == '*') return multiplying;
    }

    return assigning;
}

char get_operation_sign(const cmd_type type) 
{
    switch (type)
    {
        case plus: return '+';
        case minus: return '-';
        case division: return '/';
        case remainder: return '%';
        case multiplying: return '*';
        default:
            return ' ';
    }
}

void print_error(const status_code error_code) 
{
    switch (error_code)
    {
        case FILE_ERROR:
            printf("File error detected!\n");
            break;
        case ALLOCATE_ERROR:
            printf("Allocate error detected!\n");
            break;
        case RUNTIME_ERROR:
            printf("Runtime error detected!\n");
            break;
        case INPUT_ERROR:
            printf("No input data!\n");
        default:
            break;
    }
}

status_code get_single_name(char** str, const char* cmd, const int start_pos, const char sep, int* split_pos) 
{
    int str_len = -1, str_capacity = 32;
    *str = (char*)malloc(sizeof(char) * str_capacity);
    if (!*str) return ALLOCATE_ERROR;
    while (cmd[++str_len + start_pos] != sep) 
    {
        (*str)[str_len] = cmd[str_len + start_pos];
        if (str_len >= str_capacity) 
        {
            str_capacity *= 2;
            char* tmp = (char*)realloc(*str, sizeof(char) * str_capacity);
            if (!tmp) 
            {
                free(*str);
                return ALLOCATE_ERROR;
            }
            *str = tmp;
        }
    }
    *split_pos += str_len + 1;
    (*str)[str_len] = '\0';
    return SUCCESS;
}