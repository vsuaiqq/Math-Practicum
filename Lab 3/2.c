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

typedef struct Vector
{
    int size;
    double* coord;
} Vector;

status_code create_vector(Vector* v, const int dim, ...) 
{
    v->coord = (double*)malloc(sizeof(double) * dim);
    if (v->coord == NULL) return allocate_error;
    v->size = dim;
    va_list ptr;
    va_start(ptr, dim);
    for (int i = 0; i < dim; ++i) 
    {
        v->coord[i] = va_arg(ptr, double);
    }
    return success; 
}

void delete_vector(Vector v)
{
    free(v.coord);
}

void delete_vectors(const int num_of_vectors, ...) 
{
    va_list ptr;
    va_start(ptr, num_of_vectors);
    for (int i = 0; i < num_of_vectors; ++i) 
    {
        delete_vector(va_arg(ptr, Vector));
    }
}

void print_vector(Vector v) 
{
    if (!v.size) return;
    for (int i = 0; i < v.size; ++i) 
    {
        printf("%f ", v.coord[i]);
    }
    printf("\n");
}

double max_norm(Vector v, const int dim) 
{
    double res = 0;
    for (int i = 0; i < dim; ++i) 
    {
        res = fmax(res, fabs(v.coord[i]));
    }
    return res;
}

status_code p_norm(Vector v, const int dim, const int p, double* res) 
{
    if (p < 1) return invalid_parameter;
    *res = 0;
    for (int i = 0; i < dim; ++i) 
    {
        *res += pow(fabs(v.coord[i]), p);
    }
    *res = pow(*res, 1 / p);
    return success;
}

status_code matrix_norm(Vector v, const int dim, double* res) 
{
    double** matrix = (double**)malloc(sizeof(double*) * dim);
    if (matrix == NULL) return allocate_error;

    for (int i = 0; i < dim; ++i) 
    {
        matrix[i] = (double*)malloc(sizeof(double) * dim);
        if (matrix[i] == NULL) 
        {
            for (int k = 0; k < i; ++k) free(matrix[i]);
            free(matrix);
            return allocate_error;
        }
        for (int j = 0; j < dim; ++j) 
        {
            matrix[i][j] = (i == j) ? 1 : 0;
        }
    }

    double* mult_res = (double*)malloc(sizeof(double) * dim);
    if (mult_res == NULL) 
    {
        for (int i = 0; i < dim; ++i) free(matrix[i]);
        free(matrix);
        return allocate_error;
    }

    for (int i = 0; i < dim; ++i) 
    {
        mult_res[i] = 0;
        for (int j = 0; j < dim; ++j) 
        {
            mult_res[i] += matrix[i][j] * v.coord[i];
        }
    }

    *res = 0;
    for (int i = 0; i < dim; ++i) 
    {
        *res += mult_res[i] * v.coord[i];
    }
    *res = sqrt(*res);

    for (int i = 0; i < dim; ++i) free(matrix[i]);
    free(matrix);
    free(mult_res);
    return success;
}

