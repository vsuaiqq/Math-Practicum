#include "utils.h"
#include "stack.h"
#include "task.h"

status_code solve(const int num_of_files, char* file_pathes[]) 
{
    for (int i = 1; i < num_of_files; ++i) 
    {
        int split_pos = 0;
        while (file_pathes[i][split_pos] != '.') ++split_pos;
        const int error_filename_len = split_pos + 12;
        char* error_filename = (char*)malloc(sizeof(char) * error_filename_len);
        if (!error_filename) return ALLOCATE_ERROR;
        const char* error_filename_postfix = "_errors.txt";
        int idx = 0;
        for (int j = 0; j < error_filename_len - 1; ++j) 
        {
            if (j < split_pos) 
            {
                error_filename[j] = file_pathes[i][j];
                continue;
            }
            error_filename[j] = error_filename_postfix[idx++];
        }
        error_filename[error_filename_len - 1] = '\0';

        FILE* file = fopen(file_pathes[i], "r");
        if (!file) 
        {
            free(error_filename);
            return FILE_ERROR;
        }
        FILE* errors_file = NULL;
        char* infix = NULL;
        int line = 0;
        int getline_res;
        while ((getline_res = getline(&infix, &(size_t){0}, file)) != -1) 
        {
            if (infix[strlen(infix) - 1] == '\n') infix[strlen(infix) - 1] = '\0';

            char* postfix = NULL;
            status_code st = to_postfix(&postfix, infix);
            if (st != SUCCESS && st != ALLOCATE_ERROR) 
            {
                if (!errors_file) 
                {
                    errors_file = fopen(error_filename, "w");
                    if (!errors_file) 
                    {
                        free(error_filename);
                        free(infix);
                        free(postfix);
                        fclose(file);
                        return FILE_ERROR;
                    }
                }
                print_error(errors_file, st, infix, line);
                free(infix);
                infix = NULL;
                line++;
                continue;
            }
            if (st == ALLOCATE_ERROR) 
            {
                free(error_filename);
                free(infix);
                free(postfix);
                fclose(file);
                return ALLOCATE_ERROR;
            }

            int value = 0;
            st = calculate_postfix(postfix, &value);
            if (st != SUCCESS) 
            {
                if (st == ALLOCATE_ERROR) 
                {
                    free(error_filename);
                    free(infix);
                    free(postfix);
                    fclose(file);
                    return ALLOCATE_ERROR;
                }
                if (!errors_file) 
                {
                    errors_file = fopen(error_filename, "w");
                    if (!errors_file) 
                    {
                        free(error_filename);
                        free(infix);
                        free(postfix);
                        fclose(file);
                        return FILE_ERROR;
                    }
                }
                print_error(errors_file, st, infix, line);
                free(postfix);
                free(infix);
                infix = NULL;
                line++;
                continue;
            }

            print_expr(file_pathes[i], infix, postfix, value);
            free(infix);
            free(postfix);
            infix = NULL;
            line++;
        }

        free(error_filename);
        free(infix);
        fclose(file);
        if (errors_file) fclose(errors_file);
    }
    
    return SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        print_std_error(ARGUMENTS_ERROR);
        return ARGUMENTS_ERROR;
    }

    const status_code st = solve(argc, argv);
    if (st != SUCCESS) print_std_error(st);

    return st;
}