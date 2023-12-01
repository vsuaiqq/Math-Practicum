#include "arrays.h"

void free_array(Array* array) 
{
    if (!array) return;
    if (array->data) free(array->data);
    free(array);
}

void free_arrays(Arrays* arrays) 
{
    if (!arrays) return;
    if (arrays->data) 
    {
        for (int i = 0; i < arrays->size; ++i) 
        {
            free_array(arrays->data[i]);
        }
    }
    free(arrays);
}

status_code create_array(Array** array, const char name) 
{
    *array = (Array*)malloc(sizeof(Array));
    if (!*array) return ALLOCATE_ERROR;
    (*array)->name = name;
    (*array)->size = 0;
    (*array)->data = NULL;
    return SUCCESS;
}

status_code create_arrays(Arrays** arrays) 
{
    *arrays = (Arrays*)malloc(sizeof(Arrays));
    if (!*arrays) return ALLOCATE_ERROR;
    (*arrays)->size = 0;
    (*arrays)->data = NULL;
    return SUCCESS;
}

status_code load(Arrays* arrays, const char name, FILE* input_file) 
{
    Array* array_to_load = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            array_to_load = arrays->data[i];
        }
    }
    if (array_to_load) 
    {
        free_array(array_to_load);
        array_to_load = NULL;
    }
    if (create_array(&array_to_load, name) == ALLOCATE_ERROR) 
    {
        return ALLOCATE_ERROR;
    }
    if (arrays->size) 
    {
        Array** tmp = (Array**)realloc(arrays->data, arrays->size);
        if (!tmp) 
        {
            free_array(array_to_load);
            return ALLOCATE_ERROR;
        }
        arrays->data = tmp;
    }
    else 
    {
        arrays->data = (Array**)malloc(sizeof(Array*));
        if (!arrays->data) 
        {
            free_array(array_to_load);
            return ALLOCATE_ERROR;
        }
    }
    arrays->data[arrays->size++] = array_to_load;
    int num;
    while (fscanf(input_file, "%d", &num) == 1) 
    {
        if (!array_to_load->data) 
        {
            array_to_load->data = (int*)malloc(sizeof(int));
            if (!array_to_load->data) return ALLOCATE_ERROR;
            array_to_load->data[array_to_load->size++] = num;
        }
        else 
        {
            ++array_to_load->size;
            int* tmp = (int*)realloc(array_to_load->data, array_to_load->size * sizeof(int));
            if (!tmp) return ALLOCATE_ERROR;
            array_to_load->data = tmp;
            array_to_load->data[array_to_load->size - 1] = num;
        }
    }
    return SUCCESS;
}

status_code save(Arrays* arrays, const char name, FILE* stream) 
{
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            for (int j = 0; j < arrays->data[i]->size; ++j) 
            {
                fprintf(stream, "%d ", arrays->data[i]->data[j]);
            }
            return SUCCESS;
        }
    }
    return NOT_FOUND;
}

status_code rand_fill(Arrays* arrays, const char name, const int size, const int min, const int max) 
{
    if (size < 0 || (min > max)) return INVALID_DATA;
    Array* array_to_fill = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            array_to_fill = arrays->data[i];
        }
    }
    if (array_to_fill) 
    {
        free_array(array_to_fill);
        array_to_fill = NULL;
    }
    if (create_array(&array_to_fill, name) == ALLOCATE_ERROR) 
    {
        return ALLOCATE_ERROR;
    }
    if (arrays->size) 
    {
        Array** tmp = (Array**)realloc(arrays->data, arrays->size);
        if (!tmp) 
        {
            free_array(array_to_fill);
            return ALLOCATE_ERROR;
        }
        arrays->data = tmp;
    }
    else 
    {
        arrays->data = (Array**)malloc(sizeof(Array*));
        if (!arrays->data) 
        {
            free_array(array_to_fill);
            return ALLOCATE_ERROR;
        }
    }
    ++arrays->size;
    arrays->data[arrays->size - 1] = array_to_fill;
    array_to_fill->data = (int*)malloc(sizeof(int) * size);
    array_to_fill->size = size;
    if (!array_to_fill->data) return ALLOCATE_ERROR;
    srand(time(NULL));
    for (int i = 0; i < size; ++i)
    {
        array_to_fill->data[i] = get_rand(min, max);
    }
    return SUCCESS;
}

status_code concat(Arrays* arrays, const char dest_name, const char src_name) 
{
    if (dest_name == src_name) return INVALID_DATA;
    Array* dest_array = NULL;
    Array* src_array = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == dest_name) 
        {
            dest_array = arrays->data[i];
        }
        if (arrays->data[i]->name == src_name) 
        {
            src_array = arrays->data[i];
        }
    }
    if (!dest_array || !src_array) return NOT_FOUND;
    const int old_dist_size = dest_array->size;
    dest_array->size += src_array->size;
    int* tmp = (int*)realloc(dest_array->data, dest_array->size);
    if (!tmp) return ALLOCATE_ERROR;
    dest_array->data = tmp;
    for (int i = old_dist_size, j = 0; i < dest_array->size; ++i, ++j) 
    {
        dest_array->data[i] = src_array->data[j];
    }
    return SUCCESS;
}

