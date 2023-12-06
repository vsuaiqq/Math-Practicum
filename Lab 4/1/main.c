#include "utils.h"
#include "hash_table.h"

status_code solve(const char* file_path, hash_table** table, unsigned long long int (*hash_function)(const char* def_name)) 
{
    if (create_table(table, START_HASH_SIZE) == ALLOCATE_ERROR) 
    {
        return ALLOCATE_ERROR;
    }

    FILE* file = fopen(file_path, "r");
    if (!file) 
    {
        free_table(*table);
        return FILE_ERROR;
    }
    FILE* tmp_file = fopen("tmp_file.txt", "w");
    if (!tmp_file) 
    {
        fclose(file);
        free_table(*table);
        return FILE_ERROR;
    }

    char* line = NULL;
    int offset;
    while (getline(&line, &(size_t){0}, file) != -1) 
    {
        char def_name[BUFSIZ];
        char value[BUFSIZ];
        if (sscanf(line, "#define %s %s", def_name, value) == 2) 
        {
            if (insert_table(*table, def_name, value, false, 0, hash_function) == ALLOCATE_ERROR) 
            {
                free(line);
                fclose(file);
                fclose(tmp_file);
                free_table(*table);
                return ALLOCATE_ERROR;
            }
            fputs(line, tmp_file);
            continue;
        }
        offset = -strlen(line);
        free(line);
        break;
    }
    
    fseek(file, offset, SEEK_CUR);

    char c = fgetc(file);
    int word_size = 0, word_capacity = 2;
    char* word = (char*)malloc(sizeof(char) * word_capacity);
    if (!word) 
    {
        fclose(file);
        fclose(tmp_file);
        free_table(*table);
        return ALLOCATE_ERROR;
    }
    while (c != EOF) 
    {
        while (!isspace(c) && c != EOF) 
        {
            word[word_size] = c;
            if (word_size + 1 >= word_capacity) 
            {
                word_capacity *= 2;
                char* tmp = (char*)realloc(word, sizeof(char) * word_capacity);
                if (!tmp) 
                {
                    free(word);
                    fclose(file);
                    fclose(tmp_file);
                    free_table(*table);
                    return ALLOCATE_ERROR;
                }
                word = tmp;
            }
            ++word_size;
            c = fgetc(file);
        }
        if (isspace(c) || c == EOF) 
        {
            word[word_size] = '\0';
            const char* find_res = find_in_table(*table, word, hash_function);
            if (!find_res) 
            {
                fprintf(tmp_file, "%s", word);
            }
            else 
            {
                fprintf(tmp_file, "%s", find_res);
            }
            if (c != EOF) fprintf(tmp_file, "%c", c);
            free(word);
            word = NULL;
            word_size = 0, word_capacity = 2;
            word = (char*)malloc(sizeof(char) * word_capacity);
            if (!word) 
            {
                fclose(file);
                fclose(tmp_file);
                free_table(*table);
                return ALLOCATE_ERROR;
            }
            c = fgetc(file);
        }
    }
    free(word);

    if (rename("tmp_file.txt", file_path)) 
    {
        fclose(file);
        fclose(tmp_file);
        free_table(*table);
        return FILE_ERROR;
    }

    fclose(file);
    fclose(tmp_file);

    return SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        print_err(ARGUMENTS_ERROR);
        return ARGUMENTS_ERROR;
    }

    hash_table* table = NULL;

    const status_code st = solve(argv[1], &table, hash_function);
    if (st != SUCCESS) 
    {
        print_err(st);
        return st;
    }

    free_table(table);

    return SUCCESS;
}