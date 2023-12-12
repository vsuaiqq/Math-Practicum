#ifndef TASK_H
#define TASK_H

#include "stack.h"
#include "tree.h"

status_code to_postfix(char** postfix, const char* infix);
status_code make_tree(Tree** root, const char* postfix);
int calculate_tree(Tree *root, char* vars, int* vars_values, const int num_of_vars);
void make_table(FILE* stream, char* vars, const int vars_size, const char* infix, Tree* root);
status_code solve(const char* input_path);

#endif