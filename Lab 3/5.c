#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define STR_SIZE 256

typedef enum status_code 
{
    success,
    file_error,
    invalid_data,
    allocate_error,
} status_code;

typedef enum find_status_code 
{
    found,
    not_found,
    fsc_allocate_error,
} find_status_code;

typedef enum find_type 
{
    id,
    name,
    surname,
    group,
} find_type;

typedef struct Student
{
    int id;
    char name[STR_SIZE];
    char surname[STR_SIZE];
    char group[STR_SIZE];
    unsigned char* marks;
} Student;

void free_storage(Student* storage, const int storage_size) 
{
    for (int i = 0; i < storage_size; ++i) free(storage[i].marks);
    free(storage);
}

double get_total_avg_mark(Student* students, const int students_size) 
{
    double res = 0;
    for (int i = 0; i < students_size; ++i) 
    {
        for (int j = 0; j < 5; ++j) 
        {
            res += students[i].marks[j] - '0';
        }
    }
    return res / (students_size * 5);
}

double get_avg_mark_for_student(Student tmp) 
{
    double res = 0;
    for (int i = 0; i < 5; ++i) 
    {
        res += tmp.marks[i] - '0';
    }
    return res / 5;
}

bool validate_data(Student tmp) 
{
    if (tmp.id < 0) return false;

    for (int i = 0; i < strlen(tmp.name); ++i) 
    {
        if (!((int)tmp.name[i] >= 65 && (int)tmp.name[i] <= 90) && !((int)tmp.name[i] >= 97 && (int)tmp.name[i] <= 122)) 
        {
            return false;
        }
    }
    for (int i = 0; i < strlen(tmp.surname); ++i) 
    {
        if (!((int)tmp.surname[i] >= 65 && (int)tmp.surname[i] <= 90) && !((int)tmp.surname[i] >= 97 && (int)tmp.surname[i] <= 122)) 
        {
            return false;
        }
    }

    for (int i = 0; i < 5; ++i) 
    {
        if (!isdigit(tmp.marks[i])) 
        {
            return false;
        }
    }

    return true;
}

status_code read_data_from_file(const char* file_path, Student** students, int* students_size) 
{
    const double EPS = 1e-10;

    FILE* input_file = fopen(file_path, "r");
    if (!input_file) return file_error;

    int students_capacity = 32;
    *students_size = 0;
    *students = (Student*)malloc(sizeof(Student) * students_capacity);
    if (*students == NULL) 
    {
        fclose(input_file);
        return allocate_error;
    }

    char* line = NULL;
    size_t line_size = 0;

    int getline_res;
    while ((getline_res = getline(&line, &line_size, input_file)) != -1) 
    {
        if (getline_res == -1) 
        {
            free_storage(*students, *students_size);
            fclose(input_file);
            free(line);
            return allocate_error;
        }
        Student struct_tmp;
        struct_tmp.marks = (unsigned char*)malloc(sizeof(unsigned char) * 5);
        if (struct_tmp.marks == NULL) 
        {
            free_storage(*students, *students_size);
            fclose(input_file);
            free(line);
            return allocate_error;
        }
        if (sscanf(line, "%d %s %s %s %c %c %c %c %c", &(struct_tmp.id), struct_tmp.name, struct_tmp.surname, struct_tmp.group, &(struct_tmp.marks[0]), &(struct_tmp.marks[1]), &(struct_tmp.marks[2]), &(struct_tmp.marks[3]), &(struct_tmp.marks[4])) != 9) 
        {
            free_storage(*students, *students_size);
            fclose(input_file);
            free(line);
            return invalid_data; 
        }
        if (!validate_data(struct_tmp)) 
        {
            free_storage(*students, *students_size);
            fclose(input_file);
            free(line);
            return invalid_data;
        }

        (*students)[*students_size] = struct_tmp;
        ++(*students_size);

        if (*students_size >= students_capacity - 1) 
        {
            students_capacity *= 2;
            Student* realloc_tmp = (Student*)realloc(*students, sizeof(Student) * students_capacity);
            if (realloc_tmp == NULL) 
            {
                free_storage(*students, *students_size);
                fclose(input_file);
                free(line);
                return allocate_error;
            }
            *students = realloc_tmp;
        }
    }
    
    fclose(input_file);
    free(line);

    return success;
}

find_status_code get_student_by_id(Student* students, const int students_size, const int id, Student* res, bool is_sorted) 
{
    if (is_sorted) 
    {
        int left = 0, right = students_size - 1;
        while (left <= right) 
        {
            int mid = (left + right) / 2;
            if (students[mid].id > id) right = mid - 1;
            else if (students[mid].id < id) left = mid + 1;
            else  
            {
                *res = students[mid];
                return found;
            }
        }
        return not_found;
    }
    else 
    {
        for (int i = 0; i < students_size; ++i) 
        {
            if (students[i].id == id) 
            {
                *res = students[i];
                return found;
            }
        }
        return not_found;
    }
}

