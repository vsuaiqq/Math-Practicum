#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define EPS 1E-9

typedef enum status_code 
{
    success,
    allocate_error,
    found,
    not_found,
    invalid_data,
} status_code;

typedef struct String
{
    char* str;
    int length;
} String;

typedef struct Adress 
{
    String* city;
    String* street;
    int house;
    String* building;
    int flat;
    String* index;
} Adress;

typedef struct Mail 
{
    Adress delivery_adress;
    double weight;
    String* id;
    String* create_time;
    String* delivery_time;
} Mail;

typedef struct Post 
{
    Adress* branch_adress;
    Mail* mails;
    int mails_size;
    int mails_capacity;
} Post;

bool validate_house(const int house) 
{
    return house >= 1;
}

bool validate_flat(const int flat) 
{
    return flat >= 1;
}

bool validate_weight(const double weight) 
{
    return weight > EPS;
}

bool validate_id(const String* id) 
{
    return id->length == 14;
}

bool validate_index(const String* index) 
{
    if (index->length != 6) return false;
    for (int i = 0; i < 6; ++i) 
    {
        if (!isdigit(index->str[i])) return false;
    }
    return true;
}

bool validate_date(const String* date) 
{
    if (date->length != 19) return false;
    for (int i = 0; i < date->length; ++i) 
    {
        if (i == 2 || i == 5 || i == 13 || i == 16) 
        {
            if (date->str[i] != ':') { printf("%d\n", i); return false; }
        }
        else if (i == 10) 
        {
            if (date->str[i] != ' ') { printf("%d\n", i); return false; }
        }
        else 
        {
            if (!isdigit(date->str[i])) { printf("%d\n", i); return false; }
        }
    }
    return true;
}

void print_string(String* tmp) 
{
    printf("%s\n", tmp->str);
}

void print_adress(Adress* tmp) 
{
    printf("city: ");
    print_string(tmp->city);
    printf("street: ");
    print_string(tmp->street);
    printf("house: %d\n", tmp->house);
    printf("building: ");
    print_string(tmp->building);
    printf("flat: %d\n", tmp->flat);
    printf("index: ");
    print_string(tmp->index);
}

void print_mail(Mail* tmp) 
{
    printf("-----------------------------\n");
    print_adress(&(tmp->delivery_adress));
    printf("weight: %f\n", tmp->weight);
    printf("id: ");
    print_string(tmp->id);
    printf("create time: ");
    print_string(tmp->create_time);
    printf("delivery time: ");
    print_string(tmp->delivery_time);
    printf("-----------------------------\n");
}

void delete_string(String* string_to_remove) 
{
    if (string_to_remove->str) 
    {
        free(string_to_remove->str);
        string_to_remove->str = NULL;
        
        string_to_remove->length = 0;
        
    }
    string_to_remove = NULL;
}

void delete_adress(Adress* adress_to_remove) 
{
    if (adress_to_remove->city) delete_string(adress_to_remove->city);
    if (adress_to_remove->street) delete_string(adress_to_remove->street);
    if (adress_to_remove->index) delete_string(adress_to_remove->index);
    adress_to_remove = NULL;
}

void delete_mail(Mail* mail_to_remove) 
{
    if (&(mail_to_remove->delivery_adress)) delete_adress(&(mail_to_remove->delivery_adress));
    if (mail_to_remove->id) delete_string(mail_to_remove->id);
    if (mail_to_remove->create_time) delete_string(mail_to_remove->create_time);
    if (mail_to_remove->delivery_time) delete_string(mail_to_remove->delivery_time);
    mail_to_remove = NULL;
}

void delete_post(Post* post_to_remove) 
{
    if (post_to_remove->branch_adress) delete_adress(post_to_remove->branch_adress);
    for (int i = 0; i < post_to_remove->mails_size; ++i) 
    {
        delete_mail(&(post_to_remove->mails[i]));
    }
    if (post_to_remove->mails) { free(post_to_remove->mails); post_to_remove->mails = NULL; }
    post_to_remove = NULL;
}

