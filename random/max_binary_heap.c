#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define START_CAPACITY 32

/*

Полное двоичное дерево(кроме ласт уровня), заполняется сверху вниз и слева направо
Удовлетворяет свойству кучи: приоритет вершины не меньше приоритета её потомков

*/

typedef struct BinaryHeap 
{
    int* array;
    int size;
    int capacity;
} BinaryHeap;

typedef enum status_code 
{
    SUCCESS,
    ALLOCATE_ERROR,
} status_code;

void swap(int* a, int* b) 
{
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

BinaryHeap* init_binary_heap() 
{
    BinaryHeap* new_bh = (BinaryHeap*)malloc(sizeof(BinaryHeap));
    if (!new_bh) return NULL;
    new_bh->array = (int*)malloc(sizeof(int) * START_CAPACITY);
    if (!new_bh->array) 
    {
        free(new_bh);
        return NULL;
    }
    new_bh->size = 0, new_bh->capacity = START_CAPACITY;
    return new_bh;
}

void free_binary_heap(BinaryHeap* bh_to_free) 
{
    if (!bh_to_free) return;
    if (bh_to_free->array) 
    {
        free(bh_to_free->array);
        bh_to_free->array = NULL;
    }
    free(bh_to_free);
    bh_to_free = NULL;
}

int get_max(BinaryHeap* bh) 
{
    return bh->array[0];
}

int get_min(BinaryHeap* bh) 
{
    int min = bh->array[bh->size / 2];
    for (int i = bh->size / 2 + 1; i < bh->size; ++i) 
    {
        if (bh->array[i] < min) min = bh->array[i];
    }
    return min;
}

void heapify_up(BinaryHeap* bh) 
{
    int child = bh->size - 1;
    int parent = (child - 1) / 2;
    while (bh->array[parent] < bh->array[child]) 
    {
        swap(&bh->array[parent], &bh->array[child]);
        child = parent;
        parent = (child - 1) / 2;
    }
}

void heapify_down(BinaryHeap* bh, const int pos) 
{
    int parent = pos;
    const int left_child = parent * 2 + 1, right_child = parent * 2 + 2;
    
    if (left_child < bh->size && bh->array[left_child] > bh->array[parent]) 
    {
        parent = left_child;
    }

    if (right_child < bh->size && bh->array[right_child] > bh->array[parent]) 
    {
        parent = right_child;
    }

    if (parent != pos) 
    {
        swap(&bh->array[parent], &bh->array[pos]);
        heapify_down(bh, parent);
    }
}

status_code enqueue(BinaryHeap* bh, const int val) 
{
    bh->array[bh->size++] = val;
    if (bh->size >= bh->capacity) 
    {
        bh->capacity *= 2;
        int* tmp = (int*)realloc(bh->array, sizeof(int) * bh->capacity);
        if (!tmp) 
        {
            free(bh->array);
            return ALLOCATE_ERROR;
        }
        bh->array = tmp;
    }

    heapify_up(bh);

    return SUCCESS;
}

int dequeue(BinaryHeap* bh) 
{
    const int removed_val = bh->array[0];
    bh->array[0] = bh->array[--bh->size];

    heapify_down(bh, 0);

    return removed_val;
}

status_code destruction_merge(BinaryHeap** bh_res, BinaryHeap* bh1, BinaryHeap* bh2) 
{
    *bh_res = init_binary_heap();
    if (!*bh_res) return ALLOCATE_ERROR;

    while (bh1->size)
    {
        if (enqueue(*bh_res, dequeue(bh1)) == ALLOCATE_ERROR) 
        {
            free_binary_heap(*bh_res);
            return ALLOCATE_ERROR;
        }
    } 
    free_binary_heap(bh1);

    while (bh2->size)
    {
        if (enqueue(*bh_res, dequeue(bh2)) == ALLOCATE_ERROR) 
        {
            free_binary_heap(*bh_res);
            return ALLOCATE_ERROR;
        }
    }
    free_binary_heap(bh2);

    return SUCCESS;
}

status_code save_merge(BinaryHeap** bh_res, BinaryHeap** bh1, BinaryHeap** bh2) 
{
    *bh_res = init_binary_heap();
    if (!*bh_res) return ALLOCATE_ERROR;

    BinaryHeap* bh1_copy = init_binary_heap();
    if (!bh1_copy) 
    {
        free_binary_heap(*bh_res);
        return ALLOCATE_ERROR;
    }
    BinaryHeap* bh2_copy = init_binary_heap();
    if (!bh2_copy) 
    {
        free_binary_heap(*bh_res);
        free_binary_heap(bh1_copy);
        return ALLOCATE_ERROR;
    }

    while ((*bh1)->size) 
    {
        const int bh1_top = dequeue(*bh1);
        if (enqueue(*bh_res, bh1_top) == ALLOCATE_ERROR) 
        {
            free_binary_heap(*bh_res);
            free_binary_heap(bh1_copy);
            free_binary_heap(bh2_copy);
            return ALLOCATE_ERROR;
        }
        if (enqueue(bh1_copy, bh1_top) == ALLOCATE_ERROR) 
        {
            free_binary_heap(*bh_res);
            free_binary_heap(bh1_copy);
            free_binary_heap(bh2_copy);
            return ALLOCATE_ERROR;
        }
    }
    free_binary_heap(*bh1);

    while ((*bh2)->size) 
    {
        const int bh2_top = dequeue(*bh2);
        if (enqueue(*bh_res, bh2_top) == ALLOCATE_ERROR) 
        {
            free_binary_heap(*bh_res);
            free_binary_heap(bh1_copy);
            free_binary_heap(bh2_copy);
            return ALLOCATE_ERROR;
        }
        if (enqueue(bh2_copy, bh2_top) == ALLOCATE_ERROR) 
        {
            free_binary_heap(*bh_res);
            free_binary_heap(bh1_copy);
            free_binary_heap(bh2_copy);
            return ALLOCATE_ERROR;
        }
    }
    free_binary_heap(*bh2);

    *bh1 = bh1_copy, *bh2 = bh2_copy;
    bh1_copy = NULL, bh2_copy = NULL;

    return SUCCESS;
}

int main() 
{
    BinaryHeap* bh1 = init_binary_heap();
    BinaryHeap* bh2 = init_binary_heap();

    enqueue(bh1, 8);
    enqueue(bh1, 2);
    enqueue(bh1, 7);
    enqueue(bh1, 9);
    enqueue(bh1, 1);

    enqueue(bh2, 10);
    enqueue(bh2, 6);
    enqueue(bh2, 3);
    enqueue(bh2, 4);
    enqueue(bh2, 5);

    for (int i = 0; i < bh1->size; ++i) 
    {
        printf("%d ", bh1->array[i]);
    }
    printf("\n");
    
    for (int i = 0; i < bh2->size; ++i) 
    {
        printf("%d ", bh2->array[i]);
    }
    printf("\n");
    printf("\n\n");
    BinaryHeap* bh3;
    save_merge(&bh3, &bh1, &bh2);
    for (int i = 0; i < bh1->size; ++i) 
    {
        printf("%d ", bh1->array[i]);
    }
    printf("\n");
    for (int i = 0; i < bh2->size; ++i) 
    {
        printf("%d ", bh2->array[i]);
    }
    printf("\n");
    for (int i = 0; i < bh3->size; ++i) 
    {
        printf("%d ", bh3->array[i]);
    }
    printf("\n");
}