find_status_code get_student_by_str(Student* students, const int students_size, const char* str, Student** res, int* res_size, find_type type) 
{
    int res_capacity = 32;
    *res = (Student*)malloc(sizeof(Student) * res_capacity);
    if ((*res) == NULL) return fsc_allocate_error;
    *res_size = 0;
    bool is_found = false;
    for (int i = 0; i < students_size; ++i) 
    {
        const char* struct_str = (type == name) ? students[i].name : students[i].surname;
        if (!strcmp(struct_str, str)) 
        {
            (*res)[*res_size] = students[i];
            ++(*res_size);
            if (*res_size >= res_capacity - 1) 
            {
                res_capacity *= 2;
                Student* realloc_tmp = (Student*)realloc(*res, sizeof(Student) * res_capacity);
                if (realloc_tmp == NULL) 
                {
                    free_storage(*res, *res_size);
                    return fsc_allocate_error;
                }
                *res = realloc_tmp;
            }
        is_found = true;
        }
    }
    return (is_found) ? found : not_found;
}

find_status_code get_students_by_avg_mark(Student* students, const int students_size, Student** res, int* res_size) 
{
    double total_avg_mark = get_total_avg_mark(students, students_size);
    int res_capacity = 32;
    *res = (Student*)malloc(sizeof(Student) * res_capacity);
    if ((*res) == NULL) return fsc_allocate_error;
    *res_size = 0;
    bool is_found = false;
    for (int i = 0; i < students_size; ++i) 
    {
        double cur_avg = get_avg_mark_for_student(students[i]);
        if (cur_avg > total_avg_mark) 
        {
            (*res)[*res_size] = students[i];
            ++(*res_size);
            if (*res_size >= res_capacity - 1) 
            {
                res_capacity *= 2;
                Student* realloc_tmp = (Student*)realloc(*res, sizeof(Student) * res_capacity);
                if (realloc_tmp == NULL) 
                {
                    free_storage(*res, *res_size);
                    return fsc_allocate_error;
                }
                *res = realloc_tmp;
            }
        is_found = true;
        }
    }
    return (is_found) ? found : not_found;
}

int id_cmp(const void * first, const void * second) 
{
    return ((Student *)first)->id - ((Student *)second)->id;
}

int surname_cmp(const void * first, const void * second) 
{
    return strcmp(((Student *)first)->surname, ((Student *)second)->surname);
}

int name_cmp(const void * first, const void * second) 
{
    return strcmp(((Student *)first)->name, ((Student *)second)->name);
}

int group_cmp(const void * first, const void * second) 
{
    return strcmp(((Student *)first)->group, ((Student *)second)->group);
}

void print_start_menu() 
{
    printf("enter 1 to find for all students by one of the parameters\n");
    printf("enter 2 to sort data by one of the parameters\n");
    printf("enter 3 to find names and surnames of students with average mark above the overall and write them to trace file\n");
    printf("enter 0 to finish the programm\n");
}

void print_find_menu() 
{
    printf("select the parameter you want to search by:\n");
    printf("1 - id\n");
    printf("2 - name\n");
    printf("3 - surname\n");
    printf("4 - group\n");
    printf("enter 0 to finish the programm\n");
}

void print_sort_menu() 
{
    printf("select the parameter you want to sort by:\n");
    printf("1 - id\n");
    printf("2 - name\n");
    printf("3 - surname\n");
    printf("4 - group\n");
    printf("enter 0 to finish the programm\n");
}

