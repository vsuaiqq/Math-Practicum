#include "utils.h"
#include "memorycell.h"
#include "storage.h"

status_code solve(FILE* instructions, Storage** storage) 
{
    if (init_storage(storage) == ALLOCATE_ERROR) return ALLOCATE_ERROR;
    char* line = NULL;
    int getline_res;
    while ((getline_res = getline(&line, &(size_t){0}, instructions)) != -1) 
    {
        const cmd_type type = get_cmd_type(line);
        status_code st;

        if (type == print_all) 
        {
            print(*storage, -1);
        }
        else if (type == print_single) 
        {
            char* key = NULL;
            int split_pos = -1;
            while (line[split_pos++] != ' ');

            st = get_single_name(&key, line, split_pos, ';', &split_pos);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }

            const int pos = binary_search(*storage, key);
            if (pos == -1) 
            {
                free(line);
                free(key);
                return RUNTIME_ERROR;
            }

            print(*storage, pos);
            free(key);
        }
        else if (type == assigning) 
        {
            char* dist_key = NULL;
            char* src_key = NULL;
            int split_pos = 0;
            st = get_single_name(&dist_key, line, 0, '=', &split_pos);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }
            st = get_single_name(&src_key, line, split_pos, ';', &split_pos);
            if (st != SUCCESS) 
            {
                free(line);
                free(dist_key);
                return st;
            }
            
            const int dist_pos = binary_search(*storage, dist_key);

            if (is_num(src_key)) 
            {
                const int value = atoi(src_key);
                if (dist_pos == -1) 
                {
                    st = add_memory_cell(storage, dist_key, value);
                    if (st != SUCCESS) 
                    {
                        free(line);
                        free(dist_key);
                        free(src_key);
                        return st;
                    }
                }
                else edit_memory_cell(*storage, dist_pos, value);
            }
            else 
            {
                const int src_pos = binary_search(*storage, src_key);
                if (src_pos == -1) 
                {
                    free(line);
                    free(dist_key);
                    free(src_key);
                    return RUNTIME_ERROR;
                }
                edit_memory_cell(*storage, dist_pos, value_by_pos(*storage, src_pos));
            }

            free(dist_key);
            free(src_key);
        }
        else
        {
            char* dist_key = NULL;
            char* operand_1 = NULL;
            char* operand_2 = NULL;
            int split_pos = 0;
            st = get_single_name(&dist_key, line, 0, '=', &split_pos);
            if (st != SUCCESS) 
            {
                free(line);
                return st;
            }

            st = get_single_name(&operand_1, line, split_pos, get_operation_sign(type), &split_pos);
            if (st != SUCCESS) 
            {
                free(line);
                free(dist_key);
                return st;
            }
            const bool is_op_1_num = is_num(operand_1);
            int op_1_pos;
            if (!is_op_1_num) 
            {
                op_1_pos = binary_search(*storage, operand_1);
                if (op_1_pos == -1) 
                {
                    free(line);
                    free(dist_key);
                    free(operand_1);
                    return RUNTIME_ERROR;
                }
            }
            st = get_single_name(&operand_2, line, split_pos, ';', &split_pos);
            if (st != SUCCESS) 
            {
                free(line);
                free(dist_key);
                free(operand_1);
                return st;
            }
            const bool is_op_2_num = is_num(operand_2);
            int op_2_pos;
            if (!is_op_2_num) 
            {
                op_2_pos = binary_search(*storage, operand_2);
                if (op_2_pos == -1) 
                {
                    free(line);
                    free(dist_key);
                    free(operand_1);
                    free(operand_2);
                    return RUNTIME_ERROR;
                }
            }
            const int dist_pos = binary_search(*storage, dist_key);
            const int op_1_value = ((is_op_1_num) ? atoi(operand_1) : value_by_pos(*storage, op_1_pos));
            const int op_2_value = ((is_op_2_num) ? atoi(operand_2) : value_by_pos(*storage, op_2_pos));
            const int value = (type == plus) ? (op_1_value + op_2_value) : 
                (type == minus) ? (op_1_value - op_2_value) : 
                (type == division) ? (op_1_value / op_2_value) : 
                (type == remainder) ? (op_1_value % op_2_value) : 
                (op_1_value * op_2_value);
            if (dist_pos == -1) 
            {
                st = add_memory_cell(storage, dist_key, value);
                if (st != SUCCESS) 
                {
                    free(line);
                    free(dist_key);
                    free(operand_1);
                    free(operand_2);
                    return RUNTIME_ERROR;
                }
            }
            else edit_memory_cell(*storage, dist_pos, value);
            free(dist_key);
            free(operand_1);
            free(operand_2);
        }
    }
    free(line);
    return SUCCESS;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        print_error(INPUT_ERROR);
        return INPUT_ERROR;
    }

    FILE* instructions = fopen(argv[1], "r");
    if (!instructions) 
    {
        print_error(FILE_ERROR);
        return FILE_ERROR;
    }

    Storage* storage = NULL;

    const status_code st = solve(instructions, &storage);

    print_error(st);

    free_storage(storage);
    fclose(instructions);

    return st;
}