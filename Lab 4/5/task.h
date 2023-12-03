#ifndef TASK_H
#define TASK_H

#include "stack.h"

status_code to_postfix(char** postfix, const char* infix);
status_code calculate_postfix(const char* postfix, int* result);

#endif