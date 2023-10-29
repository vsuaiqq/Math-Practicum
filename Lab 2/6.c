#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

typedef enum status_code 
{
    success,
    invalid_parameter,
    allocate_error,
} status_code;

bool is_all_digit(const char* str, const int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool my_islower(const char* str, const int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) return false;
    }
    return true;
} 

bool my_isupper(const char* str, const int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        if ((str[i] >= 'a') && (str[i] <= 'z')) return false;
    }
    return true;
} 

status_code from_roman_to_int(const char* number_str, int* num) 
{
    int size = strlen(number_str);
    if (!size) return invalid_parameter;

    *num = 0;
    int sign = (number_str[0] == '-') ? -1 : 1;

    for (int i = size - 1; i >= 0; --i) 
    {
        char c = number_str[i];
        switch (c) 
        {
            case 'I': 
                if (number_str[i + 1] == 'X' || number_str[i + 1] == 'V') 
                {
                    --(*num);
                } else ++(*num);
                break;
            case 'V': 
                (*num) += 5;
                break;
            case 'X': 
                if (number_str[i + 1] == 'L' || number_str[i + 1] == 'C') 
                {
                    (*num) -= 10;
                } else (*num) += 10;
                break;
            case 'L': 
                (*num) += 50;
                break;
            case 'C': 
                if (number_str[i + 1] == 'D' || number_str[i + 1] == 'M') 
                {
                    (*num) -= 100;
                } 
                else (*num) += 100;
                break;
            case 'M': 
                (*num) += 1000;
                break;
            default:
                break;
        }
    }

    *num *= sign;
    return success;
}

status_code from_zeckendorf(const char* zeckendorf_str, unsigned int* number) 
{
    int size = strlen(zeckendorf_str);
    if (!size) return invalid_parameter;

    unsigned int num1 = 0;
    unsigned int num2 = 1;
    unsigned int cur = 1;

    *number = 0;

    for (int i = 0; i < size - 1; ++i) 
    {
        char symb = zeckendorf_str[i];
        if (symb == '1') (*number) += cur;
        num1 = num2;
        num2 = cur;
        cur = num1 + num2;
    }
    return success;
}

status_code from_base_to_int(const char* number, const int number_size, int base, const bool is_upper, int* res) 
{
    if (!number) return invalid_parameter;
    if ((base < 2) || (base > 36)) base = 10;
    
    int power = 1, sign = 1, sign_flag = 0;
    *res = 0;
    if (number[0] == '-') 
    {
        sign = -1;
        sign_flag = 1;
    }
    else if (number[0] == '+') sign_flag = 1;

    for (int i = number_size - 1; i >= sign_flag; --i) 
    {
        int digit = ((isdigit(number[i])) ? number[i] - '0' : (is_upper) ? number[i] - 'A' + 10 : number[i] - 'a' + 10);
        if (digit >= base)
        {
            return invalid_parameter;
        }
        *res += power * digit;
        power *= base;
    }

    *res *= sign;
    return success;
}

int overfscanf(FILE * stream, const char * format, ...) 
{
    if (!stream) return -1;
    int format_size = strlen(format);
    if (!format_size) return -1;

    int num_of_args = 0;
    for (int i = 0; i < format_size; ++i) 
    {
        if (format[i] == '%') 
        {
            if (format[i + 1] != '%') 
            {
                ++num_of_args;
            }
        }
    }

    va_list ptr;
    va_start(ptr, num_of_args);

    int index = 0;
    char* flag = NULL;
    int total = 0;

    for (int i = 0; i < num_of_args; ++i) 
    {
        int flag_size = 1, flag_capacity = 2;

        while(format[index] != '%' && index < format_size) 
        {
            ++index;
        }
        
        flag = (char*)malloc(sizeof(char) * (flag_capacity + 2));
        if (!flag) return -1;
        flag[0] = '%';
        index++;

        while ((isdigit(format[index])) || (isalpha(format[index])) && (index < format_size)) 
        {
            char c = format[index];
            flag[flag_size] = c;
            ++flag_size;
            
            if (flag_size == flag_capacity - 1) 
            {
                flag_capacity *= 2;
                char* tmp = (char*)realloc(flag, flag_capacity);
                if (!tmp) 
                {
                    free(flag);
                    printf("allocate error\n");
                    return -1;
                }
                flag = tmp;
            }
            ++index;
        }
        flag[flag_size] = '\0';
        if (!strcmp(flag, "%Ro\0")) 
        {
            int* ptr_to_arg = va_arg(ptr, int*);
            char tmp[256];
            total += fscanf(stream, "%s", tmp);
            status_code status = from_roman_to_int(tmp, ptr_to_arg);
        } 
        else if (!strcmp(flag, "%Zr\0")) 
        {
            unsigned int* ptr_to_arg = va_arg(ptr, unsigned int*);
            char tmp[256];
            total += fscanf(stream, "%s", tmp);
            status_code status = from_zeckendorf(tmp, ptr_to_arg);
        } 
        else if (!strcmp(flag, "%Cv\0")) 
        {
            int* ptr_to_arg = va_arg(ptr, int*);
            ++num_of_args;
            int base = va_arg(ptr, int);
            char number_str[256];
            total += fscanf(stream, "%s", number_str);
            int size = strlen(number_str);
            if (my_isupper(number_str, size) && !is_all_digit(number_str, size)) 
            {
                printf("letters must be in lowercase\n");
                (*ptr_to_arg) = 0;
            } 
            else 
            {
                status_code status = from_base_to_int(number_str, size, base, false, ptr_to_arg);
            }
        } 
        else if (!strcmp(flag, "%CV\0")) 
        {
            int* ptr_to_arg = va_arg(ptr, int*);
            ++num_of_args;
            int base = va_arg(ptr, int);
            char number_str[256];
            total += fscanf(stream, "%s", number_str);
            int size = strlen(number_str);
            if (my_islower(number_str, size) && !is_all_digit(number_str, size)) 
            {
                printf("letters must be in uppercase\n");
                (*ptr_to_arg) = 0;
            } 
            else 
            {
                status_code status = from_base_to_int(number_str, size, base, true, ptr_to_arg);
            }
        } 
        else 
        {
            void* tmp_ptr_to_arg = va_arg(ptr, void*);
            total += fscanf(stream, flag, tmp_ptr_to_arg);
        }
        free(flag);
        flag = NULL;
    }
    va_end(ptr);
    return total;
}

