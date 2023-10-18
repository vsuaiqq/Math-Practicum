#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum status_code 
{
    success,
    invalid_param,
} status_code;

typedef struct Point
{
    double x, y;
} Point;

double vector_product(Point a, Point b) 
{
    return a.x * b.y - a.y * b.x;
}

status_code calculate_polynomial(const double point, const int degree, double* res, ...) 
{
    if (degree < 0) return invalid_param;

    double coefs[degree + 1];

    va_list ptr;
    va_start(ptr, res);
    for (int i = 0; i < degree + 1; i++) 
    {
        coefs[i] = va_arg(ptr, double);
    }
    va_end(ptr);

    *res = 0;
    double var_degree = 1;
    for (int i = 0; i < degree + 1; i++) 
    {
        *res += coefs[degree - i] * var_degree;
        var_degree *= point;
    }

    return success;
}

status_code is_convex_polygon(const int num_of_vertex, bool* res, ...) 
{
    if (num_of_vertex < 3) return invalid_param;

    va_list ptr;
    va_start(ptr, res);
    
    int sign = 0;
    Point previous, current, next;

    previous = va_arg(ptr, Point);
    current = va_arg(ptr, Point);

    for (int i = 2; i < num_of_vertex; i++) 
    {
        next = va_arg(ptr, Point);

        Point edge1, edge2;
        edge1.x = current.x - previous.x;
        edge1.y = current.y - previous.y;
        edge2.x = next.x - current.x;
        edge2.y = next.y - current.y;
        
        double cross = vector_product(edge1, edge2);

        if (cross < 0) 
        {
            if (sign > 0) 
            {
                va_end(ptr);
                *res = false;
                return success;
            }
            sign = -1;
        } 
        else if (cross > 0) 
        {
            if (sign < 0) 
            {
                va_end(ptr);
                *res = false;
                return success;
            }
            sign = 1;
        }
        
        previous = current;
        current = next;
    }

    va_end(ptr);
    va_start(ptr, res);

    Point edge1, edge2;
    edge1.x = current.x - previous.x;
    edge1.y = current.y - previous.y;
    edge2.x = va_arg(ptr, Point).x - current.x;
    edge2.y = va_arg(ptr, Point).y - current.y;

    double cross = vector_product(edge1, edge2);

    if (cross < 0 && sign > 0) 
    {
        *res = false;
        va_end(ptr);
        return success;
    } 
    else if (cross > 0 && sign < 0)  
    {
        *res = false;
        va_end(ptr);
        return success;
    }

    *res = true;
    va_end(ptr);
    return success;
}

int main() 
{
    double polynomial_res;
    bool is_convex_res;

    Point p1 = {0, 0}, p2 = {1, 0}, p3 = {1, 1}, p4 = {0, 1};
    Point p5 = {0, 0}, p6 = {2, 0}, p7 = {2, 2}, p8 = {1, 1}, p9 = {0, 2};

    // 2x^2 - 0x + 5 в точке 36
    switch (calculate_polynomial(36, 2, &polynomial_res, (double)2, (double)0, (double)5))
    {
        case success:
            printf("res: %.2f\n", polynomial_res);
            break;
        case invalid_param:
            printf("degree of the polynomial must be non-negative\n");
    }
    
    // 2023x^3 + 2022x^2 + 2021x + 2020 в точке 12
    switch (calculate_polynomial(12, 3, &polynomial_res, (double)2023, (double)2022, (double)2021, (double)2020))
    {
        case success:
            printf("res: %.3f\n", polynomial_res);
            break;
        case invalid_param:
            printf("degree of the polynomial must be non-negative\n");
    }

    // 123456 в точке 0 + лишние коэффиценты
    switch (calculate_polynomial(0, 0, &polynomial_res, (double)123456, (double)1, (double)2, (double)3))
    {
        case success:
            printf("res: %.3f\n", polynomial_res);
            break;
        case invalid_param:
            printf("degree of the polynomial must be non-negative\n");
    }

    // отрицательная степень
    switch (calculate_polynomial(1, -1, &polynomial_res, (double)1, (double)2))
    {
        case success:
            printf("res: %.3f\n", polynomial_res);
            break;
        case invalid_param:
            printf("degree of the polynomial must be non-negative\n");
    }

    // квадрат
    switch (is_convex_polygon(4, &is_convex_res, p1, p2, p3, p4))
    {
        case success:
            if (is_convex_res) printf("polygon is convex\n");
            else printf("polygon isnt convex\n");
            break;
        case invalid_param:
            printf("polygon must have at least 3 vertices\n");
    }

    // две равные прямоугольные трапеции соединенные одной стороной
    switch (is_convex_polygon(5, &is_convex_res, p5, p6, p7, p8, p9))
    {
        case success:
            if (is_convex_res) printf("polygon is convex\n");
            else printf("polygon isnt convex\n");
            break;
        case invalid_param:
            printf("polygon must have at least 3 vertices\n");
    }

    return 0;
}