#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum status_code {
    ok,
    allocate_error,
};

int get_rand(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

void print_str(char* str) 
{
    if (str[0] == '\0') return;
    for (int i = 0; str[i] != '\0'; ++i) 
    {
        printf("%c", str[i]);
    }
    printf("\n");
}

int my_strlen(char* str) 
{
    int len = 0;
    for (int i = 0; str[i] != '\0'; ++i) ++len;
    return len;
}

bool my_isalpha(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool my_isdigit(char c) 
{
    return (c >= '0' && c <= '9');
}

bool my_isupper(char c) 
{
    return (c >= 'A' && c <= 'Z');
}

enum status_code reverse_string(char* str, char** res_str) 
{
    int str_len = my_strlen(str);
    *res_str = (char*)malloc(sizeof(char) * (str_len + 1));
    if (*res_str == NULL)
    {
        return allocate_error;
    }
    for (int i = str_len - 1; i >= 0; --i) 
    {
       (*res_str)[str_len - i - 1] = str[i];
    }
    (*res_str)[str_len] = '\0';
    return ok;
}

enum status_code odd_pos_upper(char* str, char** res_str) 
{
    int str_len = my_strlen(str);
    *res_str = (char*)malloc(sizeof(char) * (str_len + 1));
    if (*res_str == NULL) 
    {
        return allocate_error;
    }
    for (int i = 0; i < str_len; ++i) 
    {
        if (i % 2 == 0 || !my_isalpha(str[i]) || my_isupper(str[i])) 
        {
            (*res_str)[i] = str[i];
        }
        else 
        {
            (*res_str)[i] = str[i] - 'a' + 'A';
        }
    }
    (*res_str)[str_len] = '\0';
    return ok;
}

enum status_code sort_by_group(char* str, char** res_str) 
{
    int str_len = 0, digits_count = 0, letters_count = 0, other_count = 0;
    for (int i = 0; str[i] != '\0'; ++i) 
    {
        ++str_len;
        if (my_isalpha(str[i])) ++letters_count;
        else if (my_isdigit(str[i])) ++ digits_count;
        else ++other_count;
    }
    *res_str = (char*)malloc(sizeof(char*) * str_len);
    if (*res_str == NULL) 
    {
        free(*res_str);
        return allocate_error;
    }
    int digits_ins = 0, letters_ins = 0, other_ins = 0;
    for (int i = 0; i < str_len; ++i) 
    {
        if (my_isalpha(str[i])) 
        {
            (*res_str)[digits_count + letters_ins] = str[i];
            ++letters_ins;
        }
        else if (my_isdigit(str[i])) 
        {
            (*res_str)[digits_ins] = str[i];
            ++digits_ins;
        }
        else 
        {
            (*res_str)[digits_count + letters_count + other_ins] = str[i];
            ++other_ins;
        }
    }
    return ok;
}

enum status_code concat_strings(char* strings[], int len_of_strings, char** res_str, unsigned int RANDOM_SEED) 
{
    int total_len = my_strlen(strings[2]);
    for (int i = 4; i < len_of_strings; ++i) total_len += my_strlen(strings[i]);
    *res_str = (char*)malloc(sizeof(char*) * total_len);
    if (*res_str == NULL) 
    {
        free(*res_str);
        return allocate_error;
    }
    srand(RANDOM_SEED);
    int unique_rand_nums[len_of_strings - 3];
    int idx = 0;    
    for (int i = 0; i < len_of_strings - 3; ++i)
    {
        bool is_unique;
        do
        {
            unique_rand_nums[i] = get_rand(4, len_of_strings);
            is_unique = true;
            unique_rand_nums[i] = (unique_rand_nums[i] == len_of_strings) ? 2 : unique_rand_nums[i];
            for (int j = 0; j < i; ++j) 
            {
                if (unique_rand_nums[j] == unique_rand_nums[i]) 
                {
                    is_unique = false;
                    break;
                } 
            } 
        } while (!is_unique);
        for (int j = 0; strings[unique_rand_nums[i]][j] != '\0'; ++j) 
        {
            (*res_str)[idx] = strings[unique_rand_nums[i]][j];
            ++idx;
        }
    }
    return ok;
}

int main(int argc, char* argv[]) 
{
    if (argc < 3) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    const char flag = argv[1][1];

    int str_len;
    char* reversed_str = NULL;
    char* odd_pos_upped_str = NULL;
    char* sorted_by_group_str = NULL;
    char* concated_str = NULL;
    unsigned int RANDOM_SEED;

    switch (flag)
    {
        case 'l':
            str_len = my_strlen(argv[2]);
            printf("len : %d\n", str_len);
            break;
        case 'r':
            switch (reverse_string(argv[2], &reversed_str))
            {
                case ok:
                    printf("reversed string : ");
                    print_str(reversed_str);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return 1;
            }
            break;
        case 'u':
            switch (odd_pos_upper(argv[2], &odd_pos_upped_str))
            {
                case ok:
                    printf("odd pos upped string : ");
                    print_str(odd_pos_upped_str);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return 1;
            }
            break;
        case 'n':
            switch (sort_by_group(argv[2], &sorted_by_group_str))
            {
                case ok:
                    printf("sorted by group string : ");
                    print_str(sorted_by_group_str);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return 1;
            }
            break;
        case 'c':
            if (argc < 4) 
            {
                printf("invalid number of arguments\n");
                return 1;
            }
            if (sscanf(argv[3], "%d", &RANDOM_SEED) == 0) 
            {
                printf("seed for rand must have unsigned int type\n");
                return 1;
            }
            switch (concat_strings(argv, argc, &concated_str, RANDOM_SEED))
            {
                case ok:
                    printf("concated string : ");
                    print_str(concated_str);
                    break;
                case allocate_error:
                    printf("allocate error\n");
                    return 1;
            }
            break;
        default:
            printf("unknown flag\n");
            return 1;
    }

    if (reversed_str) free(reversed_str);
    if (odd_pos_upped_str) free(odd_pos_upped_str);
    if (sorted_by_group_str) free(sorted_by_group_str);
    if (concated_str) free(concated_str);


    return 0;
}