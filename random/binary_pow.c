#include <stdio.h>
#include <stdbool.h>

#define EPS 1e-11

double bin_pow(const double a, int n) 
{
    if (!n) return 1;
    const bool is_negative = n < 0;
    n = is_negative ? -n : n;
    if (!(n & 1)) 
    {
        const double tmp = bin_pow(a, n / 2);
        return is_negative ? 1 / (tmp * tmp) : (tmp * tmp);
    }
    return is_negative ? 1 / (a * bin_pow(a, n - 1)) : (a * bin_pow(a, n - 1));
}

int main() 
{
    printf("%lf\n", bin_pow(3, -1));
    printf("%lf\n", bin_pow(3, 4));
    printf("%lf\n", bin_pow(23, 0));

    return 0;
}