status_code create_string(String* new_string, const char* str) 
{
    new_string->length = strlen(str);

    new_string->str = (char*)malloc(sizeof(char) * (new_string->length + 1));
    if (!new_string->str) return allocate_error;
    
    strcpy(new_string->str, str);
    
    return success;
}

status_code create_adress(Adress* new_adress, String* city, String* street, const int house, String* building, const int flat, String* index) 
{
    new_adress->city = city;

    new_adress->street = street;

    if (!validate_house(house)) 
    {
        delete_adress(new_adress);
        return invalid_data;
    }
    new_adress->house = house;

    new_adress->building = building;

    if (!validate_flat(flat)) 
    {
        delete_adress(new_adress);
        return invalid_data;
    }
    new_adress->flat = flat;

    if (!validate_index(index)) 
    {
        delete_adress(new_adress);
        return invalid_data;
    }
    new_adress->index = index;

    return success;
}

status_code create_mail(Mail* new_mail, String* city, String* street, const int house, String* building, const int flat, String* index, const double weight, String* id, String* create_time, String* delivery_time) 
{
    Adress new_adress;
    status_code status = create_adress(&new_adress, city, street, house, building, flat, index);
    if (status == allocate_error) 
    {
        delete_adress(&new_adress);
        return allocate_error;
    }
    else if (status == invalid_data) 
    {
        delete_adress(&new_adress);
        return invalid_data;
    }
    new_mail->delivery_adress = new_adress;

    if (!validate_weight(weight)) 
    {
        delete_mail(new_mail);
        return invalid_data;
    }
    new_mail->weight = weight;

    if (!validate_id(id)) 
    {
        delete_mail(new_mail);
        return invalid_data;
    }
    new_mail->id = id;

    if (!validate_date(create_time)) 
    {
        delete_mail(new_mail);
        return invalid_data;
    }
    new_mail->create_time = create_time;
    
    if (!validate_date(delivery_time)) 
    {
        delete_mail(new_mail);
        return invalid_data;
    }
    new_mail->delivery_time = delivery_time;

    return success;
}

status_code create_post(Post* new_post, String* city, String* street, const int house, String* building, const int flat, String* index) 
{
    status_code status;

    Adress new_adress;
    status = create_adress(&new_adress, city, street, house, building, flat, index);
    if (status == allocate_error) 
    {
        return allocate_error;
    }
    if (status == invalid_data) 
    {
        return invalid_data;
    }
    new_post->branch_adress = &new_adress;

    new_post->mails_capacity = 32, new_post->mails_size = 0;
    new_post->mails = (Mail*)malloc(sizeof(Mail) * new_post->mails_capacity);
    if (!new_post->mails) 
    {
        delete_adress(&new_adress);
        return allocate_error;
    }
    
    return success;
}

void get_date_from_string(const String* tmp, int* day, int* month, int* year, int* hours, int* minutes, int* seconds) 
{
    sscanf(tmp->str, "%d:%d:%d %d:%d:%d", day, month, year, hours, minutes, seconds);
}

void get_cur_date(int* day, int* month, int* year, int* hours, int* minutes, int* seconds) 
{
    time_t t;
    struct tm *now;
    
    time(&t);
    now = localtime(&t);
    
    *year = now->tm_year + 1900;
    *month = now->tm_mon + 1;
    *day = now->tm_mday;
    *hours = now->tm_hour + 3;
    *minutes = now->tm_min;
    *seconds = now->tm_sec;
}

int my_string_cmp(const void * a, const void * b) 
{
    const String* string_1 = (String*)a;
    const String* string_2 = (String*)b;
    const int length_cmp_res = string_1->length - string_2->length;
    if (!length_cmp_res) return strcmp(string_1->str, string_2->str);
    return length_cmp_res;
}

