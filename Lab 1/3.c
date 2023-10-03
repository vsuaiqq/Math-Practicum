#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

enum solve_equation_status_code 
{
    two_rootes,
    one_root,
    no_rootes,
    invalid_param
};

enum solve_equation_status_code solve_equation(double a, double b, double c, double* x_fst, double* x_snd) 
{
    if (a == 0) return invalid_param;
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant > 0) 
    {
        *x_fst = (-1 * b + sqrt(discriminant)) / (2 * a);
        *x_snd = (-1 * b - sqrt(discriminant)) / (2 * a);
        return two_rootes;
    }
    else if (discriminant == 0) 
    {
        *x_fst = (-1 * b) / (2 * a);
        *x_snd = *x_fst; 
        return one_root;
    }
    else 
    {
        return no_rootes;
    }
}

bool triangle_check(double side_fst, double side_snd, double side_trd, double eps) 
{
    double hypo, cath_fst, cath_snd;

    if (side_fst > side_snd && side_fst > side_trd) hypo = side_fst, cath_fst = side_snd, cath_snd = side_trd;
    else if (side_snd > side_fst && side_snd > side_trd) hypo = side_snd, cath_fst = side_fst, cath_snd = side_trd;
    else hypo = side_trd, cath_fst = side_fst, cath_snd = side_snd;

    if (fabs(((cath_fst * cath_fst) + (cath_snd * cath_snd)) - (hypo * hypo)) <= eps) return true;
    return false;
}

void solve_handler(double coef_fst, double coef_snd, double coef_trd, double* root_one, double* root_two)
{
    printf("%f is x^2 coef, %f is x coef, %f is free coef\n", coef_fst, coef_snd, coef_trd);
    switch (solve_equation(coef_fst, coef_snd, coef_trd, root_one, root_two))
    {
        case two_rootes:
            printf("first root: %f\n", *root_one);
            printf("second root: %f\n", *root_two);
            break;
        case one_root:
            printf("root: %f\n", *root_one);
            break;
        case no_rootes:
            printf("no rootes\n");
            break;
        case invalid_param:
            printf("equation is not square\n");
            break;
    }
}

int main(int argc, char* argv[]) 
{
    double eps, coef_fst, coef_snd, coef_trd;
    int num_one, num_two;
    double root_one, root_two;

    if (argc == 1) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    if (strcmp(argv[1], "-q") || strcmp(argv[1], "-m") || strcmp(argv[1], "-t")
    || strcmp(argv[1], "/q") || strcmp(argv[1], "/m") || strcmp(argv[1], "/t")) 
    {
        switch (argv[1][1])
        {
        case 'q':
            if (argc != 6) 
            {
                printf("invalid number of arguments\n");
                return 1;
            }

            eps = atof(argv[2]);
            coef_fst = atof(argv[3]);
            coef_snd = atof(argv[4]);
            coef_trd = atof(argv[5]);

            if (eps <= 0) 
            {
                printf("accuracy must be positive\n");
                return 1;
            }

            solve_handler(coef_fst, coef_snd, coef_trd, &root_one, &root_two);
            
            if (fabs(coef_fst - coef_snd) > eps && fabs(coef_fst - coef_trd) > eps
            && fabs(coef_snd - coef_trd) > eps) 
            {
                solve_handler(coef_fst, coef_trd, coef_snd, &root_one, &root_two);
                solve_handler(coef_snd, coef_fst, coef_trd, &root_one, &root_two);
                solve_handler(coef_snd, coef_trd, coef_fst, &root_one, &root_two);
                solve_handler(coef_trd, coef_fst, coef_snd, &root_one, &root_two);
                solve_handler(coef_trd, coef_snd, coef_fst, &root_one, &root_two);
            }
            else if (fabs(coef_fst - coef_snd) > eps && fabs(coef_fst - coef_trd) > eps) 
            {
                solve_handler(coef_snd, coef_fst, coef_trd, &root_one, &root_two);
                solve_handler(coef_snd, coef_trd, coef_fst, &root_one, &root_two);
            }
            else if (fabs(coef_fst - coef_snd) > eps && fabs(coef_snd - coef_trd) > eps)
            {
                solve_handler(coef_fst, coef_trd, coef_snd, &root_one, &root_two);
                solve_handler(coef_snd, coef_fst, coef_trd, &root_one, &root_two);
            }
            else if (fabs(coef_fst - coef_trd) > eps && fabs(coef_snd - coef_trd) > eps)
            {
                solve_handler(coef_fst, coef_trd, coef_snd, &root_one, &root_two);
                solve_handler(coef_trd, coef_fst, coef_snd, &root_one, &root_two);
            }
            else if (fabs(coef_fst - coef_snd) > eps)
            {
                solve_handler(coef_snd, coef_fst, coef_trd, &root_one, &root_two);
            }
            else if (fabs(coef_fst - coef_trd) > eps )
            {
                solve_handler(coef_snd, coef_trd, coef_fst, &root_one, &root_two);
            }
            else if (fabs(coef_snd - coef_trd) > eps)
            {
                solve_handler(coef_fst, coef_trd, coef_snd, &root_one, &root_two);
            }
            break;
        case 'm':
            if (argc != 4) 
            {
                printf("invalid number of arguments\n");
                return 1;
            }

            num_one = atoi(argv[2]);
            num_two = atoi(argv[3]);

            if (num_one < 1 || num_two < 1) 
            {
                printf("arguments must be non-negative!\n");
                return 1;
            }

            if (num_one % num_two == 0) 
            {
                printf("%d is multiple of %d\n", num_one, num_two);
            }
            else 
            {
                printf("%d is not multiple of %d\n", num_one, num_two);
            }
            break;
        case 't':
            if (argc != 6) 
            {
                printf("invalid number of arguments\n");
                return 1;
            }

            eps = atof(argv[2]);
            coef_fst = atof(argv[3]);
            coef_snd = atof(argv[4]);
            coef_trd = atof(argv[5]);
            
            if (eps <= 0) 
            {
                printf("accuracy must be positive\n");
                return 1;
            }
            if (coef_fst <= 0 || coef_snd <= 0 || coef_trd <= 0) 
            {
                printf("sides of triangle must be positive\n");
            }

            if (triangle_check(coef_fst, coef_snd, coef_trd, eps)) 
            {
                printf("yes\n");
            }
            else 
            {
                printf("no\n");
            }
            break;
        }
    }
    else 
    {
        printf("invalid flag\n");
        return 1;
    }
}