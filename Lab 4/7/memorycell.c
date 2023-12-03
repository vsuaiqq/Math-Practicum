#include "memorycell.h"

int cmp_mc(const void* a, const void* b) 
{ 
    return strcmp((*(MemoryCell**)a)->key, (*(MemoryCell**)b)->key); 
}

void free_memory_cell(MemoryCell* memory_cell) 
{
    if (!memory_cell) return;
    if (memory_cell->key) 
    {
        free(memory_cell->key);
        memory_cell->key = NULL;
    }
    free(memory_cell);
    memory_cell = NULL;
}

status_code create_memory_cell(MemoryCell** memory_cell, const char* key, const int value) 
{
    (*memory_cell) = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (!*memory_cell) return ALLOCATE_ERROR;
    (*memory_cell)->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
    if (!(*memory_cell)->key) 
    {
        free_memory_cell(*memory_cell);
        return ALLOCATE_ERROR;
    }
    strcpy((*memory_cell)->key, key);
    (*memory_cell)->value = value;
    return SUCCESS;
}