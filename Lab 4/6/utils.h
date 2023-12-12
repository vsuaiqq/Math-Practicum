#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MIN_FILENAME_SIZE 1
#define MAX_FILENAME_SIZE 9

typedef enum status_code 
{
    SUCCESS,
    ARGUMENTS_ERROR,
    ALLOCATE_ERROR,
    FILE_ERROR,
    UNEXPECTED_CHARACTER,
    INVALID_BRACKETS_BALANCE,
    EMPTY_STRING,
    UNUSED_CHARACTER,
} status_code;

bool is_bool_value(const char c); 
bool is_empty_str(const char* str);
long long int get_binary_permutations(const int num_of_vars);
int get_binary_value(const int line, const int pos);
int get_priority(const char c);
int res_by_char(const int left, const int right, const char c);
int get_rand(const int min, const int max);
char* get_rand_filename();
void print_info(FILE* stream, const char* vars, const int vars_size, const char* infix);
void print_line(FILE* stream, const int* values, const int values_size, const int result, const int margin);
void print_error(const status_code st);
status_code get_vars_from_expr(char** vars, int* vars_size, const char* expr);

#endif