status_code get_max_vectors_by_norm(Vector*** res, int* max_norm_res_size, int* p_norm_res_size, int* matrix_norm_res_size,
    double (*max_norm)(Vector, const int), status_code (*p_norm)(Vector, const int, const int, double*), status_code (*matrix_norm)(Vector, const int, double*),
    const int dim, const int p, const int num_of_vectors, ...) 
{
    *res = (Vector**)malloc(sizeof(Vector*) * 3);
    if (*res == NULL) return allocate_error;

    va_list ptr;
    va_start(ptr, num_of_vectors);
    
    double max_norm_for_all[num_of_vectors];
    double p_norm_for_all[num_of_vectors];
    double matrix_norm_for_all[num_of_vectors];
    Vector vectors[num_of_vectors];
    double max_max_norm = -1, max_p_norm = -1, max_matrix_norm = -1;

    for (int i = 0; i < num_of_vectors; ++i) 
    {
        Vector v = va_arg(ptr, Vector);
        if (v.size != dim) 
        {
            free(*res);
            return invalid_parameter;
        }
        status_code status;

        const double cur_max_norm = max_norm(v, dim);
        max_max_norm = fmax(max_max_norm, cur_max_norm);

        double cur_p_norm;
        status = p_norm(v, dim, p, &cur_p_norm);
        if (status == invalid_parameter) return invalid_parameter;
        max_p_norm = fmax(max_p_norm, cur_p_norm);

        double cur_matrix_norm;
        status = matrix_norm(v, dim, &cur_matrix_norm);
        if (status == allocate_error) return allocate_error;
        max_matrix_norm = fmax(max_matrix_norm, cur_matrix_norm);

        max_norm_for_all[i] = cur_max_norm;
        p_norm_for_all[i] = cur_p_norm;
        matrix_norm_for_all[i] = cur_matrix_norm;
        vectors[i] = v;
    }

    int max_norm_res_capacity = 1, p_norm_res_capacity = 1, matrix_norm_res_capacity = 1;
    (*res)[0] = (Vector*)malloc(sizeof(Vector) * max_norm_res_capacity);
    (*res)[1] = (Vector*)malloc(sizeof(Vector) * p_norm_res_capacity);
    (*res)[2] = (Vector*)malloc(sizeof(Vector) * matrix_norm_res_capacity);
    *max_norm_res_size = 0, *p_norm_res_size = 0, *matrix_norm_res_size = 0;

    for (int i = 0; i < num_of_vectors; ++i) 
    {
        if (max_norm_for_all[i] == max_max_norm) 
        {
            if (*max_norm_res_size >= max_norm_res_capacity) 
            {
                max_norm_res_capacity *= 2;
                Vector* tmp = (Vector*)realloc((*res)[0], max_norm_res_capacity);
                if (tmp == NULL) 
                {
                    free(*res[1]), free(*res[2]), free(*res);
                    return allocate_error;
                }
                (*res)[0] = tmp;
            }
            (*res)[0][*max_norm_res_size] = vectors[i];
            ++(*max_norm_res_size);
        }
        if (p_norm_for_all[i] == max_p_norm) 
        {
            if (*p_norm_res_size >= p_norm_res_capacity) 
            {
                p_norm_res_capacity *= 2;
                Vector* tmp = (Vector*)realloc((*res)[1], p_norm_res_capacity);
                if (tmp == NULL) 
                {
                    free(*res[0]), free(*res[2]), free(*res);
                    return allocate_error;
                }
                (*res)[1] = tmp;
            }
            (*res)[1][*p_norm_res_size] = vectors[i];
            ++(*p_norm_res_size);
        }
        if (matrix_norm_for_all[i] == max_matrix_norm) 
        {
            if (*matrix_norm_res_size >= matrix_norm_res_capacity) 
            {
                matrix_norm_res_capacity *= 2;
                Vector* tmp = (Vector*)realloc((*res)[2], matrix_norm_res_capacity);
                if (tmp == NULL) 
                {
                    free(*res[0]), free(*res[1]), free(*res);
                    return allocate_error;
                }
                (*res)[2] = tmp;
            }
            (*res)[2][*matrix_norm_res_size] = vectors[i];
            ++(*matrix_norm_res_size);
        }
    }
    return success;
}

void print_results(Vector** res, const int dim, 
    const int max_norm_res_size, const int p_norm_res_size, const int matrix_norm_res_size) 
{
    printf("max norm res:\n\n");
    for (int i = 0; i < max_norm_res_size; ++i) 
    {
        print_vector(res[0][i]);
        printf("\n");
    }
    printf("p norm res:\n\n");
    for (int i = 0; i < p_norm_res_size; ++i) 
    {
        print_vector(res[1][i]);
        printf("\n");
    }
    printf("matrix norm res:\n\n");
    for (int i = 0; i < matrix_norm_res_size; ++i) 
    {
        print_vector(res[2][i]);
        printf("\n");
    }
}

int main() 
{
    Vector a, b, c, d;
    create_vector(&a, 3, 1.0, 2.0, 3.0);
    create_vector(&b, 3, 24.0, 0.0, 1.0);
    create_vector(&c, 3, 24.0, 3.0, 3.0);
    create_vector(&d, 3, 15.0, -17.0, 23.0);
    
    Vector** res;
    int max_norm_res_size, p_norm_res_size, matrix_norm_res_size;

    switch (get_max_vectors_by_norm(&res, &max_norm_res_size, &p_norm_res_size, &matrix_norm_res_size, max_norm, p_norm, matrix_norm, 3, 1, 4, a, b, c, d))
    {
    case success:
        break;
    case invalid_parameter:
        printf("invalid parameter detected!\n");

        return 1;
    case allocate_error:
        printf("allocate error detected!\n");
        return 1;
    }

    print_results(res, 3, max_norm_res_size, p_norm_res_size, matrix_norm_res_size);

    delete_vectors(4, a, b, c, d);
    free(res);
    return 0;
}