#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_STR_SIZE 32
#define MIN_SS_BASE 2
#define MAX_SS_BASE 36

enum read_string_status_code 
{
    rs_ok,
    rs_allocate_err,
};

enum to_decimal_status_code 
{
    td_ok,
    td_overflow,
};

enum from_decimal_status_code 
{
    fd_ok,
    fd_allocate_err,
};

bool validate_base(int base) 
{
    return (base >= MIN_SS_BASE && base <= MAX_SS_BASE);
}

bool validate_input(char* input_str, int base) 
{
    for (int i = 0; i < strlen(input_str); ++i) 
    {
        if (i == 0 && input_str[i] == '-') continue;
        if (!(input_str[i] >= 'A' && input_str[i] <= 'Z') && !(input_str[i] >= '0' && input_str[i] <= '9')) return false;
        if (input_str[i] >= '0' && input_str[i] <= '9' && (input_str[i] - '0') >= base) return false;
        if (input_str[i] >= 'A' && input_str[i] <= 'Z' && (input_str[i] - 55) >= base) return false;
    }
    return true;
}

int valid_char_to_digit(char c) 
{
    if (c >= 'A' && c <= 'Z') return c - 55;
    else return (c - '0');
}

void write_string_skip_leading_zeros(char* str) 
{
    int idx = 0;
    if (str[0] == '-') 
    { 
        putchar('-');
        ++idx; 
    }
    while (str[idx] == '0') ++idx;
    for (int i = idx; str[i] != '\0'; ++i) 
    {
        putchar(str[i]);
    }
    putchar('\n');
}

enum to_decimal_status_code to_decimal(char* str, int base, bool is_negative, int* res) 
{
    *res = 0;
    for (int i = 0 + (int)is_negative; str[i] != '\0'; ++i) 
    {
        if (*res > INT_MAX / base) return td_overflow;
        *res = (*res) * base;
        int digit = valid_char_to_digit(str[i]);
        if (*res > INT_MAX - digit) return td_overflow;
        *res += digit;
    }
    *res = ((is_negative) ? -(*res) : (*res));
    return td_ok;
}

enum from_decimal_status_code from_decimal(int decimal, int new_base, bool is_negative, char** res_str) 
{
    int size = 0 + (int)is_negative;
    decimal = ((is_negative) ? -decimal : decimal);
    if (decimal == 0) size = 2;
    else size += log(decimal) / log(new_base) + 1;

    *res_str = (char*)malloc(sizeof(char) * size);
    if (*res_str == NULL) 
    {
        return fd_allocate_err;
    }

    if (decimal == 0) 
    {
        (*res_str)[0] = '0';
        (*res_str)[1] = '\0';
        return fd_ok;
    }

    int idx = 0;
    while (decimal > 0) 
    {
        int last_digit = decimal % new_base;
        (*res_str)[idx] = ((last_digit < 10) ? (last_digit + '0') : (last_digit + 55));
        decimal /= new_base;
        ++idx;    
    }
    if (is_negative) 
    {
        (*res_str)[idx] = '-';
        idx++;
    } 
    (*res_str)[idx] = '\0';
    for (int i = 0; i < (idx)/2; ++i) 
    {
        char tmp = (*res_str)[i];
        (*res_str)[i] = (*res_str)[idx - i - 1];
        (*res_str)[idx - i - 1] = tmp;
    }

    return fd_ok;
}

enum read_string_status_code read_string(char** res_str) 
{
    *res_str = (char*)malloc(sizeof(char) * (MAX_STR_SIZE + 1));
    if (*res_str == NULL) 
    {
        return rs_allocate_err;
    }
    int cur_size = MAX_STR_SIZE, char_cnt = 0;
    char c = getchar();

    while (isspace(c)) c = getchar();

