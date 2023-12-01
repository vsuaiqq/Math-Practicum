#include "utils.h"

int cmp_asc(const void* a, const void* b) 
{
    return (*(int *)a - *(int *)b);
}

int cmp_dsc(const void* a, const void* b) 
{
    return (*(int *)b - *(int *)a);
}

int cmp_rand(const void* a, const void* b)
{
    return rand() % 3 - 1;
}

int get_rand(int min, int max) 
{
    return min + rand() % (max - min + 1);
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
        case INVALID_DATA:
            printf("Invalid input data!\n");
            break;
        case NOT_FOUND:
            printf("Array doesn't exist!\n");
        default:
            break;
    }
}

void str_to_lower(char** str) 
{
    for (int i = 0; i < strlen(*str); ++i) 
    {
        (*str)[i] = tolower((*str)[i]);
    }
}

bool validate_name(const char name) 
{
    return (name >= 'a' && name <= 'z') || (name >= 'A' && name <= 'Z');
}
 
bool validate_command(const char* command) 
{
    const int command_len = strlen(command);
    if ((command[command_len - 1] == '\n') && (command[command_len - 2] != ';')) return false;
    if ((command[command_len - 1] != '\n') && (command[command_len - 1] != ';')) return false;
    return true;
}

sort_type get_sort_type(const char* command) 
{
    const int command_len = strlen(command);
    if ((command[command_len - 1] == '\n') && (command[command_len - 3] == '+')) return ASCENDING;
    if ((command[command_len - 1] == '\n') && (command[command_len - 3] == '-')) return DESCENDING;
    if ((command[command_len - 1] != '\n') && (command[command_len - 2] == '+')) return ASCENDING;
    if ((command[command_len - 1] != '\n') && (command[command_len - 2] == '-')) return DESCENDING;
    return INVALID;
}