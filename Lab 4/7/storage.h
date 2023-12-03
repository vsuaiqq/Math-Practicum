#ifndef STORAGE_H
#define STORAGE_H

#include "memorycell.h"

typedef struct Storage 
{
    int capacity;
    int size;
    MemoryCell** data;
} Storage;

void free_storage(Storage* storage);
status_code init_storage(Storage** storage);
int binary_search(Storage* storage, const char* key);
int value_by_pos(Storage* storage, const int pos);
status_code add_memory_cell(Storage** storage, const char* key, const int value);
void edit_memory_cell(Storage* storage, const int pos, const int value);
void print(Storage* storage, const int pos);

#endif