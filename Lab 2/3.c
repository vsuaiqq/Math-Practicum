#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum status_code 
{
    success,
    file_error,
    allocate_error,
} status_code;

typedef struct Entrance 
{
    char* file_name;
    int line_num, first_entrance;
} Entrance;

void print_entrances(const int entrances_size, Entrance entrances[]) 
{
    for (int i = 0; i < entrances_size; ++i) 
    {
        printf("%s %d %d\n", entrances[i].file_name, entrances[i].line_num, entrances[i].first_entrance);
    }
}

status_code find_substr_in_file(FILE* file, const char* substr, const char* file_name, 
                                Entrance** entrances, int* entrances_size, int* entrances_capacity) 
{
    const int substr_len = strlen(substr);
    
    int substr_idx = 0;
    int line_num = 1; 
    int char_pos = 0;

    int first_in_line_num = -1;
    int first_in_char_pos = -1;

    bool is_found = false;

    char c;
    while ((c = fgetc(file)) != EOF) 
    {
        if (substr[substr_idx] == '\t' && c == ' ') 
        {
            int cnt;
            for (cnt = 0; cnt < 3; ++cnt) 
            {
                c = fgetc(file);
                if (c != ' ') break;
            }
            if (cnt == 3) c = '\t';
        }

        if (c == substr[substr_idx]) 
        {
            if (!is_found && first_in_char_pos == -1) 
            {
                is_found = true;

                first_in_line_num = line_num;
                first_in_char_pos = char_pos;
            }

            ++substr_idx;

            if (substr_idx == substr_len) 
            {  
                if (*entrances_size >= (*entrances_capacity))
                {
                    Entrance* tmp = (Entrance*)realloc(*entrances, sizeof(Entrance) * (*entrances_capacity) * 2);
                    if (tmp == NULL) 
                    {
                        free(*entrances);
                        return allocate_error;
                    }
                    *entrances_capacity *= 2;
                    *entrances = tmp;
                } 
                (*entrances)[*entrances_size].file_name = strdup(file_name);
                (*entrances)[*entrances_size].line_num = first_in_line_num;
                (*entrances)[*entrances_size].first_entrance = first_in_char_pos;
                ++(*entrances_size);

                fseek(file, 1 - substr_len, SEEK_CUR);
                char_pos -= 1 - substr_len;

                substr_idx = 0;

                is_found = false;

                first_in_line_num = -1;
                first_in_char_pos = -1;
            }
        } 
        else 
        {
            substr_idx = 0;

            is_found = false;

            first_in_line_num = -1;
            first_in_char_pos = -1;
        }
        
        char_pos++;
        
        if (c == '\n') 
        {
            ++line_num;
            char_pos = 0;
        }
    }
    /*
    Entrance* tmp = (Entrance*)realloc(*entrances, sizeof(Entrance) * (*entrances_size));
    if (tmp == NULL) 
    {
        free(*entrances);
        return allocate_error;
    }
    *entrances = tmp;
    */
    return success;  
}

status_code find_substr_in_all_files(char* substr, Entrance** entrances, int* entrances_size, int num_of_files, ...) 
{
    int entrances_capacity = 32;
    *entrances = (Entrance*)malloc(sizeof(Entrance) * entrances_capacity);
    if (*entrances == NULL) return allocate_error;
    *entrances_size = 0;
    va_list ptr;
    va_start(ptr, num_of_files);
    for (int i = 0; i < num_of_files; ++i) 
    {
        const char* file_name = va_arg(ptr, const char*);
        FILE* file = fopen(file_name, "r");
        if (file == NULL) 
        {
            if ((*entrances) != NULL) free(*entrances);
            va_end(ptr);
            return file_error;
        }
        if (find_substr_in_file(file, substr, file_name, entrances, entrances_size, &entrances_capacity) == allocate_error) 
        {
            va_end(ptr);
            fclose(file);
            return allocate_error;
        }
        fclose(file);
    }
    va_end(ptr);
    return success;
}

int main() 
{
    char* substr_to_find = "abc";

    Entrance* entrances;
    int entrances_size;

    switch (find_substr_in_all_files(substr_to_find, &entrances, &entrances_size, 3, "a.txt", "b.txt", "c.txt"))
    {
        case success:
            print_entrances(entrances_size, entrances);
            break;
        case file_error:
            printf("file error!\n");
            return 1;
        case allocate_error:
            printf("allocate error!\n");
            return 1;
    }

    free(entrances);

    return 0;
}