#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

typedef enum status_code 
{
    SUCCESS,
    ARGUMENTS_ERROR,
    ALLOCATE_ERROR,
    FILE_ERROR,
    UNEXPECTED_CHARACTER,
    INVALID_BRACKETS_BALANCE,
    EMPTY_SPACE_INSIDE_BRACKETS,
    EMPTY_STRING,
    UNUSED_CHARACTER,
    ZERO_DIVISION,
    NEGATIVE_MOD,
    NEGATIVE_POWER,
} status_code;

void print_std_error(const status_code st);
void print_error(FILE* stream, const status_code st, const char* infix, const int line);
void print_expr(const char* file_path, const char* infix, const char* postfix, const int value);
bool is_operator(const char c);
bool is_bracket(const char c);
int get_priority(const char c);

#endif