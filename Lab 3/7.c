#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define EPS 1e-9

typedef enum status_code 
{
    success,
    invalid_data,
    file_error,
    allocate_error,
    found,
    not_found,
} status_code;

typedef enum action_type 
{
    create,
    delete,
    change_surname,
    change_name,
    change_patronymic,
    change_birthday,
    change_gender,
    change_avg_income,
} action_type;

typedef struct Liver 
{
    char surname[BUFSIZ];
    char name[BUFSIZ];
    char patronymic[BUFSIZ];
    char birthday[BUFSIZ];
    char gender;
    double avg_income;
} Liver;

typedef struct Node 
{
    Liver* data;
    struct Node* next;
} Node;

typedef struct List 
{
    Node* head;
} List;

typedef struct Action 
{
    Node* node;
    action_type type;
    char old_initial[BUFSIZ];
    char old_gender;
    double old_avg_income;
    struct Action* prev_action;
} Action;

typedef struct Stack
{
    Action* last_action;
} Stack;

bool validate_initial(const char* initial) 
{
    for (int i = 0; i < strlen(initial); ++i) 
    {
        if (!((int)initial[i] >= 65 && (int)initial[i] <= 90) && !((int)initial[i] >= 97 && (int)initial[i] <= 122)) 
        {
            return false;
        }
    }
    return true;
}

bool validate_birthday(const char* birthday) 
{
    int day, month, year;
    if (sscanf(birthday, "%d,%d,%d", &day, &month, &year) != 3) return false;
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0) return false;
    return true;
}

bool validate_gender(const char gender) 
{
    return gender == 'M' || gender == 'W';
}

bool validate_avg_income(const double avg_income) 
{
    return avg_income > EPS;
}

bool validate_data(const char* surname, const char* name, const char* patronymic, const char* birthday, const char gender, const double avg_income) 
{
    return 
    (
        validate_initial(surname) 
        && validate_initial(name) 
        && validate_initial(patronymic) 
        && validate_birthday(birthday) 
        && validate_gender(gender)
        && validate_avg_income(avg_income)
    );
}

int my_birthday_cmp(const char* birthday_1, const char* birthday_2) 
{
    int day_1, month_1, year_1, day_2, month_2, year_2;
    sscanf(birthday_1, "%d,%d,%d", &day_1, &month_1, &year_1);
    sscanf(birthday_2, "%d,%d,%d", &day_2, &month_2, &year_2);
    const int year_diff = year_1 - year_2;
    if (year_diff) return year_diff;
    const int month_diff = month_1 - month_2;
    if (month_diff) return month_diff;
    return year_1 - year_2;
} 

void delete_list(List* list_to_remove) 
{
    if (list_to_remove->head) 
    {
        Node* tmp = list_to_remove->head;
        while (tmp) 
        {
            if (tmp->data) free(tmp->data);
            Node* tmp_cpy = tmp;
            tmp = tmp->next;
            free(tmp_cpy);
        }
    }
    free(list_to_remove);
}

void delete_stack(Stack* stack_to_remove) 
{
    if (stack_to_remove->last_action) 
    {
        Action* tmp = stack_to_remove->last_action;
        while (tmp) 
        {
            if (tmp->node) 
            {
                if (tmp->node->data) free(tmp->node->data);
                free(tmp->node);
            }
            Action* tmp_cpy = tmp;
            tmp = tmp->prev_action;
            free(tmp_cpy);
        }
    }
    free(stack_to_remove);
}

status_code create_liver(Liver** new_liver, const char* surname, const char* name, const char* patronymic, const char* birthday, const char gender, const double avg_income) 
{
    *new_liver = (Liver*)malloc(sizeof(Liver));
    if (!(*new_liver)) return allocate_error;
    strcpy((*new_liver)->surname, surname);
    strcpy((*new_liver)->name, name);
    strcpy((*new_liver)->patronymic, patronymic);
    strcpy((*new_liver)->birthday, birthday);
    (*new_liver)->gender = gender;
    (*new_liver)->avg_income = avg_income;
    return success;
}

