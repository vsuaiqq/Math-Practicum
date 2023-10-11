#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

int from_decimal(int x, int k) 
{
    int res = 0, n = 1;
    while (x > 0) 
    {
        int last_digit = x % k;
        res += last_digit * n;
        x /= k;
        n *= 10;
    }
    return res;
}

void flag_r_func(FILE* in_file_1, FILE* in_file_2, FILE* out_file) 
{
    char c_1 = fgetc(in_file_1), c_2 = fgetc(in_file_2);
    const char sep = ' ';
    while (c_1 != EOF || c_2 != EOF)  
    {
        while (isspace(c_1)) c_1 = fgetc(in_file_1);
        while (isspace(c_2)) c_2 = fgetc(in_file_2);
        while (!isspace(c_1) && c_1 != EOF) 
        {
            fputc(c_1, out_file);
            c_1 = fgetc(in_file_1);
        }
        if (c_2 != EOF) fputc(sep, out_file);
        while (!isspace(c_2) && c_2 != EOF) 
        {
            fputc(c_2, out_file);
            c_2 = fgetc(in_file_2);
        }
        if (c_1 != EOF) fputc(sep, out_file);
    }
}

void flag_a_func(FILE* in_file, FILE* out_file) 
{
    const char sep = ' ';
    int lex_pos = 0;
    char c = fgetc(in_file);
    while (c != EOF) 
    {
        while (isspace(c)) c = fgetc(in_file);
        ++lex_pos;
        if (lex_pos % 10 == 0) 
        {
            while (!isspace(c) && c != EOF) 
            {
                if (isalpha(c)) 
                {
                    fprintf(out_file, "%d", from_decimal(toascii(tolower(c)), 4));
                }
                else fputc(c, out_file);
                c = fgetc(in_file);
            }
        }
        else if (lex_pos % 2 == 0) 
        {   
            while (!isspace(c)) 
            {
                if (isalpha(c)) fputc(tolower(c), out_file);
                else fputc(c, out_file);
                c = fgetc(in_file);
            }
        }
        else if (lex_pos % 5 == 0) 
        {
            while (!isspace(c) && c != EOF) 
            {
                fprintf(out_file, "%d", from_decimal(toascii(c), 8));
                c = fgetc(in_file);
            }
        }
        else 
        {
            while (!isspace(c) && c != EOF) 
            {
                fputc(c, out_file);
                c = fgetc(in_file);
            }
        }
        fputc(sep, out_file);
    }
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }
    if (argv[1][0] != '-' && argv[1][0] != '/')
    {
        printf("invalid flag\n");
        return 1;
    }

    const char flag = argv[1][1];
    FILE* file_1;
    FILE* file_2;
    FILE* file_3;

    switch (flag)
    {
        case 'r':
            if (argc < 5) 
            {
                printf("invalid number of arguments\n");
                return 1;
            }

            file_1 = fopen(argv[2], "r");
            file_2 = fopen(argv[3], "r");
            file_3 = fopen(argv[4], "w");
            if (file_1 == NULL || file_2 == NULL || file_3 == NULL) 
            {
                printf("file error\n");
                return -1;
            }

            flag_r_func(file_1, file_2, file_3);

            fclose(file_1);
            fclose(file_2);
            fclose(file_3);

            break;

        case 'a':
            if (argc < 4) 
            {
                printf("invalid number of arguments\n");
                return 1;
            }

            file_1 = fopen(argv[2], "r");
            file_2 = fopen(argv[3], "w");
            if (file_1 == NULL || file_2 == NULL) 
            {
                printf("file error\n");
                return -1;
            }

            flag_a_func(file_1, file_2);

            fclose(file_1);
            fclose(file_2);
            break;

        default:
            printf("invalid flag\n");
            return 1;
    }
    return 0;
}