#ifndef MEMORYCELL_H
#define MEMORYCELL_H

#include "utils.h"

typedef struct MemoryCell 
{
    char* key;
    int value;
} MemoryCell;

int cmp_mc(const void* a, const void* b);
void free_memory_cell(MemoryCell* memory_cell);
status_code create_memory_cell(MemoryCell** memory_cell, const char* key, const int value);

#endif