status_code add_node(List* list, const char* surname, const char* name, const char* patronymic, const char* birthday, const char gender, const double avg_income, Node** new_node) 
{
    *new_node = (Node*)malloc(sizeof(Node));
    if (!(*new_node)) return allocate_error;
    
    Liver* new_liver;
    if (create_liver(&new_liver, surname, name, patronymic, birthday, gender, avg_income) == allocate_error) 
    {
        free(*new_node);
        return allocate_error;
    }
    (*new_node)->data = new_liver;
    
    if (list->head)
    {
        Node* tmp = list->head;
        Node* tmp_prev = list->head;
        while (tmp->next) 
        {
            if (my_birthday_cmp(new_liver->birthday, tmp->data->birthday) >= 0) 
            {
                if (tmp == list->head) 
                {
                    list->head = *new_node;
                    (*new_node)->next = tmp;
                }
                else 
                {
                    tmp_prev->next = *new_node;
                    (*new_node)->next = tmp;
                }
                return success;
            }
            tmp_prev = tmp;
            tmp = tmp->next;
        }
        if (my_birthday_cmp(new_liver->birthday, tmp->data->birthday) > 0) 
        {
            if (tmp == list->head) 
            {
                list->head = *new_node;
                (*new_node)->next = tmp;
            }
            else 
            {
                tmp_prev->next = *new_node;
                (*new_node)->next = tmp;
            }
        }
        else 
        {
            tmp->next = *new_node;
            (*new_node)->next = NULL;
        } 
        return success;
    }
    else 
    {
        list->head = *new_node;
        (*new_node)->next = NULL;
        return success;
    }
}

void remove_node(List* list, Node* node_to_remove) 
{
    Node* tmp = list->head;
    if (list->head == node_to_remove) 
    {
        list->head = list->head->next;
        return;
    }
    while (tmp->next) 
    {
        if (tmp->next == node_to_remove) 
        {
            tmp->next = node_to_remove->next;
            return;
        }
        tmp = tmp->next;
    }
}

status_code push(Stack* actions, Node* node, const action_type type, const char* old_initial, const char old_gender, const double old_avg_income) 
{
    Action* new_action = (Action*)malloc(sizeof(Action));
    if (!new_action) 
    {
        return allocate_error;
    }
    new_action->node = node;
    new_action->type = type;
    if (old_initial) strcpy(new_action->old_initial, old_initial);
    new_action->old_gender = old_gender;
    new_action->old_avg_income = old_avg_income;
    new_action->prev_action = actions->last_action;
    actions->last_action = new_action;
    return success;
}

status_code create_list(List** new_list) 
{
    *new_list = (List*)malloc(sizeof(List));
    if (!(*new_list)) return allocate_error;
    (*new_list)->head = NULL;
    return success;
}

status_code create_stack(Stack** new_stack) 
{
    *new_stack = (Stack*)malloc(sizeof(Stack));
    if (!(*new_stack)) return allocate_error;
    (*new_stack)->last_action = NULL;
    return success;
}

status_code read_data_from_file(List** list, const char* input_file_path) 
{
    FILE* input_file = fopen(input_file_path, "r");
    if (!input_file) return file_error;

    if (create_list(list) == allocate_error) { fclose(input_file); return allocate_error; }
    int cnt = 0;
    char* line = NULL;
    size_t line_size = 0;

    int getline_res;
    while ((getline_res = getline(&line, &line_size, input_file)) != -1) 
    {
        if (getline_res == -1) 
        {
            delete_list(*list);
            free(line);
            fclose(input_file);
            return allocate_error;
        }

        char surname[BUFSIZ];
        char name[BUFSIZ];
        char patronymic[BUFSIZ];
        char birthday[BUFSIZ];
        char gender;
        double avg_income;
        if (sscanf(line, "%s %s %s %s %c %lf", surname, name, patronymic, birthday, &gender, &avg_income) != 6) 
        {
            delete_list(*list);
            free(line);
            fclose(input_file);
            return invalid_data;
        }
        if (!validate_data(surname, name, patronymic, birthday, gender, avg_income)) 
        {
            delete_list(*list);
            free(line);
            fclose(input_file);
            return invalid_data;
        }

        Node* new_node;
        if (add_node(*list, surname, name, patronymic, birthday, gender, avg_income, &new_node) == allocate_error) 
        {
            delete_list(*list);
            free(line);
            fclose(input_file);
            return allocate_error;
        }

        free(line);
        line = NULL;
    }

    free(line);
    fclose(input_file);
    return success;
}

status_code find_liver(List* storage, const char* surname, const char* name, const char* patronymic, Node** res) 
{
    Node* tmp = storage->head;
    while (tmp) 
    {
        if (!strcmp(tmp->data->surname, surname) && !strcmp(tmp->data->name, name) && !strcmp(tmp->data->patronymic, patronymic)) 
        {
            *res = tmp;
            return found;
        }
        tmp = tmp->next;
    }
    return not_found;
}

