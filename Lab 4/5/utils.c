#include "utils.h"

void print_std_error(const status_code st) 
{
    if (st == ARGUMENTS_ERROR) 
    {
        printf("Invalid number of arguments detected!\n");
    }
    if (st == ALLOCATE_ERROR) 
    {
        printf("Allocate error detected!\n");
    }
    if (st == FILE_ERROR) 
    {
        printf("File error detected!\n");
    }
}

void print_error(FILE* stream, const status_code st, const char* infix, const int line) 
{
    if (st == UNEXPECTED_CHARACTER) 
    {
        fprintf(stream, "Unexpected character. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == INVALID_BRACKETS_BALANCE) 
    {
        fprintf(stream, "Brackets balance error. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == EMPTY_SPACE_INSIDE_BRACKETS) 
    {
        fprintf(stream, "Empty space inside brackets. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == EMPTY_STRING)
    {
        fprintf(stream, "Empty string. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == UNUSED_CHARACTER)
    {
        fprintf(stream, "Unused character. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == ZERO_DIVISION)
    {
        fprintf(stream, "Zero division. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == NEGATIVE_MOD)
    {
        fprintf(stream, "Negative mod. Expression: %s. Line: %d.\n", infix, line);
    }
    if (st == NEGATIVE_POWER) 
    {
        fprintf(stream, "Negative power. Expression: %s. Line: %d.\n", infix, line);
    }
}

void print_expr(const char* file_path, const char* infix, const char* postfix, const int value) 
{
    printf("File path: %s, ", file_path);
    printf("Expression: %s, ", infix);
    printf("Postfix: ");
    for (int i = 0; i < strlen(postfix); ++i) 
    {
        if (postfix[i] == 'd') printf("-");
        else printf("%c", postfix[i]);
    }
    printf(", ");
    printf("Value: %d", value);
    printf("\n");
}

bool is_operator(const char c) 
{
    return 
    (
        (c == '+') || 
        (c == '*') || 
        (c == '/') || 
        (c == '-') ||
        (c == '%') || 
        (c == '^') || 
        (c == 'd')
    );
}

bool is_bracket(const char c) 
{
    return (c == '(') || (c == ')');
}

int get_priority(const char c) 
{
    switch (c) 
    {
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 2;
        case '%':
            return 2;
        case 'd':
            return 2;
        case '^':
            return 3;
        default:
            return -1;
    }
}