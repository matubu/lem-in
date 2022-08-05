#include "lem_in.h"

/*
struct vec {
    int		*arr;
    int		size;
    int		capacity;
};
*/

void init_vec(vec *v, int size, int val) {
    v->arr = malloc(sizeof(int) * size);
    v->size = size;
    v->capacity = size;
    for (int i = 0; i < size; i++)
        v->arr[i] = val;
}

int push_back(vec *v, int val) {
    if (v->size == v->capacity) {
        v->capacity = (v->capacity ? v->capacity * 2 : 1);
        int *tmp = malloc(sizeof(int) * v->capacity);
        if (tmp)
            ft_memcpy(tmp, v->arr, sizeof(int) * v->size);
        free(v->arr);
        v->arr = tmp;
    }
    if (v->arr)
        v->arr[v->size++] = val;
    return (v->arr != NULL);
}

void pop_back(vec *v) {
    if (v->size) 
        v->size--;
}