void undo_change(Node* node, const action_type type, const char* old_initial, const char old_gender, const double old_avg_income) 
{
    switch (type)
    {
        case change_surname:
            strcpy(node->data->surname, old_initial);
            break;
        case change_name:
            strcpy(node->data->name, old_initial);
            break;
        case change_patronymic:
            strcpy(node->data->patronymic, old_initial);
            break;
        case change_birthday:
            strcpy(node->data->birthday, old_initial);
            break;
        case change_gender:
            node->data->gender = old_gender;
            break;
        case change_avg_income:
            node->data->avg_income = old_avg_income;
        default:
            break;
    }
}

void undo_delete(List* list, Node* node) 
{
    if (list->head)
    {
        Node* tmp = list->head;
        Node* tmp_prev = list->head;
        while (tmp->next) 
        {
            if (my_birthday_cmp(node->data->birthday, tmp->data->birthday) >= 0) 
            {
                if (tmp == list->head) 
                {
                    list->head = node;
                    node->next = tmp;
                }
                else 
                {
                    tmp_prev->next = node;
                    node->next = tmp;
                }
                return;
            }
            tmp_prev = tmp;
            tmp = tmp->next;
        }
        if (my_birthday_cmp(node->data->birthday, tmp->data->birthday) > 0) 
        {
            if (tmp == list->head) 
            {
                list->head = node;
                node->next = tmp;
            }
            else 
            {
                tmp_prev->next = node;
                node->next = tmp;
            }
        }
        else 
        {
            tmp->next = node;
            node->next = NULL;
        } 
        return;
    }
    else 
    {
        list->head = node;
        node->next = NULL;
        return;
    }
}

void undo_create(List* list, Node* node) 
{
    remove_node(list, node);
    free(node->data);
    free(node);
}

void print_liver_to_file(Liver* liver, FILE* output_file) 
{
    if (!output_file) output_file = stdout;
    fprintf(output_file, "--------------------------------\n");
    fprintf(output_file, "surname: %s\n", liver->surname);
    fprintf(output_file, "name: %s\n", liver->name);
    fprintf(output_file, "patronymic: %s\n", liver->patronymic);
    fprintf(output_file, "birthday: %s\n", liver->birthday);
    fprintf(output_file, "gender: %c\n", liver->gender);\
    fprintf(output_file, "average income: %.1f\n", liver->avg_income);
    fprintf(output_file, "--------------------------------\n");
}

status_code write_list_to_file(List* list, const char* output_file_path) 
{
    FILE* output_file;
    if (!output_file_path) output_file = NULL;
    else 
    {
        output_file = fopen(output_file_path, "w");
        if (!output_file) return file_error;
    }
    Node* tmp = list->head;
    while (tmp) 
    {
        print_liver_to_file(tmp->data, output_file);
        tmp = tmp->next;
    }
    fclose(output_file);
    return success;
}

void undo(Stack* actions, List* storage, const int modifications_count) 
{
    Action* tmp = actions->last_action;
    Action* tmp_next = actions->last_action;
    for (int i = 0; i < (int)ceil(modifications_count / 2.0); ++i)
    {
        switch (tmp->type)
        {
            case change_surname:
            case change_name:
            case change_patronymic:
            case change_gender:
            case change_avg_income:
                undo_change(tmp->node, tmp->type, tmp->old_initial, tmp->old_gender, tmp->old_avg_income);
                break;
            case change_birthday:
                undo_change(tmp->node, tmp->type, tmp->old_initial, tmp->old_gender, tmp->old_avg_income);
                remove_node(storage, tmp->node);
                undo_delete(storage, tmp->node);
                break;
            case create:
                undo_create(storage, tmp->node);
                break;
            case delete:
                undo_delete(storage, tmp->node);
        }
        tmp_next = tmp;
        free(tmp_next);
        tmp = tmp->prev_action;
        actions->last_action = tmp;
    }
}

void print_main_menu() 
{
    printf("-------Available commands-------\n");
    printf("1 - Find liver\n");
    printf("2 - Edit one of liver\'s fields\n");
    printf("3 - Add liver\n");
    printf("4 - Delete liver\n");
    printf("5 - Write all livers into file\n");
    printf("6 - Undo the last N/2 changes\n");
    printf("0 - Finish the programm\n");
    printf("--------------------------------\n");
}

void print_edit_menu() 
{
    printf("-------Choose field to edit-------\n");
    printf("1 - Surname\n");
    printf("2 - Name\n");
    printf("3 - Patronymic\n");
    printf("4 - Birthday\n");
    printf("5 - Gender\n");
    printf("6 - Average income\n");
    printf("--------------------------------\n");
}

