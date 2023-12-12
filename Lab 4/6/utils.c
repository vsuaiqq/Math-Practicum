#include "utils.h"

bool is_bool_value(const char c) 
{
    return (c == '0') || (c == '1');
}

bool is_empty_str(const char* str) 
{
    const int str_len = strlen(str);
    return (!str || !str_len || str[str_len - 1] == '\n');
}

long long int get_binary_permutations(const int num_of_vars) 
{
    return 1 << num_of_vars;
}

int get_binary_value(const int line, const int pos) 
{
    return (line >> pos) & 1;
}

int get_priority(const char c) 
{
    switch (c)
    {
        case '=':
        case '-':
        case '|':
        case '<':
            return 1;
        case '&':
        case '?':
        case '!':
        case '+':
            return 1;
        case '~':
            return 3;
        default:
            return -1;
    }
}

int res_by_char(const int left, const int right, const char c) 
{
    switch (c) 
    {
        case '&':
            return left & right;
        case '|':
            return left | right;
        case '~':
            return !left;
        case '-':
            return left <= right;
        case '+':
            return left > right;
        case '<':
            return left != right;
        case '=':
            return left == right;
        case '!':
            return !(left & right);
        case '?':
            return !(left | right);
        default:
            return -1;
    }
}

int get_rand(const int min, const int max) 
{
    return min + rand() % (max - min + 1);
}

char* get_rand_filename() 
{
    srand(time(NULL));
    const int len = get_rand(MIN_FILENAME_SIZE, MAX_FILENAME_SIZE);
    char* filename = (char*)malloc(sizeof(char) * (len + 5));
    if (!filename) return NULL;
    for (int i = 0; i < len; ++i) 
    {
        const int rand_type = get_rand(0, 1);
        if (rand_type) 
        {
            const int rand_case = get_rand(0, 1);
            if (rand_case) 
            {
                filename[i] = 'A' + get_rand(0, 25);
            }
            else 
            {
                filename[i] = 'a' + get_rand(0, 25);
            }
        }
        else 
        {
            filename[i] = '0' + get_rand(0, 9);
        }
    }
    filename[len] = '.', filename[len + 1] = 't', filename[len + 2] = 'x', filename[len + 3] = 't', filename[len + 4] = '\0';
    return filename;
}

void print_info(FILE* stream, const char* vars, const int vars_size, const char* infix) 
{
    for (int i = 0; i < vars_size; ++i) 
    {
        fprintf(stream, " %c |", vars[i]);
    }
    fprintf(stream, " %s\n", infix);
    fprintf(stream, "-------------------------");
}

void print_line(FILE* stream, const int* values, const int values_size, const int result, const int margin) 
{
    fprintf(stream, "\n");
    for (int i = 0; i < values_size; ++i)
    {
        fprintf(stream, " %d |", values[i]);
    }
    for (int i = 0; i < margin; ++i) fprintf(stream, " ");
    fprintf(stream, "%d\n", result);
    fprintf(stream, "-------------------------");
}

void print_error(const status_code st) 
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
    if (st == UNEXPECTED_CHARACTER) 
    {
        printf("Unexpected character.\n");
    }
    if (st == INVALID_BRACKETS_BALANCE) 
    {
        printf("Brackets balance error.\n");
    }
    if (st == EMPTY_STRING)
    {
        printf("Empty string.\n");
    }
    if (st == UNUSED_CHARACTER)
    {
        printf("Unused character.\n");
    }
}

status_code get_vars_from_expr(char** vars, int* vars_size, const char* expr) 
{
    (*vars) = (char*)malloc(sizeof(char) * 2);
    if (!*vars) return ALLOCATE_ERROR;
    *vars_size = 0;
    int vars_capacity = 2;
    for (int i = 0; i < strlen(expr); ++i) 
    {
        if (!isalpha(expr[i])) continue;
        bool is_duplicated = false;
        for (int j = 0; j < *vars_size; ++j) 
        {
            if ((*vars)[j] == expr[i])
            {
                is_duplicated = true;
                break;
            }
        }
        if (is_duplicated) continue;
        (*vars)[*vars_size] = expr[i];
        ++(*vars_size);
        if (*vars_size >= vars_capacity) 
        {
            vars_capacity *= 2;
            char* tmp = (char*)realloc(*vars, sizeof(char) * vars_capacity);
            if (!tmp) 
            {
                free(*vars);
                return ALLOCATE_ERROR;
            }
            *vars = tmp;
        }
    }
    return SUCCESS;
}