status_code copy(Arrays* arrays, const char dest_name, const char src_name, const int left, const int right) 
{
    if (left > right || left < 0) return INVALID_DATA;
    if (dest_name == src_name) return INVALID_DATA;
    Array* dest_array = NULL;
    Array* src_array = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == dest_name) 
        {
            dest_array = arrays->data[i];
        }
        if (arrays->data[i]->name == src_name) 
        {
            src_array = arrays->data[i];
        }
    }
    if (!src_array) return NOT_FOUND;
    if (right >= src_array->size) return INVALID_DATA;
    if (dest_array) 
    {
        free_array(dest_array);
        dest_array = NULL;
    }
    if (create_array(&dest_array, dest_name) == ALLOCATE_ERROR) 
    {
        return ALLOCATE_ERROR;
    }
    dest_array->size = right - left + 1;
    dest_array->data = (int*)malloc(sizeof(int)  * dest_array->size);
    if (!dest_array->data) return ALLOCATE_ERROR;
    for (int i = 0, j = left; i < dest_array->size; ++i, ++j) 
    {
        dest_array->data[i] = src_array->data[j];
    }
    return SUCCESS;
}

status_code remove_array(Arrays* arrays, const char name) 
{
    Array* array_to_remove = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            array_to_remove = arrays->data[i];
        }
    }
    if (!array_to_remove) return NOT_FOUND;
    free_array(array_to_remove);
    array_to_remove = NULL;
    return SUCCESS;
}

status_code remove_elements(Arrays* arrays, const char name, const int start, const int count) 
{
    Array* array_to_remove_elements = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            array_to_remove_elements = arrays->data[i];
        }
    }
    if (!array_to_remove_elements) return NOT_FOUND;
    if (count < 0 || start < 0 || (start + count) > array_to_remove_elements->size) return INVALID_DATA;
    for (int i = start + count; i < array_to_remove_elements->size; ++i) 
    {
        array_to_remove_elements->data[i - count] = array_to_remove_elements->data[i];
    }
    array_to_remove_elements->size -= count;
    int* tmp = (int*)realloc(array_to_remove_elements->data, array_to_remove_elements->size);
    if (!tmp) return ALLOCATE_ERROR;
    array_to_remove_elements->data = tmp;
    return SUCCESS;
}

status_code sort(Arrays* arrays, const char name, const sort_type type) 
{
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            switch (type)
            {
                case ASCENDING:
                    qsort(arrays->data[i]->data, arrays->data[i]->size, sizeof(int), cmp_asc);
                    break;
                case DESCENDING:
                    qsort(arrays->data[i]->data, arrays->data[i]->size, sizeof(int), cmp_dsc);
                    break;
                case RANDOM:
                    qsort(arrays->data[i]->data, arrays->data[i]->size, sizeof(int), cmp_rand);
                    break;
                default:
                    break;
            }
            return SUCCESS;
        }
    }
    return NOT_FOUND;
}

status_code stats(Arrays* arrays, const char name) 
{
    Array* array_to_get_stats = NULL;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            array_to_get_stats = arrays->data[i];
        }
    }
    if (!array_to_get_stats) return NOT_FOUND;

    int max = array_to_get_stats->data[0], min = array_to_get_stats->data[0];
    double avg = 0;
    for (int i = 0; i < array_to_get_stats->size; ++i) 
    {
        max = fmax(max, array_to_get_stats->data[i]);
        min = fmin(min, array_to_get_stats->data[i]);
        avg += array_to_get_stats->data[i];
    }
    avg /= array_to_get_stats->size;

    double max_deviation = 0;
    for (int i = 0; i < array_to_get_stats->size; ++i) 
    {
        const double deviation = fabs(array_to_get_stats->data[i] - avg);
        if (deviation - max_deviation > EPS) max_deviation = deviation;
    }

    int most_common, max_count = 0;
    for (int i = 0; i < array_to_get_stats->size; ++i) 
    {
        int cur_count = 1;
        for (int j = i + 1; j < array_to_get_stats->size; ++j) 
        {
            if (array_to_get_stats->data[j] == array_to_get_stats->data[i]) ++cur_count;
        }
        if (cur_count > max_count) 
        {
            max_count = cur_count;
            most_common = array_to_get_stats->data[i];
        }
        else if (cur_count == max_count) 
        {
            most_common = fmax(most_common, array_to_get_stats->data[i]);
        }
    }

    printf("Array %c stats:\n", name);
    printf("Size: %d\n", array_to_get_stats->size);
    printf("Max element: %d\n", max);
    printf("Min element: %d\n", min);
    printf("Most common element: %d\n", most_common);
    printf("Average elements value: %lf\n", avg);
    printf("Max deviation from average: %lf\n", max_deviation);

    return SUCCESS;
}

status_code print_arr_slice(Arrays* arrays, const char name, const int left, const int right) 
{
    if (left > right || left < 0) return INVALID_DATA;
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            if (right >= arrays->data[i]->size) return INVALID_DATA;
            int j = left;
            while (j <= right)
            {
                printf("%d ", arrays->data[i]->data[j]);
                ++j;
            }
            printf("\n");
            return SUCCESS;
        }
    }
    return NOT_FOUND;
}

status_code print_arr(Arrays* arrays, const char name) 
{
    for (int i = 0; i < arrays->size; ++i) 
    {
        if (arrays->data[i]->name == name) 
        {
            for (int j = 0; j < arrays->data[i]->size; ++j) 
            {
                printf("%d ", arrays->data[i]->data[j]);
            }
            printf("\n");
            return SUCCESS;
        }
    }
    return NOT_FOUND;
}