int oversscanf(char * buf, const char * format, ...) 
{
    if (!buf) return -1;
    int format_size = strlen(format);
    if (!format_size) return -1;

    int num_of_args = 0;
    for (int i = 0; i < format_size; ++i) 
    {
        if (format[i] == '%') 
        {
            if (format[i + 1] != '%') 
            {
                ++num_of_args;
            }
        }
    }

    va_list ptr;
    va_start(ptr, num_of_args);

    int index = 0, buf_index = 0;
    char* flag = NULL;
    int total = 0;

    for (int i = 0; i < num_of_args; ++i) 
    {
        int flag_size = 1, flag_capacity = 2;

        while(format[index] != '%' && index < format_size) 
        {
            ++index;
        }
        
        flag = (char*)malloc(sizeof(char) * (flag_capacity + 2));
        if (!flag) return -1;
        flag[0] = '%';
        index++;

        while ((isdigit(format[index])) || (isalpha(format[index])) && (index < format_size)) 
        {
            char c = format[index];
            flag[flag_size] = c;
            ++flag_size;
            
            if (flag_size == flag_capacity - 1) 
            {
                flag_capacity *= 2;
                char* tmp = (char*)realloc(flag, flag_capacity);
                if (!tmp) 
                {
                    free(flag);
                    printf("allocate error\n");
                    return -1;
                }
            }
            ++index;
        }
        flag[flag_size] = '\0';

        if (!strcmp(flag, "%Ro\0")) 
        {
            int* ptr_to_arg = va_arg(ptr, int*);
            char tmp[256];
            total += sscanf(buf_index + buf, "%s", tmp);
            buf_index += strlen(tmp) + 1;
            status_code status = from_roman_to_int(tmp, ptr_to_arg);
        } 
        else if (!strcmp(flag, "%Zr\0")) 
        {
            unsigned int* ptr_to_arg = va_arg(ptr, unsigned int*);
            char tmp[256];
            total += sscanf(buf_index + buf, "%s", tmp);
            status_code status = from_zeckendorf(tmp, ptr_to_arg);
            buf_index += strlen(tmp) + 1;
        } 
        else if (!strcmp(flag, "%Cv\0")) 
        {
            int* ptr_to_arg = va_arg(ptr, int*);
            ++num_of_args;
            int base = va_arg(ptr, int);
            char number_str[256];
            total += sscanf(buf_index + buf, "%s", number_str);
            int size = strlen(number_str);
            buf_index += size + 1;
            if (my_isupper(number_str, size) && !is_all_digit(number_str, size)) 
            {
                printf("letter must be in lowercase\n");
                (*ptr_to_arg) = 0;
            } 
            else 
            {
                status_code status = from_base_to_int(number_str, size, base, false, ptr_to_arg);
            }
            --num_of_args;
        } 
        else if (!strcmp(flag, "%CV\0")) 
        {
            int* ptr_to_arg = va_arg(ptr, int*);
            ++num_of_args;
            int base = va_arg(ptr, int);
            char number_str[256];
            total += sscanf(buf_index + buf, "%s", number_str);
            int size = strlen(number_str);
            buf_index += size + 1;
            if (my_islower(number_str, size) && !is_all_digit(number_str, size)) 
            {
                printf("letters must be in uppercase\n");
                (*ptr_to_arg) = 0;
            } 
            else 
            {
                status_code status = from_base_to_int(number_str, size, base, true, ptr_to_arg);
            }
            --num_of_args;
        } 
        else 
        {
            void* tmp_ptr_to_arg = va_arg(ptr, void*);
            total += sscanf(buf_index + buf, flag, tmp_ptr_to_arg);
            buf_index += strlen(tmp_ptr_to_arg) + 1;
        }
        free(flag);
        flag = NULL;
    }
    va_end(ptr);
    return total;
}

int main() 
{
    char buff[255] = "XV A LI 1000";
    int x, y, z, w;
    oversscanf(buff, "%Ro %CV %Ro %CV", &x, &y, 11, &z, &w, 2);
    printf("%d %d %d %d\n", x, y, z, w);

    FILE* file = fopen("test.txt", "r");
    int a, b, c, d;
    overfscanf(file, "%Ro %CV %Ro %CV", &a, &b, 11, &c, &d, 2);
    printf("%d %d %d %d\n", a, b, c, d);
}