#include "storage.h"

void free_storage(Storage* storage) 
{
    if (!storage) return;
    if (storage->data) 
    {
        for (int i = 0; i < storage->size; ++i) 
        {
            free_memory_cell(storage->data[i]);
        }
        free(storage->data);
    }
    free(storage);
    storage = NULL;
}

status_code init_storage(Storage** storage) 
{
    *storage = (Storage*)malloc(sizeof(Storage));
    if (!*storage) return ALLOCATE_ERROR;
    (*storage)->capacity = 32, (*storage)->size = 0;
    (*storage)->data = (MemoryCell**)malloc(sizeof(MemoryCell*) * (*storage)->capacity);
    if (!(*storage)->data) 
    {
        free_storage(*storage);
        return ALLOCATE_ERROR;
    }
    return SUCCESS;
}

int binary_search(Storage* storage, const char* key) 
{
    int left = 0;
    int right = storage->size - 1;
    while (left <= right)
    {
        const int mid = (left + right) / 2;
        const int cmp_res = strcmp(storage->data[mid]->key, key);
        if (cmp_res < 0) 
        {
            left = mid + 1;
        }
        else if (cmp_res > 0) 
        {
            right = mid - 1;
        }
        else return mid;
    }
    return -1;
}

int value_by_pos(Storage* storage, const int pos) 
{
    return storage->data[pos]->value;
}

status_code add_memory_cell(Storage** storage, const char* key, const int value) 
{

    MemoryCell* new_memory_cell;
    if (create_memory_cell(&new_memory_cell, key, value) == ALLOCATE_ERROR) return ALLOCATE_ERROR;
    (*storage)->data[(*storage)->size] = new_memory_cell;
    if ((*storage)->size >= (*storage)->capacity) 
    {
        (*storage)->capacity *= 2;
        MemoryCell** tmp = (MemoryCell**)realloc((*storage)->data, sizeof(MemoryCell*) * (*storage)->capacity);
        if (!tmp) return ALLOCATE_ERROR;
        (*storage)->data = tmp;
    }
    ++(*storage)->size;
    qsort((*storage)->data, (*storage)->size, sizeof(MemoryCell*), cmp_mc);
    return SUCCESS;
}

void edit_memory_cell(Storage* storage, const int pos, const int value) 
{
    storage->data[pos]->value = value;
}

void print(Storage* storage, const int pos) 
{
    if (pos != -1) 
    {
        printf("Key: %s, Value: %d\n", storage->data[pos]->key, storage->data[pos]->value);
    }
    else 
    {
        for (int i = 0; i < storage->size; ++i) 
        {
            printf("Key: %s, Value: %d\n", storage->data[i]->key, storage->data[i]->value);
        }
    }
}