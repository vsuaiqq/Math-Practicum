#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "utils.h"

#define HASHSIZE 128

typedef struct Item 
{
    char* def_name;
    char* value;
    unsigned long long int hash_value;
    struct Item* next;
} Item;

typedef struct List 
{
    int length;
    Item* head;
} List;

typedef struct hash_table
{
    int hash_size;
    List** data;
} hash_table;

void free_item(Item* item);
void free_list(List* list);
void free_table(hash_table* table);
status_code create_table(hash_table** table, const int hash_size);
status_code create_item(Item** item, const char* def_name, const char* value, const bool has_hash_val, const unsigned long long int hash_value, unsigned long long int (*hash_function)(const char* def_name));
status_code insert_list(List** list, Item* item);
status_code insert_table(hash_table* table, const char* def_name, const char* value, const bool has_hash_val, const unsigned long long int hash_value, unsigned long long int (*hash_function)(const char* def_name));
status_code rebuild_table(hash_table* table);
unsigned long long int hash_function(const char* def_name);
char* find_in_table(hash_table* table, const char* def_name, unsigned long long int (*hash_function)(const char* def_name));
bool check_chains(hash_table* table);

#endif