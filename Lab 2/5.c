#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef enum status_code 
{
    success,
    invalid_parameter,
    allocate_error,
} status_code;

status_code to_roman(int number, char** roman) 
{
    int roman_numbers[13] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
    char roman_letters[13][2] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};

    int roman_size = 0, roman_capacity = 2;
    *roman = (char*)malloc(sizeof(char) * roman_capacity);
    if (*roman == NULL) return allocate_error;

    if (number < 0) 
    {
        number = abs(number);
        (*roman)[0] = '-';
        ++roman_size;
    }

    while (number) 
    {
        int index = 13;
        while (roman_numbers[--index] > number);
        for (int i = 0; i < 2; ++i) 
        {
            if (roman_letters[index][i] == 'I' || roman_letters[index][i] == 'V' || roman_letters[index][i] == 'X' || 
            roman_letters[index][i] == 'L' || roman_letters[index][i] == 'C' || roman_letters[index][i] == 'D' || roman_letters[index][i] == 'M') 
            {
                (*roman)[roman_size] = roman_letters[index][i];
                ++roman_size;
                if (roman_size == roman_capacity) 
                {
                    roman_capacity *= 2;
                    char* tmp = (char*)realloc(*roman, roman_capacity);
                    if (tmp == NULL) 
                    {
                        free(*roman);
                        return allocate_error;
                    }
                    *roman = tmp;
                }
            }
        }
        number -= roman_numbers[index];
    }

    (*roman)[roman_size] = '\0';
    return success;
}

void fibonacci(unsigned int result[2], unsigned int number) 
{
    unsigned int count = 0;
    unsigned int tmp1 = 0, tmp2 = 1;
    unsigned int next = tmp1 + tmp2;
    while (next <= number) 
    {
        tmp1 = tmp2;
        tmp2 = next;
        next = tmp1 + tmp2;
        ++count;
    }
    result[0] = count;
    result[1] = tmp2;
}

bool my_isupper(char * number, int number_size) 
{
    for (int i = 0; i < number_size; i++) 
    {
        if ((number[i] >= 'a') && (number[i] <= 'z')) return false;
    }
    return true;
}

bool my_islower(char * number, int number_size) 
{
    for (int i = 0; i < number_size; i++) 
    {
        if ((number[i] >= 'A') && (number[i] <= 'Z')) return false;
    }
    return true;
}

status_code from_base_to_int(const char* number, const int number_size, int base, const bool is_upper, long long int* res) 
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

status_code from_decimal_to_base(long long int number, int base, const bool is_upper, char** res) 
{
    if ((base < 2) || (base > 36)) base = 10;
    
    int res_length = 0, sign_flag = 0;
    if (number < 0) {
        res_length++;
        sign_flag = 1;
    }
    number = (number < 0) ? -number : number;
    res_length += (log(number) / log(base)) + 2;

    *res = (char*)malloc(sizeof(char) * res_length);
    if (*res == NULL) return allocate_error;

    (*res)[res_length - 1] = '\0';
    if (sign_flag) (*res)[0] = '-';
    for (int i = res_length - 2; i >= sign_flag; i--) 
    {
        (*res)[i] = (number % base > 9) ? (is_upper) ? number % base + 'A' - 10 : number % base + 'a' - 10: number % base + '0';
        number /= base;
    }

    return success;
}

void zeckendorf(int* result_size, unsigned int number, unsigned int result[number][2]) 
{
    (*result_size) = 0; 
    while (number) 
    {
        fibonacci(result[*result_size], number);
        ++(*result_size);
        number -= result[(*result_size) - 1][1];
    }
}

