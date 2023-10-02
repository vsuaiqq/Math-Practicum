#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum status_code 
{
    ok,
    overflow
};

enum status_code factorial(int n, long* res) 
{
    if (n == 0 || n == 1) 
    {
        *res = 1;
        return ok;
    }
    *res = 1;
    long prev = 1;
    for (int i = 2; i <= n; ++i) 
    {
        *res *= i;
        if (*res < prev) 
        {
            return overflow;
        }
        prev = *res;
    }
    return ok;
}

enum status_code double_factorial(int n, long* res) 
{
    if (n == 0 || n == 1) 
    {
        *res = 1;
        return ok;
    }
    *res = 1;
    long prev = 1;
    for (int i = (2 - n % 2); i <= n; i += 2) 
    {
        *res *= i;
        if (*res < prev) 
        {
            return overflow;
        }
        prev = *res;
    }
    return ok;
}

enum status_code sum_a(double x, double eps, double* res)
{
    int n = 0;
    double prev = 1;
    *res = 1;
    do
    {
        ++n;
        prev = *res;
        long fact_res;
        double pow_res;
        switch (factorial(n, &fact_res))
        {
            case ok:
                break;
            case overflow:
                return overflow;
        }
        pow_res = pow(x, n);
        *res += pow_res / fact_res;
    } while (fabs(prev - *res) > eps);
    return ok;
}

enum status_code sum_b(double x, double eps, double* res)
{
    int n = 0;
    double prev = 1;
    *res = 1;
    do
    {
        ++n;
        prev = *res;
        long fact_res;
        double pow_res;
        switch (factorial(2 * n, &fact_res))
        {
            case ok:
                break;
            case overflow:
                return overflow;
        }
        pow_res = pow(x, 2 * n);
        pow_res = (n % 2 == 0) ? pow_res : -1 * pow_res;
        *res += pow_res / fact_res;
    } while (fabs(prev - *res) > eps);
    return ok;
}

enum status_code sum_c(double x, double eps, double* res)
{
    int n = 0;
    double prev = 1;
    *res = 1;
    do
    {
        ++n;
        prev = *res;
        long fact_res_fst, fact_res_snd;
        double pow_res_fst, pow_res_snd;
        switch (factorial(n, &fact_res_fst))
        {
            case ok:
                    break;
            case overflow:
                return overflow;
        }
        switch (factorial(3 * n, &fact_res_snd))
        {
            case ok:
                    break;
            case overflow:
                return overflow;
        }
        pow_res_fst = pow(3, 3 * n);
        pow_res_snd = pow(x, 2 * n);
        *res += (pow_res_fst * pow_res_snd * fact_res_fst * fact_res_fst * fact_res_fst) / fact_res_snd;
    } while (fabs(prev - *res) > eps);
    return ok;
}

enum status_code sum_d(double x, double eps, double* res)
{
    int n = 0;
    double prev = 1;
    *res = 1;
    do
    {
        ++n;
        prev = *res;
        double pow_res;
        pow_res = pow(x, 2 * n);
        if (isinf(pow_res)) 
        {
            return overflow;
        }
        pow_res = (n % 2 == 0) ? pow_res : -1 * pow_res;
        long fact_res;
        switch (double_factorial(2 * n, &fact_res))
        {
            case ok:
                break;
            case overflow:
                return overflow;
        }
        pow_res /= fact_res;
        switch (double_factorial(2 * n - 1, &fact_res))
        {
            case ok:
                break;
            case overflow:
                return overflow;
        }
        if (isinf(pow_res)) 
        {
            return overflow;
        }
        *res += pow_res;
    } while (fabs(prev - *res) > eps);
    return ok;
}

int main(int argc, char* argv[])
{
    double eps, x;
    double res;

    if (argc != 3) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    eps = atof(argv[1]), x = atof(argv[2]);

    if (eps <= 0) 
    {
        printf("accuracy must be positive\n");
        return 1;
    }

    switch (sum_a(x, eps, &res))
    {
        case ok:
            printf("sum a = %f\n", res);
            break;
        case overflow:
            printf("overflow detected\n");
            break;
    }

    switch (sum_b(x, eps, &res))
    {
        case ok:
            printf("sum b = %f\n", res);
            break;
        case overflow:
            printf("overflow detected\n");
            break;
    }

    switch (sum_c(x, eps, &res))
    {
        case ok:
            printf("sum c = %f\n", res);
            break;
        case overflow:
            printf("overflow detected\n");
            break;
    }

    switch (sum_d(x, eps, &res))
    {
        case ok:
            printf("sum d = %f\n", res);
            break;
        case overflow:
            printf("overflow detected\n");
            break;
    }

    return 0;
}