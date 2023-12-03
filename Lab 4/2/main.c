#include "arrays.h"
#include "utils.h"

status_code solve(FILE* instructions, Arrays** arrays) 
{
    if (create_arrays(arrays) == ALLOCATE_ERROR) return ALLOCATE_ERROR;
    char* line = NULL;
    int getline_res;
    while ((getline_res = getline(&line, &(size_t){0}, instructions)) != -1) 
    {
        if (!validate_command(line)) 
        {
            free(line);
            return INVALID_DATA;
        }

        str_to_lower(&line);
        const int command_len = strlen(line);

        int num1, num2, num3;
        char name1, name2;
        char file_path[BUFSIZ];

        if (sscanf(line, "load %c, %s;", &name1, file_path) == 2) 
        {
            if (!validate_name(name1)) 
            {
                free(line);
                return INVALID_DATA;
            }
            file_path[strlen(file_path) - 1] = '\0';
            FILE* input_file = fopen(file_path, "r");
            if (!input_file) 
            {
                free(line);
                return FILE_ERROR;
            }
            const status_code st = load(*arrays, name1, input_file);
            if (st != SUCCESS) 
            {
                free(line);
                fclose(input_file);
                return st;
            }
            fclose(input_file);
        }
        else if (sscanf(line, "save %c, %s;", &name1, file_path) == 2) 
        {
            if (!validate_name(name1)) 
            {
                free(line);
                return INVALID_DATA;
            }
            file_path[strlen(file_path) - 1] = '\0';
            FILE* output_file = fopen(file_path, "w");
            if (!output_file) 
            {
                free(line);
                return FILE_ERROR;
            }
            if (save(*arrays, name1, output_file) == NOT_FOUND) 
            {
                free(line);
                fclose(output_file);
                return NOT_FOUND; 
            }
            fclose(output_file);
        }
        else if (sscanf(line, "rand %c, %d, %d, %d;", &name1, &num1, &num2, &num3) == 4) 
        {
            if (!validate_name(name1) || (13 + int_len(num1) + int_len(num2) + int_len(num3) != command_len)) return INVALID_DATA;
            const status_code st = rand_fill(*arrays, name1, num1, num2, num3);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        } 
        else if (sscanf(line, "concat %c, %c;", &name1, &name2) == 2) 
        {
            if (!validate_name(name1) || !validate_name(name2) || (command_len != 12)) return INVALID_DATA;
            const status_code st = concat(*arrays, name1, name2);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        } 
        else if (sscanf(line, "free(%c);", &name1) == 1) 
        {
            if (!validate_name(name1) || (command_len != 8)) return INVALID_DATA;
            if (remove_array(*arrays, name1) == NOT_FOUND) 
            {
                free(line);
                return NOT_FOUND;
            }
        }
        else if (sscanf(line, "remove %c, %d, %d;", &name1, &num1, &num2) == 3) 
        {
            if (!validate_name(name1) || (13 + int_len(num1) + int_len(num2) != command_len)) return INVALID_DATA;
            const status_code st = remove_elements(*arrays, name1, num1, num2);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "copy %c, %d, %d, %c;", &name1, &num1, &num2, &name2) == 4) 
        {
            if (!validate_name(name1) || !validate_name(name2) 
                || (14 + int_len(num1) + int_len(num2) != command_len)) return INVALID_DATA;
            const status_code st = copy(*arrays, name2, name1, num1, num2);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "sort %c;", &name1) == 1) 
        {
            if (!validate_name(name1) || (command_len != 8)) return INVALID_DATA;
            const sort_type type = get_sort_type(line);
            if (type == INVALID) 
            {
                free(line);
                return INVALID_DATA;
            }
            const status_code st = sort(*arrays, name1, type);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "shuffle %c;", &name1) == 1) 
        {
            if (!validate_name(name1) || (command_len != 10)) return INVALID_DATA;
            const status_code st = sort(*arrays, name1, RANDOM);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "stats %c;", &name1) == 1) 
        {
            if (!validate_name(name1) || (command_len != 8)) return INVALID_DATA;
            const status_code st = stats(*arrays, name1);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "print %c, %d, %d;", &name1, &num1, &num2) == 3) 
        {
            if (!validate_name(name1) || (12 + int_len(num1) + int_len(num2) != command_len)) return INVALID_DATA;
            const status_code st = print_arr_slice(*arrays, name1, num1, num2);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "print %c, %d;", &name1, &num1) == 2) 
        {
            if (!validate_name(name1) || (10 + int_len(num1) != command_len)) return INVALID_DATA;
            const status_code st = print_arr_slice(*arrays, name1, num1, num1);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else if (sscanf(line, "print %c, all;", &name1) == 1) 
        {
            if (!validate_name(name1) || (command_len != 13)) return INVALID_DATA;
            const status_code st = print_arr(*arrays, name1);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
        }
        else 
        {
            free(line);
            return INVALID_DATA;
        }

        free(line);
        line = NULL;
    }

    return SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        print_error(INVALID_DATA);
        return INVALID_DATA;
    }

    FILE* instructions = fopen(argv[1], "r");
    if (!instructions) 
    {
        print_error(FILE_ERROR);
        return FILE_ERROR;
    }

    Arrays* arrays = NULL;

    const status_code st = solve(instructions, &arrays);

    print_error(st);

    free_arrays(arrays);
    fclose(instructions);

    return st;
}