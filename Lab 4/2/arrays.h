#ifndef ARRAYS_H
#define ARRAYS_H

#include "utils.h"

typedef struct Array 
{
    char name;
    int size;
    int* data;
} Array;

typedef struct Arrays 
{
    int size;
    Array** data;
} Arrays;

void free_array(Array* array);
void free_arrays(Arrays* arrays);
status_code create_array(Array** array, const char name);
status_code create_arrays(Arrays** arrays);
status_code load(Arrays* arrays, const char name, FILE* input_file);
status_code save(Arrays* arrays, const char name, FILE* stream);
status_code rand_fill(Arrays* arrays, const char name, const int size, const int min, const int max);
status_code concat(Arrays* arrays, const char dest_name, const char src_name);
status_code copy(Arrays* arrays, const char dest_name, const char src_name, const int left, const int right);
status_code remove_array(Arrays* arrays, const char name);
status_code remove_elements(Arrays* arrays, const char name, const int start, const int count);
status_code sort(Arrays* arrays, const char name, const sort_type type);
status_code stats(Arrays* arrays, const char name);
status_code print_arr_slice(Arrays* arrays, const char name, const int left, const int right);
status_code print_arr(Arrays* arrays, const char name);

#endif