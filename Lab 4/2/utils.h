#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-11

typedef enum status_code 
{
    SUCCESS,
    FILE_ERROR,
    ALLOCATE_ERROR,
    INVALID_DATA,
    NOT_FOUND,
} status_code;

typedef enum sort_type 
{
    ASCENDING,
    DESCENDING,
    RANDOM,
    INVALID,
} sort_type;

int cmp_asc(const void* a, const void* b);
int cmp_dsc(const void* a, const void* b);
int cmp_rand(const void* a, const void* b);
int get_rand(int min, int max);
void print_error(const status_code error_code);
void str_to_lower(char** str);
bool validate_name(const char name);
bool validate_command(const char* command);
sort_type get_sort_type(const char* command);

#endif