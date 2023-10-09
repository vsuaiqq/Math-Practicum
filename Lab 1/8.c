#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_STR_SIZE 32

enum read_lexeme_status_code 
{
    rl_ok,
    rl_invalid,
    rl_allocate_err,
};

enum to_decimal_status_code 
{
    td_ok,
    td_overflow,
};

bool is_valid_char(char c) 
{
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A') && (c <= 'Z'));
}

int char_to_num(char c) 
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'z') return c - 87;
    if (c >= 'A' && c <= 'Z') return c - 55;
}


enum to_decimal_status_code to_decimal(char* str, int base, bool is_negative, int* res) 
{
    *res = 0;
    for (int i = 0 + (int)is_negative; str[i] != '\0'; ++i) 
    {
        if (*res > INT_MAX / base) return td_overflow;
        *res = (*res) * base;
        int digit = char_to_num(str[i]);
        if (*res > INT_MAX - digit) return td_overflow;
        *res += digit;
    }
    *res = ((is_negative) ? -(*res) : (*res));
    return td_ok;
}

enum read_lexeme_status_code read_lexeme (FILE* file, char** res_str, int* min_base) 
{
    *res_str = (char*)malloc(sizeof(char) * (MAX_STR_SIZE + 1));
    if (*res_str == NULL) 
    {
        free(*res_str);
        return rl_allocate_err;
    }
    int cur_size = MAX_STR_SIZE, char_cnt = 0;
    char c = fgetc(file);
    while (!isspace(c) && c != EOF) 
    {
        if (!is_valid_char(c) && char_cnt != '0' && c != '-') 
        {
            free(*res_str);
            return rl_invalid;
        }
        if (char_to_num(c) + 1 > *min_base) *min_base = char_to_num(c) + 1;
        ++char_cnt;
        if (char_cnt > MAX_STR_SIZE) 
        {
            cur_size *= 2;
            *res_str = (char*)realloc(*res_str, cur_size);
            if (*res_str == NULL) 
            {
                free(*res_str);
                return rl_allocate_err;
            }
        }
        (*res_str)[char_cnt - 1] = c;
        c = fgetc(file);
    }
    (*res_str)[char_cnt] = '\0';
    return rl_ok;
}

void write_lexem_skip_trailing_zeros(char* str, FILE* file) 
{
    int idx = 0;
    if (str[0] == '-') 
    { 
        fputc('-', file); 
        ++idx; 
    }
    while (str[idx] == '0') ++idx;
    for (int i = idx; str[i] != '\0'; ++i) 
    {
        fputc(str[i], file); 
    }
    fputc(' ', file);
}

void write_min_base(int base, FILE* file) 
{
    fprintf(file, "%d ", base);
}

void write_decimal(int decimal, FILE* file) 
{
    fprintf(file, "%d", decimal);
}

int main (int argc, char* argv[]) 
{
    if (argc < 3) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    if (input_file == NULL || output_file == NULL) 
    {
        printf("file error\n");
        return 1;
    }

    char c = fgetc(input_file);
    while (c != EOF) 
    {
        while (isspace(c)) c = fgetc(input_file);
        ungetc(c, input_file);

        char* cur_lexem = NULL;
        int min_base = 2;

        switch (read_lexeme(input_file, &cur_lexem, &min_base))
        {
            case rl_ok:
                break;
            case rl_invalid:
                printf("invalid value\n");
                fclose(input_file);
                fclose(output_file);
                return 1;
            case rl_allocate_err:
                printf("allocate error\n");
                fclose(input_file);
                fclose(output_file);
                return 1;
        }

        int lexem_decimal;
        bool is_negative = ((cur_lexem[0] == '-') ? true : false);

        switch (to_decimal(cur_lexem, min_base, is_negative, &lexem_decimal))
        {
            case td_ok:
                break;
            case td_overflow:
                printf("overflow\n");
                free(cur_lexem);
                fclose(input_file);
                fclose(output_file);
                return 1;
        }
        
        write_lexem_skip_trailing_zeros(cur_lexem, output_file);
        write_min_base(min_base, output_file);
        write_decimal(lexem_decimal, output_file);
        fputc('\n', output_file);

        free(cur_lexem);

        c = fgetc(input_file);
    }
    
    fclose(input_file);
    fclose(output_file);

    return 0;
}