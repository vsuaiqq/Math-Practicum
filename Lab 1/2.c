#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_prime(int a) 
{
    if (a == 2) return true;
    if (a == 1 || a % 2 == 0) return false;
    for (int i = 3; i * i <= a; i += 2) 
    {
        if (a % i == 0) return false; 
    }
    return true;
}

double exp_by_lim(double eps) 
{
    int n = 1;
    double cur_value = 2, prev_value;
    do
    {
        ++n;
        prev_value = cur_value;
        cur_value = pow(1 + 1.0 / n, n);
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double exp_by_series(double eps)
{
    int n = 1;
    double cur_value = 2, prev_value;
    double fact = 1;
    do
    {
        ++n;
        prev_value = cur_value;
        fact *= n;
        cur_value += 1.0 / fact;
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double exp_by_equation(double eps) 
{
    double left = 1, right = 4, root, value;
    do
    {
        root = (left + right) / 2;
        value = log(root);
        if (value > 1) 
        {
            right = root;
        }
        else if (value < 1)
        {
            left = root;
        }
        else return root;
    } while (fabs(value - 1) > eps);
    return root;
}

double pi_by_lim(double eps) 
{
    int n = 1;
    double cur_value = 4, prev_value;
    do
    {
        prev_value = cur_value;
        cur_value *= 4.0 * (double)((n + 1) * n) / (double)((2 * n + 1)*(2 * n + 1));
        ++n;
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double pi_by_series(double eps) 
{
    int n = 1;
    double cur_value = 1, prev_value;
    do
    {
        ++n;
        prev_value = cur_value;
        cur_value += (n % 2 == 0) ? (-1.0 / (2 * n - 1)) : (1.0 / (2 * n - 1));
    } while (fabs(prev_value - cur_value) > eps);
    return 4 * cur_value;
}

double pi_by_equation(double eps)
{
    double left = 2, right = 4;
    double cur_root = (left + right / 2), prev_root;
    double value = -1 * sin(cur_root);
    do
    {
        prev_root = cur_root;
        if (value < 0) 
        {
            left = cur_root;
        }
        else if (value > 0) 
        {
            right = cur_root;
        }
        else return cur_root;
        cur_root = (left + right) / 2;
        value = -1 * sin(cur_root);
    } while (fabs(value) > eps && fabs(cur_root - prev_root) > eps);
    return cur_root;
}

double log_by_lim(double eps) 
{
    int n = 1;
    double cur_value = 1, prev_value = 1;
    do
    {
        ++n;
        prev_value = cur_value;
        cur_value = n * (pow(2, 1.0 / n) - 1);
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double log_by_series(double eps) 
{
    int n = 1;
    double cur_value = 1, prev_value = 1;
    do
    {
        ++n;
        prev_value = cur_value;
        cur_value += (n % 2 == 0) ? (-1.0 / n) : (1.0 / n);
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double log_by_equation(double eps) 
{
    double left = 0, right = 1, root, value;
    do
    {
        root = (left + right) / 2;
        value = exp(root);
        if (value < 2) 
        {
            left = root;
        }
        else if (value > 2) 
        {
            right = root;
        }
        else 
        {
            return root;
        }
    } while (fabs(value - 2) > eps);
    return root;
}

double root_by_lim(double eps) 
{
    double cur_value = -0.5, prev_value;
    do
    {
        prev_value = cur_value;
        cur_value = prev_value - 0.5 * (prev_value * prev_value) + 1;
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double root_by_series(double eps) 
{
    int k = 2;
    double cur_value = pow(2, 0.25), prev_value;
    do
    {
        ++k;
        prev_value = cur_value;
        cur_value *= pow(2, pow(2, -k));
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double root_by_equation(double eps) 
{
    double left = 1, right = 2, cur_root = 1.5, prev_root = 1.5;
    double value = cur_root * cur_root;
    do
    {
        prev_root = cur_root;
        if (value < 2) 
        {
            left = cur_root;
        }
        else if (value > 2) 
        {
            right = cur_root;
        }
        else 
        {
            return cur_root;
        }
        cur_root = (left + right) / 2;
        value = cur_root * cur_root;
    } while (fabs(value - 2) > eps && fabs(cur_root - prev_root) > eps);
    return cur_root;
}

double gamma_by_lim(double eps) 
{
    int n = 1;
    double cur_value = 1, prev_value;
    do
    {
        ++n;
        prev_value = cur_value;
        cur_value = 0;
        for (int i = 1; i <= n; ++i) 
        {
            cur_value += 1.0 / i;
        }
        cur_value -= log(n);
    } while (fabs(prev_value - cur_value) > eps);
    return cur_value;
}

double gamma_by_series(double eps) 
{
    int k = 2;
    const double pi = pi_by_equation(eps);
    double cur_value = 0.5, prev_value;
    do
    {
        ++k;
        prev_value = cur_value;
        cur_value += (1.0 / pow((int)sqrt(k), 2)) - (1.0 / k);
    } while (fabs(prev_value - cur_value) > eps || fabs(prev_value - cur_value) == 0);
    return cur_value - pi * pi / 6.0;
}

double gamma_by_equation(double eps) 
{
    int t = 2;
    double cur_root = -log(0.5 * log(2)), prev_root;
    double product_of_prime_nums = 0.5;
    do {
        ++t;
        prev_root = cur_root;
        if (is_prime(t)) 
        {
            product_of_prime_nums *= (t - 1.0) / t;
        }
        cur_root = -log(log(t) * product_of_prime_nums);
    } while(fabs(prev_root - cur_root) > eps);
    return cur_root;
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("invalid number of arguments detected!\n");
        return 1;
    }

    const double eps = atof(argv[1]);

    if (eps <= 0) 
    {
        printf("accuracy must be non-negative!\n");
        return 1;
    }

    printf("exp limit: %f\n", exp_by_lim(eps));
    printf("exp series: %f\n", exp_by_series(eps));
    printf("exp equation: %f\n", exp_by_equation(eps));
    
    printf("pi limit: %f\n", pi_by_lim(eps));
    printf("pi series: %f\n", pi_by_series(eps));
    printf("pi equation: %f\n", pi_by_equation(eps));
    
    printf("log limit: %f\n", log_by_lim(eps));
    printf("log series: %f\n", log_by_series(eps));
    printf("log equation: %f\n", log_by_equation(eps));

    printf("root limit: %f\n", root_by_lim(eps));
    printf("root series: %f\n", root_by_series(eps));
    printf("root equation: %f\n", root_by_equation(eps));
    
    printf("gamma limit: %f\n", gamma_by_lim(eps));
    printf("gamma series: %f\n", gamma_by_series(eps));
    printf("gamma equation: %f\n", gamma_by_equation(eps));
    


    return 0;
}