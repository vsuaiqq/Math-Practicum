#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) 
{
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* O(log(n)), n - длина массива

функция partition:
    выбирает опорный элемент как крайний правый
    перемещает элементы относительно него так, чтобы все элементы < опорного находились слева от него, а все > справа
    возвращает окончательную позицию опорного элемента

функция quicksort:
    проверяет что левая граница массива меньше правой
    вызывает пивот и получает позицию опорного элемента
    рекурсивно делает два вызова:
        quicksort(arr, l, pivot - 1)
        quicksort(arr, pivot + 1, r)

Линеарифмическая сложность обусловлена делением массива на две части на каждом уровне рекурсии

*/

int partition(int* arr, const int left, const int right) 
{
    const int pivot_val = arr[right];
    int j = left - 1;
    for (int i = left; i < right; ++i) 
    {
        if (arr[i] <= pivot_val) 
        {
            swap(&arr[i], &arr[++j]);
        }
    }
    swap(&arr[++j], &arr[right]);
    return j;
}

void my_quicksort(int* arr, const int left, const int right) 
{
    if (left >= right) return;
    const int pivot = partition(arr, left, right);
    my_quicksort(arr, left, pivot - 1);
    my_quicksort(arr, pivot + 1, right);
}

int main() 
{
    int* arr = (int*)malloc(sizeof(int) * 5);
    arr[0] = -10;
    arr[1] = 123;
    arr[2] = 1;
    arr[3] = -333;
    arr[4] = 1212;
    my_quicksort(arr, 0, 4);
    for (int i = 0 ; i < 5; ++i) printf("%d ", arr[i]);
    printf("\n");
}