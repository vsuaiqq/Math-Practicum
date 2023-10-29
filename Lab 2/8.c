#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum status_code
{
    success,
    invalid_base,
    invalid_num,
    allocate_error,
} status_code;

bool validate_base(const int base) 
{
    return base >= 2 && base <= 36;
}

bool validate_input(const char* str, const int base) 
{
    const int str_len = strlen(str);

    if (!str_len) return false;
    for (int i = 0; i < str_len; ++i) 
    {
        if (!isalpha(str[i]) && !isdigit(str[i])) return false;
        else if (isalpha(str[i])) 
        {
            if (toupper(str[i]) - 55 >= base) return false;
        }
        else if (isdigit(str[i]))
        {
            if (str[i] - '0' >= base) return false;
        }
    }
    return true;
} 


char digit_to_char(const int d) 
{
    if (d >= 10) 
    {
        return (char)(d + 55);
    }
    else return d + '0';
}

int char_to_int(const char c) 
{
    if (c >= 'A' && c <= 'Z') 
    {
        return c - 'A' + 10;
    }
    else if (c >= 'a' && c <= 'z') 
    {
        return toupper(c) - 'A' + 10;
    }
    else return c - '0';
}

status_code sum_of_two(const char* str_1, const char* str_2, const int base, char** res) 
{
    const int len_1 = strlen(str_1);
    const int len_2 = strlen(str_2);
    const int max_len = (len_1 > len_2) ? len_1 : len_2;
    
    *res = (char*)malloc(sizeof(char) * (max_len + 2));
    if (*res == NULL) return allocate_error;
    
    int pos_1 = len_1 - 1;
    int pos_2 = len_2 - 1;
    int res_pos = max_len;

    int carry = 0;
    while (pos_1 >= 0 || pos_2 >= 0) 
    {
        int digit_1 = (pos_1 >= 0) ? char_to_int(str_1[pos_1]) : 0;
        int digit_2 = (pos_2 >= 0) ? char_to_int(str_2[pos_2]) : 0;
        int sum = digit_1 + digit_2 + carry;

        carry = (sum >= base) ? 1 : 0;

        (*res)[res_pos] = digit_to_char(sum % base);

        --pos_1, --pos_2, --res_pos;
    }
    (*res)[0] = (carry == 1) ? '1': '0';

    bool is_all_zero = false;
    int i = 0;
    while ((*res)[i] == '0') 
    {
        if (i == max_len) is_all_zero = true;
        ++i;
    }
    if (is_all_zero) --i;
    for (int j = i; j < max_len + 1; ++j) 
    {
        (*res)[j - i] = (*res)[j];
    }
    (*res)[max_len + 1 - i] = '\0';

    char* tmp = (char*)realloc(*res, sizeof(char) * (max_len + 2 - i));
    if (tmp == NULL) 
    {
        free(*res);
        return allocate_error;
    }
    *res = tmp;

    return success;
}

status_code sum_of_numbers(const int base, const int nums_amount, char** result, ...) 
{
    if (!validate_base(base)) return invalid_base;

    char* prev_res = (char*)malloc(sizeof(char) * 2);
    prev_res[0] = '0', prev_res[1] = '\0';

    va_list ptr;
    va_start(ptr, result);

    for (int i = 0; i < nums_amount; ++i) 
    {
        const char* cur_num = va_arg(ptr, const char*);
        if (!validate_input(cur_num, base)) 
        {
            va_end(ptr);
            free(prev_res);
            return invalid_num;
        }

        if (sum_of_two(prev_res, cur_num, base, result) == allocate_error) 
        { 
            va_end(ptr);
            free(prev_res);
            return allocate_error;
        }

        char* tmp = (char*)realloc(prev_res, sizeof(char) * (strlen(*result) + 1));
        if (tmp == NULL) 
        {
            va_end(ptr);
            free(prev_res);
            free(*result);
            return allocate_error;
        }
        prev_res = tmp;
        strcpy(prev_res, *result);
        if (i != nums_amount - 1) free(*result);
    }

    va_end(ptr);
    free(prev_res);
    return success;
}

int main() 
{
    char* res_str = NULL;
    switch (sum_of_numbers(2, 2, &res_str, "1", "0", "1"))
    {
        case success:
            printf("res: %s\n", res_str);
            break;
        case invalid_base:
            printf("base must be in range [2, 36]!\n");
            break;
        case invalid_num:
            printf("invalid num for current base!\n");
            break;
        case allocate_error:
            printf("allocate error detected!\n");
            return 1;
    }

    if (res_str != NULL) 
    {
        free(res_str);   
        res_str = NULL;
    } 
    return 0;
}