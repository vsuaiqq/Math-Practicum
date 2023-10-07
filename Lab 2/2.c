#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

enum geometric_mean_status_code 
{
    gm_invalid_param,
    gm_nan_error,
    gm_ok,
    gm_overflow,
};

enum binary_pow_status_code 
{
    bp_overflow,
    bp_ok,
    bp_undef,
};

enum geometric_mean_status_code geometric_mean(double* result, int count, ...) 
{
    if (count < 1) 
    {
        return gm_invalid_param;
    }

    va_list ptr;
    va_start(ptr, count);

    *result = 1;
    for (int i = 0; i < count; ++i) 
    {
        *result *= va_arg(ptr, double);
    }
    *result = pow(*result, 1.0 / count);

    va_end(ptr);

    if (isinf(*result)) return gm_overflow;
    if (isnan(*result)) return gm_nan_error;

    return gm_ok;
}

enum binary_pow_status_code binary_pow(double* result, double a, int n, double eps) 
{
    if (fabsl(a) < eps && fabsl(n) < eps) return bp_undef;
    if (n == 0) 
    {
        *result = 1;
        return bp_ok;
    }
    enum binary_pow_status_code recursive_status_code;
    if (n % 2 != 0) 
    {
        recursive_status_code = binary_pow(result, a, abs(n - 1), eps);
        if (isinf(*result)) return bp_overflow;
        switch (recursive_status_code)
        {
            case bp_ok:
                *result *= a;
                if (n < 0) 
                {
                    *result = (a * a / *result);
                }
                return bp_ok;
            case bp_overflow:
                *result = 0;
                return bp_overflow;
        }
    }
    else 
    {
        recursive_status_code = binary_pow(result, a, abs(n / 2), eps);
        if (isinf(*result)) return bp_overflow;
        switch (recursive_status_code)
        {
            case bp_ok:
                *result *= *result;
                if (n < 0) 
                {
                    *result = (1 / *result);
                }
                return bp_ok;
            case bp_overflow:
                *result = 0;
                return bp_overflow;
        }
    }
}

int main(int argc, char* argv[]) 
{
    double bin_pow_res, geom_mean_res;

    switch (geometric_mean(&geom_mean_res, 5, (double)1, (double)2, (double)3, (double)4, (double)5))
    {
        case gm_ok:
            printf("result : %f\n", geom_mean_res);
            break;
        case gm_nan_error:
            printf("even degree root of a negative number!\n");
            return 1;
        case gm_overflow:
            printf("overflow detected!\n");
            return 1;
        case gm_invalid_param:
            printf("invalid parameter!\n");
            return 1;
    }

    switch (binary_pow(&bin_pow_res, 1, -10, 1e-5))
    {
        case bp_ok:
            printf("result : %f\n", bin_pow_res);
            break;
        case bp_overflow:
            printf("overflow detected!\n");
            return 1;
        case bp_undef:
            printf("uncertainty of the form 0 in degree 0\n");
            return 1;
    }

    return 0;
}