int my_mails_cmp_main(const void * a, const void * b) 
{
    const Mail* mail_1 = (Mail*)a;
    const Mail* mail_2 = (Mail*)b;
    const int index_cmp_res = my_string_cmp(mail_1->delivery_adress.index, mail_2->delivery_adress.index);
    if (!index_cmp_res) return my_string_cmp(mail_1->id, mail_2->id);
    return index_cmp_res;
}

int my_mails_cmp_by_date(const void * a, const void * b) 
{
    const Mail* mail_1 = (Mail*)a;
    const Mail* mail_2 = (Mail*)b;
    int day_1, month_1, year_1, hours_1, minutes_1, seconds_1;
    int day_2, month_2, year_2, hours_2, minutes_2, seconds_2;
    get_date_from_string(mail_1->create_time, &day_1, &month_1, &year_1, &hours_1, &minutes_1, &seconds_1);
    get_date_from_string(mail_2->create_time, &day_2, &month_2, &year_2, &hours_2, &minutes_2, &seconds_2);
    const int year_diff = year_1 - year_2;
    if (year_diff) return year_diff;
    const int month_diff = month_1 - month_2;
    if (month_diff) return month_diff;
    const int day_diff = day_1 - day_2;
    if (day_diff) return day_diff;
    const int hours_diff = hours_1 - hours_2;
    if (hours_diff) return hours_diff;
    const int minutes_diff = minutes_1 - minutes_2;
    if (minutes_diff) return minutes_diff;
    return seconds_1 - seconds_2;
}

int compare_dates(int day_1, int month_1, int year_1, int hours_1, int minutes_1, int seconds_1, int day_2, int month_2, int year_2, int hours_2, int minutes_2, int seconds_2) 
{
    const int year_diff = year_1 - year_2;
    if (year_diff) return year_diff;
    const int month_diff = month_1 - month_2;
    if (month_diff) return month_diff;
    const int day_diff = day_1 - day_2;
    if (day_diff) return day_diff;
    const int hours_diff = hours_1 - hours_2;
    if (hours_diff) return hours_diff;
    const int minutes_diff = minutes_1 - minutes_2;
    if (minutes_diff) return minutes_diff;
    return seconds_1 - seconds_2;
}

bool are_equal(const String* string_1, const String* string_2) 
{
    return !strcmp(string_1->str, string_2->str);
}

status_code copy_to_existing(String* dest_string, const String* src_string) 
{
    dest_string->length = src_string->length;
    char* tmp = (char*)realloc(dest_string->str, sizeof(char) * (dest_string->length + 1));
    if (!tmp) 
    {
        free(dest_string->str);
        return allocate_error;
    }
    dest_string->str = tmp;
    strcpy(dest_string->str, src_string->str);
    return success;
}

status_code copy_to_new(String* dest_string, const String* src_string) 
{
    return create_string(dest_string, src_string->str);
}

status_code concat(String* dest_string, const String* src_string) 
{
    const int dest_string_old_len= dest_string->length;

    dest_string->length += src_string->length;
    char* tmp = (char*)realloc(dest_string->str, dest_string->length + 1);
    if (!tmp) 
    {
        free(dest_string->str);
        return allocate_error;
    }
    dest_string->str = tmp;

    for (int i = 0; i < src_string->length; ++i) 
    {
        dest_string->str[dest_string_old_len + i] = src_string->str[i];
    }
    dest_string->str[dest_string->length] = '\0';

    return success;
}

status_code add_mail(Post* storage, String* city, String* street, const int house, String* building, const int flat, String* index, const double weight, String* id, String* create_time, String* delivery_time) 
{
    Mail new_mail;
    status_code status = create_mail(&new_mail, city, street, house, building, flat, index, weight, id, create_time, delivery_time);
    if (status == allocate_error) 
    {
        return allocate_error;
    }
    if (status == invalid_data) 
    {
        return invalid_data;
    }

    storage->mails[storage->mails_size] = new_mail;
    storage->mails_size = storage->mails_size + 1;
    if (storage->mails_size >= storage->mails_capacity - 1) 
    {
        storage->mails_capacity = storage->mails_capacity * 2;
        Mail* tmp = (Mail*)realloc(storage->mails, sizeof(Mail) * storage->mails_capacity);
        if (tmp == NULL) 
        {
            return allocate_error;
        }
        storage->mails = tmp;
    }
    qsort(storage->mails, storage->mails_size, sizeof(storage->mails[0]), my_mails_cmp_main);
    return success;
}