    while (!isspace(c)) 
    {
        ++char_cnt;
        if (char_cnt > MAX_STR_SIZE) 
        {
            cur_size *= 2;
            *res_str = (char*)realloc(*res_str, cur_size);
            if (*res_str == NULL) 
            {
                free(*res_str);
                return rs_allocate_err;
            }
        }
        (*res_str)[char_cnt - 1] = c;
        c = getchar();
    }
    (*res_str)[char_cnt] = '\0';

    return rs_ok;
}

int main() 
{
    int base;

    if (scanf("%d", &base) == 0) 
    {
        printf("base must have int type\n");
        return 1;
    }

    if (!validate_base(base)) 
    {
        printf("base must be in range [2, 36]\n");
        return 1;
    }

    char* input_str = NULL;
    char* max_input_str = NULL;
    char* max_input_str_9cc = NULL;
    char* max_input_str_18cc = NULL;
    char* max_input_str_27cc = NULL;
    char* max_input_str_36cc = NULL;
    int max_input_is_negative, max_input_decimal = INT_MIN;

    bool is_empty_input = true;

    if (read_string(&input_str) == rs_allocate_err) 
    {
        printf("allocate error\n");
        return 1;
    }

    while (strcmp(input_str, "Stop") != 0)
    {
        is_empty_input = false;

        if (!validate_input(input_str, base)) 
        {
            printf("invalide value\n");
            return 1;
        }

        int input_decimal;
        bool is_negative = ((input_str[0] == '-') ? true : false);

        if (to_decimal(input_str, base, is_negative, &input_decimal) == td_overflow) 
        {
            printf("overflow\n");
            free(input_str);
            free(max_input_str);
            return 1;
        }

        if (abs(input_decimal) > abs(max_input_decimal))
        {
            max_input_decimal = input_decimal, max_input_is_negative = is_negative;
            if (max_input_str != NULL) free(max_input_str);
            max_input_str = (char*)malloc(sizeof(char) * (strlen(input_str) + 1));
            if (max_input_str == NULL) 
            {
                printf("allocate error\n");
                free(input_str);
                return 1;
            }
            strcpy(max_input_str, input_str);
        }

        free(input_str);

        if (read_string(&input_str) == rs_allocate_err) 
        {
            printf("allocate error\n");
            return 1;
        }
    }
    
    if (is_empty_input)
    {
        printf("no lines entered\n");
        return 1;
    }

    if (from_decimal(max_input_decimal, 9, max_input_is_negative, &max_input_str_9cc) == fd_allocate_err) 
    {
        printf("allocate error\n");
        free(input_str);
        free(max_input_str);
        return 1;
    }
    if (from_decimal(max_input_decimal, 18, max_input_is_negative, &max_input_str_18cc) == fd_allocate_err) 
    {
        printf("allocate error\n");
        free(input_str);
        free(max_input_str);
        free(max_input_str_9cc);
        return 1;
    }
    if (from_decimal(max_input_decimal, 27, max_input_is_negative, &max_input_str_27cc) == fd_allocate_err) 
    {
        printf("allocate error\n");
        free(input_str);
        free(max_input_str);
        free(max_input_str_9cc);
        free(max_input_str_18cc);
        return 1;
    }
    if (from_decimal(max_input_decimal, 36, max_input_is_negative, &max_input_str_36cc) == fd_allocate_err) 
    {
        printf("allocate error\n");
        free(input_str);
        free(max_input_str);
        free(max_input_str_9cc);
        free(max_input_str_18cc);
        free(max_input_str_27cc);
        return 1;
    }

    write_string_skip_leading_zeros(max_input_str);
    printf("%s\n", max_input_str_9cc);
    printf("%s\n", max_input_str_18cc);
    printf("%s\n", max_input_str_27cc);
    printf("%s\n", max_input_str_36cc);

    free(input_str);
    free(max_input_str);
    free(max_input_str_9cc);
    free(max_input_str_18cc);
    free(max_input_str_27cc);
    free(max_input_str_36cc);

    return 0;
}