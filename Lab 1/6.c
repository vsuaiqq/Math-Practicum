#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

enum status_code 
{
    ok,
    invalid_parameter,
    overflow,
};

enum status_code func_a(double* val, double x) 
{
    if (x == 0 || x <= -1) return invalid_parameter;

    *val = log(1 + x) / x;
    
    return ok;
}

enum status_code func_b(double* val, double x) 
{
    *val = pow(exp(1), -x * x / 2);
    
    return ok;
}

enum status_code func_c(double* val, double x) 
{
    if (x >= 1) return invalid_parameter;
    
    *val = log(1 / (1 - x));
    
    return ok;
}

enum status_code func_d(double* val, double x) 
{
    *val = pow(x, x);
    
    return ok;
}

enum status_code solve(double* res, enum status_code func(double* res, double x), double left, double right, const double eps) 
{
    double cur, prev;
    int num_of_splits = 1;

    do
    {
        double len_of_split = (right - left) / num_of_splits;
        prev = cur, cur = 0;
        for (int i = 0; i < num_of_splits; ++i) 
        {
            double middle = left + (i + 0.5) * len_of_split, func_value;
            switch (func(&func_value, middle)) 
            {
                case ok:
                case overflow:
                    break;
                case invalid_parameter: return invalid_parameter;
            }
            cur += (func_value * len_of_split);
        }
        if (num_of_splits > (INT_MAX / 2)) return overflow;
        num_of_splits *= 2;
    } while (fabs(cur - prev) > eps);
    
    *res = prev;
    return ok;
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("invalid number of arguments detected!\n");
        return 1;
    }
    
    const double eps = atof(argv[1]);
    double res_a, res_b, res_c, res_d;
    
    switch (solve(&res_a, func_a, 0, 1, eps)) 
    {
        case ok:
            printf("res for a: %f\n", res_a);
            break;
        case invalid_parameter:
            printf("invalid parameter detected!\n");
            break;
        case overflow:
            printf("overflow detected!\n");
            break;
    }

    switch (solve(&res_b, func_b, 0, 1, eps)) 
    {
        case ok:
            printf("res for b: %f\n", res_b);
            break;
        case invalid_parameter:
            printf("invalid parameter detected!\n");
            break;
        case overflow:
            printf("overflow detected!\n");
            break;
    }
    
    switch (solve(&res_c, func_c, 0, 1, eps)) 
    {
        case ok:
            printf("res for c: %f\n", res_c);
            break;
        case invalid_parameter:
            printf("invalid parameter detected!\n");
            break;
        case overflow:
            printf("overflow detected!\n");
            break;
    }
    
    switch (solve(&res_d, func_d, 0, 1, eps)) 
    {
        case ok:
            printf("res for d: %f\n", res_d);
            break;
        case invalid_parameter:
            printf("invalid parameter detected!\n");
            break;
        case overflow:
            printf("overflow detected!\n");
            break;
    }
    
    return 0;
}