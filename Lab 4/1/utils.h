#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define START_HASH_SIZE 128

typedef enum status_code 
{
    SUCCESS,
    ALLOCATE_ERROR,
    FILE_ERROR,
    ARGUMENTS_ERROR,
} status_code;

bool is_prime(const int num);
int get_next_prime(int num);
void print_err(const status_code st);

#endif