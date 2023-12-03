#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum status_code 
{
    SUCCESS,
    FILE_ERROR,
    ALLOCATE_ERROR,
    RUNTIME_ERROR,
    INPUT_ERROR,
} status_code;

typedef enum cmd_type 
{
    print_all,
    print_single,
    assigning,
    plus,
    minus,
    division,
    remainder,
    multiplying,
} cmd_type;

bool is_num(const char* str);
cmd_type get_cmd_type(char* cmd);
char get_operation_sign(const cmd_type type);
void print_error(const status_code error_code);
status_code get_single_name(char** str, const char* cmd, const int start_pos, const char sep, int* split_pos);

#endif