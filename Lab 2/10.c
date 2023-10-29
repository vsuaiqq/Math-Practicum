#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

typedef enum status_code 
{
    success,
    invalid_parameter,
    allocate_error,
} status_code;

void print_coefs(const int coefs_size, double coefs[]) 
{
    if (!coefs_size) return;
    for (int i = 0; i < coefs_size; ++i) 
    {
        printf("%.5f ", coefs[i]);
    }
    printf("\n");
}

void derivative(const int degree, double coefs[]) 
{
    for (int i = 0; i < degree; ++i) 
    {
        coefs[i] = coefs[i + 1] * (i + 1);
    }
}

double calculate_polynomial(const double point, const int degree, double coefs[]) 
{
    double res = 0;
    double var_degree = 1;
    for (int i = 0; i < degree + 1; ++i) 
    {
        res += coefs[i] * var_degree;
        var_degree *= point;
    }
    return res;
}

status_code redecomposion(const double eps, const double a, const int degree, int* new_coefs_len, double** new_coefs, ...) 
{
    if (degree < 0) return invalid_parameter;

    *new_coefs = (double*)malloc(sizeof(double) * (degree + 1));
    if (*new_coefs == NULL) return allocate_error;

    double coefs[degree + 1];

    va_list ptr;
    va_start(ptr, new_coefs);
    for (int i = 0; i < degree + 1; ++i) 
    {
        coefs[i] = va_arg(ptr, double);
    }
    va_end(ptr);

    double fact = 1;
    for (int i = 0; i < degree + 1; ++i) 
    {
        (*new_coefs)[i] = calculate_polynomial(a, degree - i, coefs);
        if (i > 1) 
        {
            fact *= i;
            (*new_coefs)[i] /= fact;
        }
        derivative(degree - i, coefs);
    }

    *new_coefs_len = degree + 1;

    return success;
}

int main() 
{
    double* new_coefs;
    int new_coefs_len;

    switch (redecomposion(1e-8, -3, 1, &new_coefs_len, &new_coefs, (double)8, (double)5, (double)-2, (double)0, (double)3, (double)1))
    {
    case success:
        print_coefs(new_coefs_len, new_coefs);
        break;
    case invalid_parameter:
        printf("invalid parameter detected!\n");
        break;
    case allocate_error:
        printf("allocate error\n");
        return 1;
    }

    free(new_coefs);

    return 0;
}