void initials_input(char* surname, char* name, char* patronymic) 
{
    printf("Enter surname of liver: ");
    scanf("%s", surname);
    printf("Enter name of liver: ");
    scanf("%s", name);
    printf("Enter patronymic of liver: ");
    scanf("%s", patronymic);
}

void add_input(char* surname, char* name, char* patronymic, char* birthday, char* gender, double* avg_income) 
{
    initials_input(surname, name, patronymic);
    printf("Enter birthday of liver: ");
    scanf("%s", birthday);
    while (getchar() != '\n');
    printf("Enter gender of liver: ");
    scanf("%c", gender);
    while (getchar() != '\n');
    printf("Enter average income of liver: ");
    if (scanf("%lf", avg_income) != 1) 
    {
        while (getchar() != '\n');
        *avg_income = -1;
    }
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        printf("Invalid number of arguments detected!\n");
        return 1;
    }

    const char* input_file_path = argv[1];

    List* storage;

    status_code status;

    status = read_data_from_file(&storage, input_file_path);
    if (status == allocate_error) 
    {
        printf("Allocate error detected!\n");
        return 1;
    }
    if (status == file_error) 
    {
        printf("File error detected!\n");
        return 1;
    }
    if (status == invalid_data)
    {
        printf("Invalid data detected!\n");
        return 1;
    }

    Stack* actions;
    if (create_stack(&actions) == allocate_error)
    {
        delete_list(storage);
        printf("Allocate error detected!\n");
        return 1;
    }

    int modifications_count = 0, cmd;

    while (true) 
    {
        if (cmd == 7) while (getchar() != '\n');

        print_main_menu();
        printf("Enter command: ");
        if (scanf("%d", &cmd) != 1) cmd = 7;

        if (cmd == 1) 
        {
            char surname[BUFSIZ];
            char name[BUFSIZ];
            char patronymic[BUFSIZ];
            initials_input(surname, name, patronymic);
            if (validate_initial(surname) && validate_initial(name) && validate_initial(patronymic)) 
            {
                Node* res;
                status = find_liver(storage, surname, name, patronymic, &res);
                if (status == found) 
                {
                    print_liver_to_file(res->data, NULL);
                }
                if (status == not_found) 
                {
                    printf("Liver doesn\'t found!\n");
                }
            } else printf("Invalid data!\n");
        }
        else if (cmd == 2) 
        {
            char surname[BUFSIZ];
            char name[BUFSIZ];
            char patronymic[BUFSIZ];
            initials_input(surname, name, patronymic);
            if (validate_initial(surname) && validate_initial(name) && validate_initial(patronymic)) 
            {
                Node* res;
                status = find_liver(storage, surname, name, patronymic, &res);
                if (status == found) 
                {
                    print_edit_menu();
                    printf("Enter number of field you want to edit: ");
                    if (scanf("%d", &cmd) != 1) cmd = 7;
                    
                    if (cmd == 1) 
                    {
                        char new_surname[BUFSIZ];
                        printf("Enter new surname: ");
                        scanf("%s", new_surname);
                        if (validate_initial(new_surname)) 
                        {
                            if (push(actions, res, change_surname, res->data->surname, 0, 0) == allocate_error) 
                            { 
                                printf("Allocate error detected!\n"); 
                                return 1; 
                            }
                            strcpy(res->data->surname, new_surname);
                            ++modifications_count;
                        }
                        else printf("Invalid data!\n");
                    }
                    else if (cmd == 2) 
                    {
                        char new_name[BUFSIZ];
                        printf("Enter new name: ");
                        scanf("%s", new_name);
                        if (validate_initial(new_name))
                        {
                            if (push(actions, res, change_name, res->data->name, 0, 0) == allocate_error) 
                            { 
                                printf("Allocate error detected!\n");
                                return 1; 
                            }
                            strcpy(res->data->name, new_name);
                            ++modifications_count;
                        }
                        else printf("Invalid data!\n");
                    }
                    else if (cmd == 3) 
                    {
                        char new_patronymic[BUFSIZ];
                        printf("Enter new patronymic: ");
                        scanf("%s", new_patronymic);
                        if (validate_initial(new_patronymic)) 
                        {
                            if (push(actions, res, change_patronymic, res->data->patronymic, 0, 0) == allocate_error) 
                            { 
                                printf("Allocate error detected!\n"); 
                                return 1; 
                            }
                            strcpy(res->data->patronymic, new_patronymic);
                            ++modifications_count;
                        }
                        else printf("Invalid data!\n");
                    }
                    else if (cmd == 4) 
                    {
                        char new_birthday[BUFSIZ];
                        printf("Enter new birthday: ");
                        scanf("%s", new_birthday);
                        if (validate_birthday(new_birthday)) 
                        {
                            if (push(actions, res, change_birthday, res->data->birthday, 0, 0) == allocate_error) 
                            { 
                                printf("Allocate error detected!\n"); 
                                return 1; 
                            }
                            strcpy(res->data->birthday, new_birthday);
                            remove_node(storage, res);
                            undo_delete(storage, res);
                            ++modifications_count;
                        }
                        else printf("Invalid data!\n");
                    }
                    else if (cmd == 5) 
                    {
                        char new_gender;
                        printf("Enter new gender: ");
                        while (getchar() != '\n');
                        scanf("%c", &new_gender);
                        if (validate_gender(new_gender)) 
                        {
                            if (push(actions, res, change_gender, NULL, res->data->gender, 0) == allocate_error) 
                            { 
                                printf("Allocate error detected!\n"); 
                                return 1; 
                            }
                            res->data->gender = new_gender;
                            ++modifications_count;
                        }
                        else printf("Invalid data!\n");
                    }
                    else if (cmd == 6) 
                    {
                        double new_avg_income;
                        printf("Enter new average income: ");
                        if (scanf("%lf", &new_avg_income) != 1) 
                        {
                            printf("Invalid data!\n");
                            while (getchar() != '\n');
                        }
                        else 
                        {
                            if (validate_avg_income(new_avg_income)) 
                            {
                                if (push(actions, res, change_avg_income, NULL, 0, res->data->avg_income) == allocate_error) 
                                { 
                                    printf("Allocate error detected!\n"); 
                                    return 1; 
                                }
                                res->data->avg_income = new_avg_income;
                                ++modifications_count;
                            }
                            else printf("Invalid data!\n");
                        }
                    }
                    else printf("Invalid command!\n");
                }
                if (status == not_found) 
                {
                    printf("Liver doesn\'t found\n");
                }
            } else printf("Invalid data!\n");
        }
        else if (cmd == 3) 
        {
            char surname[BUFSIZ];
            char name[BUFSIZ];
            char patronymic[BUFSIZ];
            char birthday[BUFSIZ];
            char gender;
            double avg_income;
            add_input(surname, name, patronymic, birthday, &gender, &avg_income);

            if (validate_data(surname, name, patronymic, birthday, gender, avg_income)) 
            {
                Node* new_node;
                if (add_node(storage, surname, name, patronymic, birthday, gender, avg_income, &new_node) == allocate_error) 
                {
                    delete_stack(actions);
                    delete_list(storage);
                    printf("Allocate error detected!\n");
                    return 1;
                }
                if (push(actions, new_node, create, NULL, 0, 0) == allocate_error) 
                { 
                    printf("Allocate error detected!\n"); 
                    return 1; 
                }
                ++modifications_count;
            } else printf("Invalid data!\n");
        }
        else if (cmd == 4) 
        {
            char surname[BUFSIZ];
            char name[BUFSIZ];
            char patronymic[BUFSIZ];
            initials_input(surname, name, patronymic);
            if (validate_initial(surname) && validate_initial(name) && validate_initial(patronymic)) 
            {
                Node* res;
                status = find_liver(storage, surname, name, patronymic, &res);
                if (status == found) 
                {
                    remove_node(storage, res);
                    if (push(actions, res, delete, NULL, 0, 0) == allocate_error) 
                    { 
                        printf("Allocate error detected!\n"); 
                        return 1; 
                    }
                    ++modifications_count;
                }
                if (status == not_found) 
                {
                    printf("Liver doesn\'t found!\n");
                }
            } else printf("Invalid data!\n");
            
        }
        else if (cmd == 5) 
        {
            char output_file_path[BUFSIZ];
            printf("Enter file path: ");
            scanf("%s", output_file_path);
            write_list_to_file(storage, output_file_path);
        }
        else if (cmd == 6) 
        {
            if (modifications_count)
            { 
                undo(actions, storage, modifications_count);
                modifications_count = 0;
            }
            else printf("No changes to undo!\n");
        }
        else if (cmd == 0) 
        {
            delete_stack(actions);
            delete_list(storage);
            return 0;
        }
        else printf("Invalid command!\n");
    }
    delete_stack(actions);
    delete_list(storage);
    return 0;
}