int overfprintf(FILE * stream, const char * format, ...) 
{
    if (!stream) return -1;
    int format_size = strlen(format);
    if (!format_size) return -1;

    int num_of_args = 0;
    for (int i = 0; i < format_size; ++i) 
    {
        if (format[i] == '%') ++num_of_args;
    }

    va_list ptr;
    va_start(ptr, format);

    int format_index = 0;
    int total = 0;
    while (format[format_index] != '%' && (format_index < format_size)) 
    {
        total += fprintf(stream, "%c", format[format_index]);
        ++format_index;
    }

    char* flag = NULL;
    char* tmp_flag = NULL;

    for (int i = 0; i < num_of_args; ++i) 
    {
        int flag_size = 1, flag_capacity = 2;
        flag = (char*)malloc(sizeof(char) * flag_capacity);
        flag[0] = '%';

        if (tmp_flag) free(tmp_flag);

        while ((isalpha(format[++format_index]) || isdigit(format[format_index]) || (format[format_index] == '%' && format[format_index - 1] == '%') ||
        (format[format_index] == '.' && (isalpha(format[format_index + 1]) || isdigit(format[format_index + 1]) || format[format_index + 1] == '*'))
        || (format[format_index] == '#') || (format[format_index] == '+') || (format[format_index] == '-') || (format[format_index] == '0')) && (format_index < format_size)) 
        {
            flag[flag_size] = format[format_index];
            ++flag_size;
            if (flag_size >= flag_capacity) 
            {
                flag_capacity *= 2;
                char* tmp = (char*)realloc(flag, sizeof(char) * flag_capacity);
                if (tmp == NULL) 
                {
                    free(flag);
                    printf("allocate error\n");
                    return -1;
                }
                flag = tmp;
            }
        }
        flag[flag_size] = '\0';
        if (!strcmp(flag, "%Ro\0")) 
        {
            char* result;

            status_code status = to_roman(va_arg(ptr, int), &result);
            switch (status)
            {
                case success:
                    total += fprintf(stream, "%s", result);
                    free(result);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return -1;
                case invalid_parameter:
                    break;
            }
        }
        else if (!strcmp(flag, "%Zr\0")) 
        {
            unsigned int number = va_arg(ptr, unsigned int);
            int result_size;
            unsigned int result[number][2];
            zeckendorf(&result_size, number, result);
            int count = 1;
            int i = result_size - 1;
            while (i >= 0) 
            {
                if (count == result[i][0]) 
                {
                    total += fprintf(stream, "1");
                    --i;
                    ++count;
                }
                else 
                {
                    total += fprintf(stream, "0");
                    ++count;
                }
            }
            total += fprintf(stream, "1");
        }
        else if (!strcmp(flag, "%Cv\0") || !strcmp(flag, "%CV\0")) 
        {
            long long int to_convert = va_arg(ptr, long long int);
            if (!to_convert) 
            {
                fprintf(stream, "%d", 0);
                continue;
            }
            ++num_of_args;
            int base = va_arg(ptr, int);
            char* converted = NULL;
            const bool is_upper = (!strcmp(flag, "%Cv\0")) ? false : true;

            status_code status = from_decimal_to_base(to_convert, base, is_upper, &converted);
            switch (status)
            {
                case success:
                    total += fprintf(stream, "%s", converted);
                    free(converted);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return -1;
                case invalid_parameter:
                    break;
            }
        }
        else if (!strcmp(flag, "%to\0") || !strcmp(flag, "%TO\0")) 
        {
            char* to_convert = va_arg(ptr, char *);
            int to_convert_size = strlen(to_convert);
            ++num_of_args;
            int base = va_arg(ptr, int);
            long long int result = 0;
            bool is_upper;
            if (!strcmp(flag, "%to\0")) 
            {
                if (!my_islower(to_convert, to_convert_size)) 
                {
                    total += fprintf(stream, "\nLetters must be lowercase\n");
                    return -1;
                }
                is_upper = false;
            }
            else
            {
                if (!my_isupper(to_convert, to_convert_size)) 
                {
                    total += fprintf(stream, "\nLetters must be uppercase\n");
                    return -1;
                }
                is_upper = true;
            }

            status_code status = from_base_to_int(to_convert, to_convert_size, base, is_upper, &result);
            switch (status)
            {
                case success:
                    total += fprintf(stream, "%lld", result);
                    break;
                case invalid_parameter:
                    printf("invalid parameter\n");
                    return -1;
                case allocate_error:
                    break;
            }
        }
        else if (!strcmp(flag, "%mi")) 
        {
            int value = va_arg(ptr, int);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(int); ++i) 
            {
                total += fprintf(stream, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else if (!strcmp(flag, "%mu")) 
        {
            unsigned int value = va_arg(ptr, unsigned int);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(unsigned int); ++i) 
            {
                total += fprintf(stream, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else if (!strcmp(flag, "%md")) 
        {
            double value = va_arg(ptr, double);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(double); ++i) 
            {
                total += fprintf(stream, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else if (!strcmp(flag, "%mf")) 
        {
            float value = va_arg(ptr, double);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(float); ++i) 
            {
                total += fprintf(stream, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else total += vfprintf(stream, flag, ptr);
        while (format[format_index] != '%' && (format_index < format_size)) 
        {
            total += fprintf(stream, "%c", format[format_index]);
            ++format_index;
        }
        tmp_flag = flag;
    }

    if (tmp_flag) free(tmp_flag);
    va_end(ptr);
    return total;
}

int oversprintf(char * buf, char * format, ...) 
{
    int format_size = strlen(format);
    if (!format_size) return -1;
    int num_of_args = 0;

    for (int i = 0; i < format_size; ++i) 
    {
        if (format[i] == '%') ++num_of_args;
    }

    va_list ptr;
    va_start(ptr, format);

    int format_index = 0;
    int total = 0;
    while (format[format_index] != '%' && (format_index < format_size)) 
    {
        total += sprintf(buf + total, "%c", format[format_index]);
        ++format_index;
    }

    char * flag = NULL;
    char * tmp_flag = NULL;

    for (int i = 0; i < num_of_args; ++i) 
    {
        int flag_size = 1, flag_capacity = 2;
        flag = (char *)malloc(sizeof(char) * flag_capacity);
        flag[0] = '%';

        if (tmp_flag) free(tmp_flag);

        while ((isalpha(format[++format_index]) || isdigit(format[format_index]) || (format[format_index] == '%' && format[format_index - 1] == '%') ||
        (format[format_index] == '.' && (isalpha(format[format_index + 1]) || isdigit(format[format_index + 1]) || format[format_index + 1] == '*'))
        || (format[format_index] == '#') || (format[format_index] == '+') || (format[format_index] == '-') || (format[format_index] == '0')) && (format_index < format_size)) 
        {
            flag[flag_size] = format[format_index];
            ++flag_size;
            if (flag_size >= flag_capacity) 
            {
                flag_capacity *= 2;
                char * tmp = (char *)realloc(flag, sizeof(char) * flag_capacity);
                if (tmp == NULL) 
                {
                    free(flag);
                    printf("allocate error\n");
                    return -1;
                }
                flag = tmp;
            }
        }
        flag[flag_size] = '\0';
        if (!strcmp(flag, "%Ro\0")) 
        {
            char* result;

            status_code status = to_roman(va_arg(ptr, int), &result);
            switch (status)
            {
                case success:
                    total += sprintf(buf + total, "%s", result);
                    free(result);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return -1;
                case invalid_parameter:
                    break;
            }
        }
        else if (!strcmp(flag, "%Zr\0")) 
        {
            unsigned int number = va_arg(ptr, unsigned int);
            int result_size;
            unsigned int result[number][2];
            zeckendorf(&result_size, number, result);
            int count = 1;
            int i = result_size - 1;
            while (i >= 0) 
            {
                if (count == result[i][0]) 
                {
                    total += sprintf(buf + total, "1");
                    --i;
                    ++count;
                }
                else 
                {
                    total += sprintf(buf + total, "0");
                    ++count;
                }
            }
            total += sprintf(buf + total, "1");
        }
        else if (!strcmp(flag, "%Cv\0") || !strcmp(flag, "%CV\0")) 
        {
            long long int to_convert = va_arg(ptr, long long int);
            if (!to_convert) 
            {
                total += sprintf(total + buf, "%d", 0);
                continue;
            }
            ++num_of_args;
            int base = va_arg(ptr, int);
            char* converted = NULL;
            const bool is_upper = (!strcmp(flag, "%Cv\0")) ? false : true;

            status_code status = from_decimal_to_base(to_convert, base, is_upper, &converted);
            switch (status)
            {
                case success:
                    total += sprintf(buf + total, "%s", converted);
                    free(converted);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return -1;
                case invalid_parameter:
                    break;
            }
        }
        else if (!strcmp(flag, "%to\0") || !strcmp(flag, "%TO\0")) 
        {
            char* to_convert = va_arg(ptr, char *);
            int to_convert_size = strlen(to_convert);
            ++num_of_args;
            int base = va_arg(ptr, int);
            long long int result = 0;
            bool is_upper;
            if (!strcmp(flag, "%to\0")) 
            {
                if (!my_islower(to_convert, to_convert_size)) 
                {
                    total += sprintf(buf + total, "\nletters must be in lowercase\n");
                    return -1;
                }
                is_upper = false;
            }
            else
            {
                if (!my_isupper(to_convert, to_convert_size)) 
                {
                    total += sprintf(buf + total, "\nletters must be in uppercase\n");
                    return -1;
                }
                is_upper = true;
            }

            status_code status = from_base_to_int(to_convert, to_convert_size, base, is_upper, &result);
            switch (status)
            {
                case success:
                    total += sprintf(buf + total, "%lld", result);
                    break;
                case invalid_parameter:
                    printf("invalid parameter\n");
                    return -1;
                case allocate_error:
                    break;
            }
        }
        else if (strcmp(flag, "%mi") == 0) 
        {
            int value = va_arg(ptr, int);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(int); ++i) 
            {
                total += sprintf(buf + total, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else if (strcmp(flag, "%mu") == 0) 
        {
            unsigned int value = va_arg(ptr, unsigned int);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(unsigned int); ++i) 
            {
                total += sprintf(buf + total, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else if (strcmp(flag, "%md") == 0) 
        {
            double value = va_arg(ptr, double);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(double); ++i) 
            {
                total += sprintf(buf + total, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else if (strcmp(flag, "%mf") == 0) 
        {
            float value = va_arg(ptr, double);
            unsigned char* ptr = (unsigned char*)&value;
            for (int i = 0; i < sizeof(float); ++i) 
            {
                total += sprintf(buf + total, "%u", *ptr % 2);
                ++ptr;
            }
        }
        else total += vsprintf(buf + total, flag, ptr);
        while (format[format_index] != '%' && (format_index < format_size)) 
        {
            total += sprintf(buf + total, "%c", format[format_index]);
            ++format_index;
        }
        tmp_flag = flag;
    }

    if (tmp_flag) free(tmp_flag);
    va_end(ptr);
    return total;
}

int main() 
{
    overfprintf(stdout, "testdrive %Cv %Ro %Ro %CV %mi %mi\n", 33, 2, 15, 10, 10, 16, 65, 5);
    return 0;
}