status_code remove_mail_by_id(Post* storage, const String* id) 
{
    for (int i = 0; i < storage->mails_size; ++i) 
    {
        if (are_equal(storage->mails->id, id)) 
        {
            delete_mail(&(storage->mails[i]));
            for (int j = i; j < storage->mails_size - 1; ++j) 
            {
                storage->mails[i] = storage->mails[i + 1];
            }
            storage->mails_size = storage->mails_size - 1;
            Mail* tmp = (Mail*)realloc(storage->mails, sizeof(Mail) * storage->mails_size);
            if (!tmp) 
            {
                return allocate_error;
            } 
            storage->mails = tmp;
            return success;
        }
    }
    return not_found;
}

status_code find_mail_by_id(const Post* storage, const String* id, Mail* res)  
{
    for (int i = 0; i < storage->mails_size; ++i) 
    {
        if (are_equal(storage->mails[i].id, id))
        {
            *res = storage->mails[i];
            return found;
        }
    }
    return not_found;
}

status_code find_all_delivered_shipments(Post* storage, Mail** res, int* res_size) 
{
    int res_capacity = 32;
    *res_size = 0;
    *res = (Mail*)malloc(sizeof(Mail) * res_capacity);
    if (!*res) return allocate_error;
    for (int i = 0; i < storage->mails_size; ++i) 
    {
        int cur_day, cur_month, cur_year, cur_hours, cur_minutes, cur_seconds;
        int tmp_day, tmp_month, tmp_year, tmp_hours, tmp_minutes, tmp_seconds;
        get_date_from_string(storage->mails[i].delivery_time, &tmp_day, &tmp_month, &tmp_year, &tmp_hours, &tmp_minutes, &tmp_seconds);
        get_cur_date(&cur_day, &cur_month, &cur_year, &cur_hours, &cur_minutes, &cur_seconds);
        if (compare_dates(cur_day, cur_month, cur_year, cur_hours, cur_minutes, cur_seconds, tmp_day, tmp_month, tmp_year, tmp_hours, tmp_minutes, tmp_seconds) >= 0)
        {
            (*res)[*res_size] = storage->mails[i];
            ++(*res_size);
            if (*res_size >= res_capacity - 1) 
            {
                res_capacity *= 2;
                Mail* tmp = (Mail*)realloc(*res, res_capacity);
                if (!tmp) 
                {
                    return allocate_error;
                }
                *res = tmp;
            }
        }
    }
    return (*res_size > 0) ? success : not_found;
}

status_code find_all_undelivered_shipments(Post* storage, Mail** res, int* res_size) 
{
    int res_capacity = 32;
    *res_size = 0;
    *res = (Mail*)malloc(sizeof(Mail) * res_capacity);
    if (!*res) return allocate_error;
    for (int i = 0; i < storage->mails_size; ++i) 
    {
        int cur_day, cur_month, cur_year, cur_hours, cur_minutes, cur_seconds;
        int tmp_day, tmp_month, tmp_year, tmp_hours, tmp_minutes, tmp_seconds;
        get_date_from_string(storage->mails[i].delivery_time, &tmp_day, &tmp_month, &tmp_year, &tmp_hours, &tmp_minutes, &tmp_seconds);
        get_cur_date(&cur_day, &cur_month, &cur_year, &cur_hours, &cur_minutes, &cur_seconds);
        if (compare_dates(cur_day, cur_month, cur_year, cur_hours, cur_minutes, cur_seconds, tmp_day, tmp_month, tmp_year, tmp_hours, tmp_minutes, tmp_seconds) < 0)
        {
            (*res)[*res_size] = storage->mails[i];
            ++(*res_size);
            if (*res_size >= res_capacity - 1) 
            {
                res_capacity *= 2;
                Mail* tmp = (Mail*)realloc(*res, res_capacity);
                if (!tmp) 
                {
                    return allocate_error;
                }
                *res = tmp;
            }
        }
    }
    return (*res_size > 0) ? success : not_found;
}

