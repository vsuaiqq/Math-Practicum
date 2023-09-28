#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool is_latin(char c) 
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void remove_digits(FILE* in_file, FILE* out_file) 
{
    char cur_symbol;
    while ((cur_symbol = fgetc(in_file)) != EOF)  
    {
        if (!isdigit(cur_symbol)) 
        {
            fputc(cur_symbol, out_file);
        }
    }
}

void count_latin(FILE* in_file, FILE* out_file) 
{
    char cur_symbol, prev_symbol;
    int counter = 0, prev_counter = 1, counter_len = 1;
    char* counter_str;
    while ((cur_symbol = fgetc(in_file)) != EOF)  
    {
        if (is_latin(cur_symbol)) 
        {
            ++counter;
            if (counter == 10 * prev_counter) 
            {
                prev_counter = counter;
                ++counter_len;
            }
        }
        if (cur_symbol == '\n') 
        {
            counter_str = (char*)malloc(sizeof(char*) * (counter_len + 1));
            sprintf(counter_str, "%d\n", counter);
            fputs(counter_str, out_file);
            counter = 0, counter_len = 0;
            free(counter_str);
        }
    }
    if (prev_symbol != '\n') 
    {
        counter_str = (char*)malloc(sizeof(char*) * (counter_len + 1));
        sprintf(counter_str, "%d\n", counter);
        fputs(counter_str, out_file);
        free(counter_str);
    }
}

void s_flag_func(FILE* in_file, FILE* out_file) 
{
    char cur_symbol, prev_symbol;
    int counter = 0, prev_counter = 1, counter_len = 1;
    char* counter_str;
    while ((cur_symbol = fgetc(in_file)) != EOF)  
    {
        if (!is_latin(cur_symbol) && cur_symbol != ' ' && !isdigit(cur_symbol)) 
        {
            ++counter;
            if (counter == 10 * prev_counter) 
            {
                prev_counter = counter;
                ++counter_len;
            }
        }
        if (cur_symbol == '\n') 
        {
            counter_str = (char*)malloc(sizeof(char*) * (counter_len + 1));
            sprintf(counter_str, "%d\n", counter);
            fputs(counter_str, out_file);
            counter = 0, counter_len = 0;
            free(counter_str);
        }
    }
    if (prev_symbol != '\n') 
    {
        counter_str = (char*)malloc(sizeof(char*) * (counter_len + 1));
        sprintf(counter_str, "%d\n", counter);
        fputs(counter_str, out_file);
        free(counter_str);
    }
}

void replace_to_ascii(FILE* in_file, FILE* out_file) 
{
    char cur_symbol;
    char* hex_ascii = (char*)malloc(sizeof(char*) * 3);
    while ((cur_symbol = fgetc(in_file)) != EOF)  
    {
        if (!isdigit(cur_symbol)) 
        {
            sprintf(hex_ascii, " %X ", (int)cur_symbol);
            fputs(hex_ascii, out_file);
        }
        else 
        {
            fputc(cur_symbol, out_file);
        }
    }
    free(hex_ascii);
}

int main(int argc, char* argv[])
{
    FILE* input_file = NULL;
    FILE* output_file = NULL;
    char* output_file_name = NULL;
    char* output_file_prefix = "output_";
    char flag;
    
    if (argc < 3 || argc > 4) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') 
    {
        printf("invalid flag\n");
        return 1;
    }

    input_file = fopen(argv[2], "r");
    if (!input_file) 
    {
        printf("file open error\n");
        return 1;
    }

    if (argv[1][1] == 'n')
    {
        if (argc != 4) 
        {
            printf("invalid number of arguments\n");
            return 1;
        }

        output_file = fopen(argv[3], "w");
        if (!output_file) 
        {
            printf("file open error\n");
            return 1;
        }

        flag = argv[1][2];
    }
    else 
    {
        if (argc != 3) 
        {
            printf("invalid number of arguments\n");
            return 1;
        }

        int i = strlen(argv[2]) - 1;
        while (argv[2][i] != '/' && i >= 0) 
        {
            --i;
        }
        ++i;

        output_file_name = (char*)malloc(sizeof(char*) * (strlen(argv[2]) + strlen(output_file_prefix) + 1));
        
        for (int j = 0; j < strlen(output_file_prefix); ++j) 
        {
            output_file_name[j] = output_file_prefix[j];
        }
        int ins_idx = strlen(output_file_prefix);
        for (int j = i; j < strlen(argv[2]); ++j) 
        {
            output_file_name[ins_idx] = argv[2][j];
            ++ins_idx;
        }

        output_file = fopen(output_file_name, "w");
        if (!output_file) 
        {
            printf("file open error\n");
            return 1;
        }

        flag = argv[1][1];
    }

    switch (flag)
    {
        case 'd':
            remove_digits(input_file, output_file);
            break;
        case 'i':
            count_latin(input_file, output_file);
        case 's':
            s_flag_func(input_file, output_file);
        case 'a':
            replace_to_ascii(input_file, output_file);
        default:
            printf("invalid flag\n");
            return 1;
    }

    if (output_file_name) 
    {
        free(output_file_name);
    }

    fclose(input_file);
    fclose(output_file);
}