#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define START_CAPACITY 33

typedef enum status_code 
{
    success,
    allocate_error,
} status_code;

typedef struct Entrance 
{
    int line_num, first_entrance;
} Entrance;

void print_entrances(const int entrances_size, Entrance entrances[]) 
{
    for (int i = 0; i < entrances_size; ++i) 
    {
        printf("   %d %d\n", entrances[i].line_num, entrances[i].first_entrance);
    }
}

status_code find_substr_in_file(FILE* file, const char* substr, Entrance** entrances, int* entrances_size) {
    *entrances = (Entrance*)malloc(sizeof(Entrance) * START_CAPACITY);
    if (*entrances == NULL) return allocate_error;
    *entrances_size = 0;

    const int substr_len = strlen(substr);
    int cur_capacity = START_CAPACITY;
    int substr_idx = 0;
    int first_in_line_num = -1;
    int first_in_char_pos = -1;
    int line_num = 1; 
    int char_pos = 0;

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
                if (*entrances_size >= cur_capacity) 
                {
                    Entrance* tmp = (Entrance*)realloc(*entrances, sizeof(Entrance) * cur_capacity * 2);
                    if (tmp == NULL) 
                    {
                        free(*entrances);
                        return allocate_error;
                    }
                    cur_capacity *= 2;
                    *entrances = tmp;
                }
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

    Entrance* tmp = (Entrance*)realloc(*entrances, sizeof(Entrance) * (*entrances_size));
    if (tmp == NULL) 
    {
        free(*entrances);
        return allocate_error;
    }
    *entrances = tmp;

    return success;  
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        printf("invalid number of parameters\n");
        return 1;
    }

    const char* substr_to_find = argv[1];
    //const char* substr_to_find = "abc";
    //const char* substr_to_find = "ab\nc";
    //const char* substr_to_find = "\n";
    for (int i = 2; i < argc; ++i) 
    {
        FILE* file = fopen(argv[i], "r");
        if (file == NULL) 
        {
            printf("file open error\n");
            return 1;
        }

        Entrance* entrances;
        int entrances_size;

        switch (find_substr_in_file(file, substr_to_find, &entrances, &entrances_size))
        {
            case success:
                printf("%s\n", argv[i]);
                print_entrances(entrances_size, entrances);
                break;
            case allocate_error: 
                printf("allocate error!\n");
                fclose(file);
                return 1;
        }

        free(entrances);
        fclose(file);
    }

    return 0;
}