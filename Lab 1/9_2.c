#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int get_rand(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

void print_arr(int arr[], int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void random_fill(int arr[], int size, int min, int max) 
{
    for (int i = 0; i < size; ++i) 
    {
        arr[i] = get_rand(min, max);
    }
}

void fill_c_arr(int c[], int a[], int b[], int size) 
{
    for (int i = 0; i < size; ++i) 
    {
        int diff = abs(a[i] - b[0]), min_diff_el = b[0];
        for (int j = 1; j < size; ++j) 
        {
            if (abs(a[i] - b[j]) < diff) 
            {
                diff = abs(a[i] - b[j]);
                min_diff_el = b[j];
            }
        }
        c[i] = a[i] + min_diff_el;
    }
}

int main(int argc, char* argv[]) 
{
    int dynamic_size = get_rand(10, 10000);

    int* dynamic_arr_a = (int*)malloc(sizeof(int*) * dynamic_size);
    int* dynamic_arr_b = (int*)malloc(sizeof(int*) * dynamic_size);
    int* dynamic_arr_c = (int*)malloc(sizeof(int*) * dynamic_size);

    if (dynamic_arr_a == NULL || dynamic_arr_b == NULL || dynamic_arr_c == NULL) 
    {
        printf("allocate error\n");
        free(dynamic_arr_a);
        free(dynamic_arr_b);
        free(dynamic_arr_c);
        return 1;
    }
 
    random_fill(dynamic_arr_a, dynamic_size, -1000, 1000);
    random_fill(dynamic_arr_b, dynamic_size, -1000, 1000);

    fill_c_arr(dynamic_arr_c, dynamic_arr_a, dynamic_arr_b, dynamic_size);

    free(dynamic_arr_a);
    free(dynamic_arr_b);
    free(dynamic_arr_c);

    return 0;
}