#include <stdio.h>
#include <math.h>

typedef enum status_code {
    success,
    invalid_function,
    invalid_interval,
} status_code;

double func_1(double root) 
{
    return sin(root) - 0.5;
}

double func_2(double root) 
{
    return log2(root) - 3;
}

double func_3(double root) 
{
    return root * root - 10;
}

status_code dichotomy_solve(double (*function)(double), double left, double right, double* res, const double eps) 
{
    if (function(left) * function(right) >= 0) return invalid_function;
    if (left > right) return invalid_interval;
    double middle_value, middle_point;
    while (fabs(left - right) > eps)
    {
        middle_point = (left + right) * 0.5;
        middle_value = function(middle_point);
        if (middle_value * function(left) < 0) 
        {
            right = middle_point;
        }
        else 
        {
            left = middle_point;
        }
    }
    *res = middle_point;
    return success;
}

void dichotomy_solve_handle(double (*function)(double), double left, double right, double* res, const double eps) 
{
    switch (dichotomy_solve(function, left, right, res, eps))
    {
        case success: 
            printf("%f\n", *res);
            break;
        case invalid_function:
            printf("the equation cannot be solved by this method\n");
            break;
        case invalid_interval:
            printf("the left edge of the segment must not exceed the right\n");
    }
}

int main() 
{
    const double eps = 1e-8;
    double res;

    dichotomy_solve_handle(func_1, 0, 1, &res, eps);
    dichotomy_solve_handle(func_2, 0.5, 10, &res, eps);
    dichotomy_solve_handle(func_3, 0, 10, &res, eps);

    return 0;
}