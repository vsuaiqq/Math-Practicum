#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef enum status_code 
{
    success,
    file_error,
    invalid_data,
    allocate_error,
} status_code;

typedef struct Employee 
{
    int id;
    char name[256];
    char surname[256];
    double salary;
} Employee;

bool validate_data(const int id, const char* name, const char* surname, const double salary) 
{
    const double EPS = 1e-10;

    if (id < 0) return false;
    
    if (fabs(salary) <= EPS) return false;

    for (int i = 0; i < strlen(name); ++i) 
    {
        if (!((int)name[i] >= 65 && (int)name[i] <= 90) && !((int)name[i] >= 97 && (int)name[i] <= 122)) 
        {
            return false;
        }
    }

    for (int i = 0; i < strlen(surname); ++i) 
    {
        if (!((int)surname[i] >= 65 && (int)surname[i] <= 90) && !((int)surname[i] >= 97 && (int)surname[i] <= 122)) 
        {
            return false;
        }
    }

    return true;
}

int my_cmp_ascending(const void* val_1, const void* val_2) 
{
    const double EPS = 1e-10;

    Employee* emp_1 = (Employee*)val_1;
    Employee* emp_2 = (Employee*)val_2;

    if (emp_1->salary - emp_2->salary > EPS) 
    {
        return 1;
    }
    else if (emp_1->salary - emp_1->salary < EPS) 
    {
        return -1;
    }

    int surname_cmp_res = strcmp(emp_1->surname, emp_2->surname);
    if (surname_cmp_res) 
    {
        return surname_cmp_res;
    }

    int name_cmp_res = strcmp(emp_1->name, emp_2->name);
    if (name_cmp_res) 
    {
        return name_cmp_res;
    }

    return emp_1->id - emp_2->id;
}

int my_cmp_descending(const void* val_1, const void* val_2) 
{
    Employee* emp_1 = (Employee*)val_1;
    Employee* emp_2 = (Employee*)val_2;

    if (emp_1->salary > emp_2->salary) 
    {
        return -1;
    }
    else if (emp_1->salary < emp_1->salary) 
    {
        return 1;
    }

    int surname_cmp_res = strcmp(emp_1->surname, emp_2->surname);
    if (surname_cmp_res) 
    {
        return -surname_cmp_res;
    }

    int name_cmp_res = strcmp(emp_1->name, emp_2->name);
    if (name_cmp_res) 
    {
        return -name_cmp_res;
    }

    return -(emp_1->id - emp_2->id);
}

status_code read_data_from_file(const char* file_path, Employee** employees, int* employees_size) 
{
    const double EPS = 1e-10;

    FILE* input_file = fopen(file_path, "r");
    if (!input_file) return file_error;

    int employees_capacity = 32;
    *employees_size = 0;
    *employees = (Employee*)malloc(sizeof(Employee) * employees_capacity);
    if (*employees == NULL) 
    {
        fclose(input_file);
        return allocate_error;
    }

    char* line = NULL;
    size_t line_size = 0;

    int getline_res;
    while ((getline_res = getline(&line, &line_size, input_file)) != 1) 
    {
        if (getline_res == -1) 
        {
            free(line);
            return success;
        }
        Employee struct_tmp;

        if (sscanf(line, "%d %s %s %lf", &(struct_tmp.id), struct_tmp.name, struct_tmp.surname, &(struct_tmp.salary)) != 4) 
        {
            fclose(input_file);
            free(line);
            free(*employees);
            return invalid_data; 
        }
        if (!validate_data(struct_tmp.id, struct_tmp.name, struct_tmp.surname, struct_tmp.salary)) 
        {
            fclose(input_file);
            free(line);
            free(*employees);
            return invalid_data;
        }

        (*employees)[*employees_size] = struct_tmp;
        ++(*employees_size);

        if (*employees_size >= employees_capacity - 1) 
        {
            employees_capacity *= 2;
            Employee* realloc_tmp = (Employee*)realloc(*employees, sizeof(Employee) * employees_capacity);
            if (realloc_tmp == NULL) 
            {
                fclose(input_file);
                free(line);
                free(*employees);
                return allocate_error;
            }
            *employees = realloc_tmp;
        }
    }
    
    fclose(input_file);
    free(line);

    return success;
}

status_code write_data_to_file(const char* file_path, const Employee employees[], const int employees_size) 
{
    FILE* output_file = fopen(file_path, "w");
    if (!output_file) return file_error;
    
    for (int i = 0; i < employees_size; ++i) 
    {
        fprintf(output_file, "%d %s %s %f\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary);
    }

    return success;
}

int main(int argc, char* argv[])
{
    if (argc < 4) 
    {
        printf("invalid number of arguments!\n");
        return 1;
    }

    Employee* employees;
    int employees_size;

    const char* input_file_path = argv[1];
    const char* flag = argv[2];
    const char* output_file_path = argv[3];

    bool ascending;

    if (!strcmp(flag, "-a") || !strcmp(flag, "/a")) 
    {
        ascending = true;
    }
    else if (!strcmp(flag, "-d") || !strcmp(flag, "/d")) 
    {
        ascending = false;
    }
    else 
    {
        printf("invalid flag!\n");
        return 1;
    }

    switch (read_data_from_file(input_file_path, &employees, &employees_size))
    {
        case success:
            break;
        case file_error:
            printf("file error!\n");
            return 1;
        case invalid_data:
            printf("invalid data in file!\n");
            return 1;
        case allocate_error:
            printf("allocate error!\n");
            return 1;
    }

    if (ascending) 
    {
        qsort(employees, employees_size, sizeof(Employee), my_cmp_ascending);
    }
    else 
    {
        qsort(employees, employees_size, sizeof(Employee), my_cmp_descending);
    }

    write_data_to_file(argv[3], employees, employees_size);

    free(employees);
    return 0;
}