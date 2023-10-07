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

int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        printf("invalid number of arguments\n");
        return 1;
    }

    int fixed_arr[FIXED_SIZE];
    int min_rand, max_rand;
    
    if (sscanf(argv[1], "%d", &min_rand) == 0 || sscanf(argv[2], "%d", &max_rand) == 0) 
    {
        printf("arguments must have int type\n");
        return 1;
    }

    if (min_rand > max_rand) swap(&min_rand, &max_rand);

    random_fill(fixed_arr, FIXED_SIZE, min_rand, max_rand);

    int min_el, max_el;
    find_swap_min_max(fixed_arr, FIXED_SIZE, &min_el, &max_el);

    return 0;
}