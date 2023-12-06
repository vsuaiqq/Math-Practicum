#include "hash_table.h"

void free_item(Item* item) 
{
    if (!item) return;
    if (item->def_name) free(item->def_name);
    if (item->value) free(item->value);
    item->def_name = NULL, item->value = NULL, item = NULL;
}

void free_list(List* list) 
{
    if (!list) return;
    Item* tmp = list->head;
    Item* tmp_prev;
    while (tmp) 
    {
        tmp_prev = tmp;
        tmp = tmp->next;
        free_item(tmp_prev);
    }
    list->head = NULL, list = NULL;
}

void free_table(hash_table* table) 
{
    if (!table) return;
    if (table->data) 
    {
        for (int i = 0; i < table->hash_size; ++i) 
        {
            free_list(table->data[i]);
        }
    }
    table->hash_size = 0, table = NULL;
}

status_code create_table(hash_table** table, const int hash_size) 
{
    (*table) = (hash_table*)malloc(sizeof(hash_table));
    if (!*table) return ALLOCATE_ERROR;
    (*table)->data = (List**)malloc(sizeof(Item) * hash_size);
    if (!(*table)->data) 
    {
        free_table(*table);
        return ALLOCATE_ERROR;
    }
    (*table)->hash_size = hash_size;
    return SUCCESS;
}

status_code create_item(Item** item, const char* def_name, const char* value, const bool has_hash_val, const unsigned long long int hash_value, unsigned long long int (*hash_function)(const char* def_name))
{
    (*item) = (Item*)malloc(sizeof(Item));
    if (!*item) return ALLOCATE_ERROR;
    (*item)->def_name = (char*)malloc(sizeof(char) * (strlen(def_name) + 1));
    if (!(*item)->def_name) 
    {
        free_item(*item);
        return ALLOCATE_ERROR;
    }
    strcpy((*item)->def_name, def_name);
    (*item)->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
    if (!(*item)->value) 
    {
        free_item(*item);
        return ALLOCATE_ERROR;
    }
    strcpy((*item)->value, value);
    if (has_hash_val) 
    {
        (*item)->hash_value = hash_value;
    }
    else 
    {
        (*item)->hash_value = hash_function(def_name);
    }
    return SUCCESS;
}

status_code insert_list(List** list, Item* item) 
{
    if (!*list) 
    {
        (*list) = (List*)malloc(sizeof(List));
        if (!*list) return ALLOCATE_ERROR;
        (*list)->head = item;
        (*list)->length = 1;
    }
    else 
    {
        Item* tmp = (*list)->head;
        while (tmp->next) 
        {
            tmp = tmp->next;
        }
        tmp->next = item;
        ++(*list)->length;
    }
    return SUCCESS;
}

status_code insert_table(hash_table* table, const char* def_name, const char* value, const bool has_hash_val, const unsigned long long int hash_value, unsigned long long int (*hash_function)(const char* def_name)) 
{
    Item* item = NULL;
    if (create_item(&item, def_name, value, has_hash_val, hash_value, hash_function) == ALLOCATE_ERROR) 
    {
        return ALLOCATE_ERROR;
    }
    const int index = item->hash_value % (table->hash_size);
    if (insert_list(&table->data[index], item) == ALLOCATE_ERROR) 
    {
        free_item(item);
        return ALLOCATE_ERROR;
    }
    if (!check_chains(table)) 
    {
        return rebuild_table(table);
    }
    return SUCCESS;
}

status_code rebuild_table(hash_table* table) 
{
    hash_table* rebuilded_table = (hash_table*)malloc(sizeof(hash_table));
    if (!rebuilded_table) return ALLOCATE_ERROR;
    rebuilded_table->hash_size = get_next_prime(table->hash_size);
    rebuilded_table->data = (List**)malloc(sizeof(List) * rebuilded_table->hash_size);
    if (!rebuilded_table->data) 
    {
        free_table(rebuilded_table);
        return ALLOCATE_ERROR;
    }
    for (int i = 0; i < table->hash_size; ++i) 
    {
        if (table->data[i]) 
        {
            Item* tmp = table->data[i]->head;
            while (tmp) 
            {
                if (insert_table(rebuilded_table, tmp->def_name, tmp->value, true, tmp->hash_value, NULL) == ALLOCATE_ERROR) 
                {
                    free_table(rebuilded_table);
                    return ALLOCATE_ERROR;
                }
                tmp = tmp->next;
            }
        }
    }
    free_table(table);
    table = rebuilded_table;
    return SUCCESS;
}

unsigned long long int hash_function(const char* def_name) 
{
    unsigned long long int hash_value = 0, power = 1;
    for (int i = strlen(def_name) - 1; i >= 0; --i) 
    {
        int tmp;
        if ('0' <= def_name[i] && def_name[i] <= '9') 
        {
            tmp = def_name[i] - '0';
        }
        if ('A' <= def_name[i] && def_name[i] <= 'Z') 
        {
            tmp = def_name[i] - 'A' + 10;
        }
        if ('a' <= def_name[i] && def_name[i] <= 'z') 
        {
            tmp = def_name[i] - 'a' + 36;
        }
        hash_value += tmp * power;
        power *= 62;
    }
    return hash_value;
}

char* find_in_table(hash_table* table, const char* def_name, unsigned long long int (*hash_function)(const char* def_name)) 
{
    const unsigned long long int hash_value = hash_function(def_name);
    const int index = hash_value % table->hash_size;
    if (!table->data[index]) return NULL;
    Item* tmp = table->data[index]->head;
    while (tmp) 
    {
        if (!strcmp(tmp->def_name, def_name)) return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

bool check_chains(hash_table* table) 
{
    int max_chain_len = 0, min_chain_len = 0;
    for (int i = 0; i < table->hash_size; ++i) 
    {
        if (table->data[i]) 
        {
            if (table->data[i]->length > max_chain_len) 
            {
                max_chain_len = table->data[i]->length;
            }
            if (table->data[i]->length < min_chain_len) 
            {
                min_chain_len = table->data[i]->length;
            }
        }
    }
    return (max_chain_len / min_chain_len < 2);
}