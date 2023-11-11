#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum status_code 
{
    success,
    invalid_parameter,
    allocate_error,
} status_code;

bool validate_power(const int power) 
{
    return 1 <= power && power <= 5;
}

int add(int a, int b) 
{
    while (b != 0) 
    {
        int carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

void increment(int* x) 
{
    *x = add(*x, 1);
}

int negate(const int x)
{
    int negative_x = ~x;
    increment(&negative_x);
    return negative_x;
}

status_code decimal_to_power_of_two(const int power_of_two, int num, char** result) 
{
    if (!validate_power(power_of_two)) return invalid_parameter;

    const int base = 2 << add(power_of_two, negate(1));

    if (!num) 
    {
        *result = (char*)malloc(sizeof(char) * 2);
        if (*result == NULL) return allocate_error;

        (*result)[0] = '0', (*result)[1] = '\0';
        
        return success;
    }

    const char all_symbols[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V',
    };

    int result_capacity = 32;
    int result_size = 0;

    *result = (char*)malloc(sizeof(char) * result_capacity);
    if (*result == NULL) return allocate_error;

    bool is_negative = false;
    if (num < 0) 
    {
        is_negative = true;
        num = negate(num);
        (*result)[0] = '-';
        increment(&result_size);
    }

    while (num > 0) 
    {
        const int remainder = num & add(base, negate(1));
        const char digit = all_symbols[remainder];

        if (result_size >= add(result_capacity, negate(1))) 
        {
            result_capacity = result_capacity << 1;
            char* tmp = (char*)realloc(*result, result_capacity);
            if (tmp == NULL) 
            {
                free(*result);
                return allocate_error;
            }
            *result = tmp;
        }

        (*result)[result_size] = digit;
        increment(&result_size);

        num = num >> power_of_two;
    }

    (*result)[result_size] = '\0';
    increment(&result_size);

    char* tmp = (char*)realloc(*result, result_size);
    if (tmp == NULL) 
    {
        free(*result);
        return allocate_error;
    }
    *result = tmp;

    int i = (is_negative) ? 1 : 0;
    const int dif = (is_negative) ? 1 : 2;
    for (; i < (result_size >> 1); increment(&i)) 
    {
        const int right_idx = add(result_size, negate(add(i, dif)));
        const char tmp = (*result)[i];
        (*result)[i] = (*result)[right_idx];
        (*result)[right_idx] = tmp;
    }

    return success;
}

int main() 
{
    char* result;

    switch (decimal_to_power_of_two(1, 16, &result)) 
    {
        case success:
            printf("%s\n", result);
            break;
        case invalid_parameter:
            printf("invalid parameter detected!\n");
            return 1;
        case allocate_error:
            printf("allocate error detected!\n");
            return 1;
    }

    return 0;
}