void print_start_menu() 
{
    printf("first you need create post object\n");
    printf("enter the post office address details:\n");
}

void print_main_menu() 
{
    printf("enter 1 to add mail into post\n");
    printf("enter 2 to remove mail from post\n");
    printf("enter 3 to find mail by id\n");
    printf("enter 4 to find all delivered shipments\n");
    printf("enter 5 to find all undelivered shipments\n");
    printf("enter 0 to finish the programm\n");
}

status_code mail_data_input(String* city, String* street, int* house, String* building, int* flat, String* index) 
{
    char buff1[BUFSIZ];
    char buff2[BUFSIZ];
    char buff3[BUFSIZ];
    char buff4[BUFSIZ];

    printf("enter city (string): ");
    scanf("%[^\n]", buff1);
    while (getchar() != '\n');
    printf("enter street (string): ");
    scanf("%[^\n]", buff2);
    while (getchar() != '\n');
    printf("enter house (positive integer): ");
    if (scanf("%d", house) != 1) return invalid_data;
    while (getchar() != '\n');
    printf("enter building (string): ");
    scanf("%[^\n]", buff3);
    printf("enter flat (positive integer): ");
    while (getchar() != '\n');
    if (scanf("%d", flat) != 1) return invalid_data;
    while (getchar() != '\n');
    printf("enter index (6-digit string): ");
    scanf("%s", buff4);
    while (getchar() != '\n');

    if (create_string(city, buff1) == allocate_error || create_string(street, buff2) == allocate_error
    || create_string(building, buff3) == allocate_error || create_string(index, buff4) == allocate_error) 
    {
        return allocate_error;
    }

    return success;
}

status_code adress_data_input(String* city, String* street, int* house, String* building, int* flat, String* index, double* weight, String* id, String* create_time, String* delivery_time) 
{
    while (getchar() != '\n');
    printf("enter departure details:\n");
    status_code status = mail_data_input(city, street, house, building, flat, index);
    if (status == allocate_error) return allocate_error;
    if (status == invalid_data) return invalid_data;
    char buff5[BUFSIZ];
    char buff6[BUFSIZ];
    char buff7[BUFSIZ];

    printf("enter weight (non-negative real number): ");
    if (scanf("%lf", weight) != 1) return invalid_data;
    printf("enter id (14-symbol string): ");
    scanf("%s", buff5);
    while (getchar() != '\n');
    printf("enter create-time (dd:MM:yyyy hh:mm:ss): ");
    scanf("%[^\n]", buff6);
    while (getchar() != '\n');
    printf("enter delivery-time (dd:MM:yyyy hh:mm:ss): ");
    scanf("%[^\n]", buff7);
    while (getchar() != '\n');
    if (create_string(id, buff5) == allocate_error || create_string(create_time, buff6) == allocate_error
    || create_string(delivery_time, buff7) == allocate_error) 
    {
        return allocate_error;
    }

    return success;
}

status_code id_input(String* id) 
{
    printf("enter id: ");
    char buff[BUFSIZ];
    scanf("%s", buff);
    if (create_string(id, buff) == allocate_error) return allocate_error;
    return success;
}