void print_trace_file_tip() 
{
    printf("you can also write result to trace file\n");
    printf("enter 1 to write\n");
    printf("enter 2 to skip\n");
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        printf("invalid number of arguments detected!\n");
        return 1;
    }

    const char* input_file_path = argv[1];
    FILE* trace_file;
    bool has_trace = false;
    if (argc > 2) 
    {
        has_trace = true;
        trace_file = fopen(argv[2], "w");
        if (trace_file == NULL) 
        {
            printf("file error detected!\n");
            return 1;
        }
    }

    Student* students;
    int students_size;
    switch (read_data_from_file(input_file_path, &students, &students_size))
    {
        case success:
            break;
        case file_error:
            printf("file error detected!\n");
            if (has_trace) fclose(trace_file);
            return 1;
        case allocate_error:
            printf("allocate error detected!\n");
            if (has_trace) fclose(trace_file);
            return 1;
        case invalid_data:
            printf("invalid data detected!\n");
            if (has_trace) fclose(trace_file);
            return 1;
    }

    int cmd;
    bool is_sorted = false;
    while (true) 
    {
        if (cmd == 4) while (getchar() != '\n');
        print_start_menu();

        int scanf_res = scanf("%d", &cmd);
        if (scanf_res != 1) cmd = 4;

        if (cmd == 1) 
        {
            if (cmd == 5) while(getchar() != '\n');
            print_find_menu();

            scanf_res = scanf("%d", &cmd);
            if (scanf_res != 1) cmd = 5;

            find_type type;
            if (cmd == 1) type = id;
            else if (cmd == 2) type = name;
            else if (cmd == 3) type = surname;
            else if (cmd == 4) type = group;
            else if (cmd == 0) 
            {
                if (has_trace) fclose(trace_file);
                free_storage(students, students_size);
                return 0;
            }
            else printf("invalid command!\n");

            if (type == id) 
            {
                printf("enter id to find: ");
                int id;
                scanf("%d", &id);

                Student res;
                find_status_code status = get_student_by_id(students, students_size, id, &res, is_sorted);
                if (status == found) 
                {
                    printf("----------------------------------------\n");
                    printf("id: %d\nname: %s\nsurname: %s\ngroup: %s\nmarks: %c %c %c %c %c\n", res.id, res.name, res.surname, res.group, res.marks[0], res.marks[1], res.marks[2], res.marks[3], res.marks[4]);
                    printf("----------------------------------------\n");
                    if (has_trace) 
                    {
                        print_trace_file_tip();

                        scanf_res = scanf("%d", &cmd);
                        if (scanf_res != 1) cmd = 3;

                        if (cmd == 1) 
                        {
                            double avg_mark = get_avg_mark_for_student(res);
                            fprintf(trace_file, "----------------------------------------\n");
                            fflush(trace_file);
                            fprintf(trace_file, "name: %s\nsurname: %s\ngroup: %s\naverage mark: %f\n", res.name, res.surname, res.group, avg_mark);
                            fflush(trace_file);
                            fprintf(trace_file, "----------------------------------------\n");
                            fflush(trace_file);
                        }
                        else if (cmd == 3) printf("invalid command!\n");
                    }
                }
                else printf("student doesnt found!\n");
            }
            else if (type == name || surname || group) 
            {
                char input_str[BUFSIZ];
                printf("enter value to find: ");
                scanf("%s", input_str);

                Student* res;
                int res_size;
                find_status_code status = get_student_by_str(students, students_size, input_str, &res, &res_size, type);
                if (status == found) 
                {
                    for (int i = 0; i < res_size; ++i) 
                    {
                        printf("----------------------------------------\n");
                        printf("id: %d\nname: %s\nsurname: %s\ngroup: %s\nmarks: %c %c %c %c %c\n", res[i].id, res[i].name, res[i].surname, res[i].group, res[i].marks[0], res[i].marks[1], res[i].marks[2], res[i].marks[3], res[i].marks[4]);
                        printf("----------------------------------------\n");
                    }
                    free(res);
                }
                else if (status == fsc_allocate_error) 
                {
                    printf("allocate error detected!\n");
                    if (has_trace) fclose(trace_file);
                    free_storage(students, students_size);
                    return 1;
                }
                else printf("students doesnt found!\n");
            }
        }
        else if (cmd == 2) 
        {
            if (cmd == 5) while(getchar() != '\n');
            print_sort_menu();

            scanf_res = scanf("%d", &cmd);
            if (scanf_res != 1) cmd = 5;

            if (cmd == 1) 
            {
                qsort(students, students_size, sizeof(students[0]), id_cmp);
                is_sorted = true;
            }
            else if (cmd == 2) qsort(students, students_size, sizeof(students[0]), name_cmp);
            else if (cmd == 3) qsort(students, students_size, sizeof(students[0]), surname_cmp);
            else if (cmd == 4) qsort(students, students_size, sizeof(students[0]), group_cmp);
            else printf("invalid command!\n");
        }
        else if (cmd == 3) 
        {
            if (!has_trace) printf("path to trace file is not specified!\n");
            else 
            {
                Student* res;
                int res_size;

                find_status_code status = get_students_by_avg_mark(students, students_size, &res, &res_size);
                if (status == found) 
                {
                    for (int i = 0; i < res_size; ++i) 
                    {
                        fprintf(trace_file, "----------------------------------------\n");
                        fflush(trace_file);
                        fprintf(trace_file, "name: %s\nsurname: %s\n", res[i].name, res[i].surname);
                        fflush(trace_file);
                        fprintf(trace_file, "----------------------------------------\n");
                        fflush(trace_file);
                    }
                    free(res);
                }
                else if (status == fsc_allocate_error) 
                {
                    printf("allocate error detected!\n");
                    if (has_trace) fclose(trace_file);
                    free_storage(students, students_size);
                    return 1;
                }
                else printf("students doesnt found!\n");
            }
        }
        else if (cmd == 0) 
        {
            if (has_trace) fclose(trace_file);
            free_storage(students, students_size);
            return 0;
        }
        else printf("invalid command!\n");
    }

    if (has_trace) fclose(trace_file);
    free_storage(students, students_size);
    return 0;
}