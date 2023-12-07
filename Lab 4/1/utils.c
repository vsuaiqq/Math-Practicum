#include "utils.h"

bool is_prime(const int num) 
{
    if (!num || num == 1) return false;
    if (num == 2) return true;
    if (!(num & 1)) return false;
    for (int d = 3; d * d <= num; d += 2) 
    {
        if (!(num % d)) return false;
    }
    return true;
}

int get_next_prime(int num) 
{
    if (!num || num == 1) return 2;
    if (!(num & 1)) return ++num;
    do  
    {
        num += 2;
    } while (!is_prime(num));
    return num;
}

void print_err(const status_code st) 
{
    if (st == ALLOCATE_ERROR) 
    {
        printf("Allocate error detected!\n");
    }
    if (st == FILE_ERROR) 
    {
        printf("File error detected!\n");
    }
    if (st == ARGUMENTS_ERROR) 
    {
        printf("Invalid number of arguments!\n");
    }
}