int main() 
{
    status_code status;
    print_start_menu();

    String post_city;
    String post_street;
    int post_house;
    String post_building;
    int post_flat;
    String post_index;
    
    status = mail_data_input(&post_city, &post_street, &post_house, &post_building, &post_flat, &post_index);
    if (status == allocate_error) 
    {
        printf("allocate error detected!\n");
        return 1;
    }
    if (status == invalid_data) 
    {
        printf("invalid data detected!\n");
        return 1;
    }  
    
    Post storage;
    status = create_post(&storage, &post_city, &post_street, post_house, &post_building, post_flat, &post_index);
    if (status == allocate_error) 
    {
        printf("allocate error detected!\n");
        return 1;
    }
    if (status == invalid_data) 
    {
        printf("invalid data detected!\n");
        return 1;
    }

    int cmd;
    while (true)
    {
        if (cmd == 6) while (getchar() != '\n');
        print_main_menu();

        int scanf_res = scanf("%d", &cmd);
        if (scanf_res != 1) cmd = 6;

        if (cmd == 1) 
        {            
            String input_city;
            String input_street;
            int input_house;
            String input_building;
            int input_flat;
            String input_index;
            double input_weight;
            String input_id;
            String input_create_time;
            String input_delivery_time;

            status = adress_data_input(&input_city, &input_street, &input_house, &input_building, &input_flat, &input_index, &input_weight, &input_id, &input_create_time, &input_delivery_time);
            if (status == allocate_error) 
            {
                printf("allocate error detected!\n");
                return 1;
            }
            if (status == invalid_data) 
            {
                printf("invalid data detected!\n");
                return 1;
            }

            Mail res;
            if (find_mail_by_id(&storage, &input_id, &res) == found) 
            {
                printf("user with this id already exists\n");
            }
            else 
            {
                status = add_mail(&storage, &input_city, &input_street, input_house, &input_building, input_flat, &input_index, input_weight, &input_id, &input_create_time, &input_delivery_time);
                if (status == allocate_error) 
                {
                    printf("allocate error detected!\n");
                    return 1;
                }
                if (status == invalid_data) 
                {
                    printf("invalid data detected!\n");
                    return 1;
                }
            }
        }
        else if (cmd == 2) 
        {
            String input_id;
            if (id_input(&input_id) == allocate_error) 
            {
                printf("allocate error detected!\n");
                return 1;
            }

            status = remove_mail_by_id(&storage, &input_id);
            if (status == allocate_error) 
            {
                delete_string(&input_id);
                printf("allocate error detected!\n");
                return 1;
            }
            if (status == not_found) printf("mail doesnt exist\n");

            delete_string(&input_id);
        }
        else if (cmd == 3) 
        {
            String id;
            if (id_input(&id) == allocate_error) 
            {
                printf("allocate error detected!\n");
                return 1;
            }

            Mail res;
            if (find_mail_by_id(&storage, &id, &res) == found) 
            {
                print_mail(&res);
            }
            else printf("mail doesnt found\n");

            delete_string(&id);
        }
        else if (cmd == 4) 
        {
            Mail* res;
            int res_size;

            status = find_all_delivered_shipments(&storage, &res, &res_size);
            if (status == allocate_error) 
            {
                delete_mail(res);
                printf("allocate error detected!\n");
                return 1;
            }
            if (status == not_found) 
            {
                printf("delivered mails dont found\n");
            }
            if (status == success) 
            {
                qsort(res, res_size, sizeof(res[0]), my_mails_cmp_by_date);
                for (int i = 0; i < res_size; ++i) print_mail(&res[i]);
            }

            delete_mail(res);
        }
        else if (cmd == 5) 
        {
            Mail* res;
            int res_size;

            status = find_all_undelivered_shipments(&storage, &res, &res_size);
            if (status == allocate_error) 
            {
                delete_mail(res);
                printf("allocate error detected!\n");
                return 1;
            }
            if (status == not_found) 
            {
                printf("undelivered mails dont found\n");
            }
            if (status == success) 
            {
                qsort(res, res_size, sizeof(res[0]), my_mails_cmp_by_date);
                for (int i = 0; i < res_size; ++i) print_mail(&res[i]);
            }

            delete_mail(res);
        }
        else if (cmd == 0) 
        {
            return 0;
        }
        else printf("invalid command!\n");
    }
    
    return 0;
}