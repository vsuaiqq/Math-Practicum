#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum status_code 
{
    success,
    invalid_parameter,
    allocate_error,
} status_code;

typedef struct Result
{
    double num;
    bool has_fin_repr;
} Result;

bool validate_base(const int base) 
{
    return base >= 2 && base <= 36;
}

bool validate_num(const double num, const double EPS)
{
    return (fabs(num) >= EPS && fabs(num) <= 1 - EPS);
} 

void print_results(const Result results[], const int results_size) 
{
    for (int i = 0; i < results_size; ++i) 
    {
        if (results[i].has_fin_repr) 
        {
            printf("%f true\n", results[i].num);
        }
        else 
        {
            printf("%f false\n", results[i].num);
        }
    }
}

int gcd(int a, int b) 
{
    while (b != 0) 
    {
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

int fractional_length(double num, const double EPS) 
{
    int length = 0;
    while (fabs(num - (int)num) > EPS) 
    {
        num *= 10;
        ++length;
    }
    return length;
}

void to_common_fraction(double cur_num, int* numerator, int* denominator, const double EPS) 
{
    *denominator = 1;
    int num_len = fractional_length(cur_num, EPS);
    for (int i = 0; i < num_len; ++i) 
    {
        cur_num *= 10;
        *denominator *= 10;
    }
    *numerator = (int)cur_num;
    int gcd_val = gcd(*numerator, *denominator);    
    *numerator /= gcd_val, *denominator /= gcd_val;
}

status_code fill_prime_divisors(int** prime_divisors, int* prime_divisors_size, const int num) 
{
    int capacity = 32;
    *prime_divisors = (int*)malloc(sizeof(int) * capacity);
    if (*prime_divisors == NULL) 
    {
        return allocate_error;
    }
    *prime_divisors_size = 0;

    if ((num & 1) == 0) 
    {
        (*prime_divisors)[0] = 2;
        ++(*prime_divisors_size);
    }
    for (int d = 3; d <= num; d += 2) 
    {
        if (num % d != 0) continue;

        bool is_prime = true;
        for (int j = 0; j < (*prime_divisors_size) + 1; ++j) 
        {
            if (d % (*prime_divisors)[j] == 0) 
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime) 
        {
            ++(*prime_divisors_size);
            if (*prime_divisors_size >= capacity) 
            {
                capacity *= 2;
                int* tmp = (int*)realloc(*prime_divisors, capacity);
                if (tmp == NULL) 
                {
                    free(*prime_divisors);
                    return allocate_error;
                }
                *prime_divisors = tmp;
            }
            (*prime_divisors)[*prime_divisors_size - 1] = d;
        }
    }

    return success;
} 

status_code has_finite_representation(const double EPS, Result** results, int* results_size, 
                                      const int base, const int nums_count, ...) 
{
    if (!validate_base(base)) return invalid_parameter;

    *results_size = nums_count;
    *results = (Result*)malloc(sizeof(Result) * (*results_size));
    if (*results == NULL) return allocate_error;

    va_list ptr;
    va_start(ptr, nums_count);
    for (int i = 0; i < nums_count; ++i) 
    {
        double cur_num = va_arg(ptr, double);
        if (!validate_num(cur_num, EPS))  
        {
            free(*results);
            return invalid_parameter;
        }
        (*results)[i].num = cur_num;

        int numerator, denominator;
        to_common_fraction(cur_num, &numerator, &denominator, EPS);

        int* denominator_prime_divisors;
        int denominator_prime_divisors_size;
        if (fill_prime_divisors(&denominator_prime_divisors, &denominator_prime_divisors_size, denominator) == allocate_error) 
        {
            free(*results);
            return allocate_error;
        }

        bool check = true;
        for (int j = 0; j < denominator_prime_divisors_size; ++j) 
        {
            if (base % denominator_prime_divisors[j] != 0) 
            {
                check = false;
                break;
            }
        }

        (*results)[i].has_fin_repr = check ? true : false;
        free(denominator_prime_divisors);
    }
    va_end(ptr);
    return success;
}

int main() 
{
    Result* results;
    int results_size;

    switch (has_finite_representation(1e-9, &results, &results_size, 2, 5, 0.2, 0.125, 0.4, 0.1, 0.05))
    {
        case success:
            print_results(results, results_size);
            break;
        case invalid_parameter:
            printf("invalid parameter detected!\n");
            break;
        case allocate_error:
            printf("allocate error detected!\n");
            return 1;
    }

    free(results);
    return 0;
}