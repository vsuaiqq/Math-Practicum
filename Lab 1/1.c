#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

enum factorial_status_code 
{
    fsc_ok,
    fsc_overflow,
    fsc_invalid_parametr
};

enum is_simple_status_code 
{
    issc_yes,
    issc_no,
    issc_invalid_parametr
};

enum range_sum_status_code 
{
    rssc_ok,
    rssc_overflow,
    rssc_invalid_parametr
};

enum factorial_status_code factorial(int x, long *result)
{
    if (x < 0)
    {
        return fsc_invalid_parametr;
    }
    if (x == 0 || x == 1) 
    {
        *result = 1;
        return fsc_ok;
    }
    *result = 1;
    long prev = 1;
    for (int num = 2; num <= x; ++num) 
    {
        *result *= num;
        if (*result < prev) 
        {
            return fsc_overflow;
        }
        prev = *result;
    }
    return fsc_ok;
}

enum is_simple_status_code is_simple(int x) 
{
    if (x <= 1) 
    {
        return issc_invalid_parametr;
    }
    if (x == 2) 
    {
        return issc_yes;
    }
    for (int num = 2; num * num <= x; ++num) 
    {
        if (x % num == 0) return issc_no;
    }
    return issc_yes;
}

void split_num(const char *num_str)
{
    if (num_str[0] == '0' && num_str[1] == '\0') 
    {
        printf("0\n");
        return;
    }

    int i = 0;
    while (num_str[i] == '0') ++i;
    if (num_str[0] == '-') ++i;

    for (int j = i; j < strlen(num_str); ++j) 
    {
        printf("%c ", num_str[j]);
    }
    printf("\n");
}

enum range_sum_status_code range_sum(int x, long* result)
{
    if (x <= 0) return rssc_invalid_parametr;
    double a = x * 0.5 + 0.5;
    long b = x;
    if (a > LONG_MAX / b) 
    {
        return rssc_overflow;
    }
    *result = a * b;
    return rssc_ok;
}

void multiples_nums(int x, bool* is_found)
{
    *is_found = false;
    for (int num = x; num <= 100; num += x) 
    {
        *is_found = true;
        printf("%d ", num);
    }
    if (*is_found) printf("\n");
}

void degree_table(int x) 
{
    for (int base = 1; base <= 10; ++base) 
    {
        long long num = 1;
        for (int degree = 1; degree <= x; ++degree) 
        {
            num *= base;
            printf("%11lld ", num);
        } 
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc > 3) 
    {
        printf("invalid number of arguments detected!\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[2]); ++i) 
    {
        if (i == 0 && argv[2][0] == '-') continue;
        if ((int)argv[2][i] < 48 || (int)argv[2][i] > 57) 
        {
            printf("argument must have int type!\n");
            return 1;
        }
    }
    
    if ((argv[1][0] != '/' && argv[1][0] != '-') || strlen(argv[1]) != 2)
    {
        printf("invalid flag detected!\n");
        return 1;
    }

    int input_num = atoi(argv[2]);
    
    long result;
    bool is_found;

    switch (argv[1][1])
    {
        case 'e':
            if (input_num > 10 || input_num < 1) 
            {
                printf("invalid parametr detected!\n");
                break;
            }
            degree_table(input_num);
            break;
            
        case 'h':
            if (input_num <= 0) 
            {
                printf("invalid parametr detected!\n");
                break;
            }
            multiples_nums(input_num, &is_found);
            if (!is_found) 
            {
                printf("no numbers found\n");
            }
            break;

        case 'a':
            switch (range_sum(input_num, &result))
            {
                case rssc_ok:
                    printf("%ld\n", result);
                    break;
                case rssc_overflow:
                    printf("overflow detected!\n");
                    break;
                case rssc_invalid_parametr:
                    printf("invalid parametr detected!\n");
                    break;
            }
            break;

        case 's':
            split_num(argv[2]);
            break;

        case 'p':
            switch (is_simple(input_num))
            {
                case issc_yes:
                    printf("%d is simple\n", input_num);
                    break;
                case issc_no:
                    printf("%d is composite\n", input_num);
                    break;
                case issc_invalid_parametr:
                    printf("invalid parametr detected!\n");
                    break;
            }
            break;

        case 'f':
            switch (factorial(input_num, &result))
            {
                case fsc_ok:
                    printf("%d! = %ld\n", input_num, result);
                    break;
                case fsc_overflow:
                    printf("overflow detected!\n");
                    break;
                case fsc_invalid_parametr:
                    printf("invalid parametr detected!\n");
                    break;
            }
            break;

        default:
            printf("unknown flag detected!\n");
            break;
    }
    return 0;
}