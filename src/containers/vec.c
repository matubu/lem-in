#include "vec.h"

void	init_vec(vec *v, u64 size, u64 val) {
	v->arr = safe_malloc(sizeof(u64), size);
	v->size = size;
	v->capacity = size;
	for (u64 i = 0; i < size; i++)
		v->arr[i] = val;
}

bool	push_back(vec *v, u64 val) {
	if (v->size == v->capacity) {
		v->capacity = (v->capacity ? v->capacity * 2 : 1);
		u64	*tmp = safe_malloc(sizeof(u64), v->capacity);
		ft_memcpy(tmp, v->arr, sizeof(u64) * v->size);
		free(v->arr);
		v->arr = tmp;
	}
	if (v->arr)
		v->arr[v->size++] = val;
	return (v->arr != NULL);
}

void	pop_back(vec *v) {
	if (v->size) 
		v->size--;
}

void	reverse_vec(vec *v) {
	for (u64 i = 0; i < v->size / 2; i++) {
		int tmp = v->arr[i];
		v->arr[i] = v->arr[v->size - i - 1];
		v->arr[v->size - i - 1] = tmp;
	}
}

void	clear_vec(vec *v) {
	free(v->arr);
}