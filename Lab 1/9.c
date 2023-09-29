#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FIXED_SIZE 10

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

void swap(int* a, int* b) 
{
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

void random_fill(int arr[], int size, int min, int max) 
{
    for (int i = 0; i < size; ++i) 
    {
        arr[i] = get_rand(min, max);
    }
}

void find_swap_min_max(int arr[], int size, int* min_res, int* max_res) 
{
    if (!size) 
    {
        return;
    }
    int min = arr[0], min_idx = 0, max = arr[0], max_idx = 0;
    for (int i = 1; i < size; ++i) 
    {
        if (arr[i] < min) min = arr[i], min_idx = i;
        if (arr[i] > max) max = arr[i], max_idx = i;
    }
    *min_res = min, *max_res = max;
    swap(&arr[min_idx], &arr[max_idx]);
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
    if (argc != 3) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    int fixed_arr[FIXED_SIZE];
    int min_rand = atoi(argv[1]), max_rand = atoi(argv[2]);
    
    if (min_rand > max_rand) swap(&min_rand, &max_rand);

    random_fill(fixed_arr, FIXED_SIZE, min_rand, max_rand);

    int min_el, max_el;
    find_swap_min_max(fixed_arr, FIXED_SIZE, &min_el, &max_el);

    int dynamic_size = get_rand(10, 10);

    int* dynamic_arr_a = (int*)malloc(sizeof(int*) * dynamic_size);
    if (dynamic_arr_a == NULL) 
    {
        printf("allocate error\n");
        return 1;
    }

    int* dynamic_arr_b = (int*)malloc(sizeof(int*) * dynamic_size);
    if (dynamic_arr_b == NULL) 
    {
        printf("allocate error\n");
        return 1;
    }
    
    random_fill(dynamic_arr_a, dynamic_size, -100, 100);
    random_fill(dynamic_arr_b, dynamic_size, -100, 100);

    int* dynamic_arr_c = (int*)malloc(sizeof(int*) * dynamic_size);
    if (dynamic_arr_c == NULL) 
    {
        printf("allocate error\n");
        return 1;
    }

    fill_c_arr(dynamic_arr_c, dynamic_arr_a, dynamic_arr_b, dynamic_size);

    free(dynamic_arr_a);
    free(dynamic_arr_b);
    free(dynamic_arr_c);

    return 0;
}