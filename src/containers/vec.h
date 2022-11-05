#pragma once

#include "../allocator/safe_malloc.h"
#include "utils.h"
#include "types.h"

/* vec */
typedef struct s_vec {
	u64	*arr;
	u64	size;
	u64	capacity;
}	vec;

#define at(i) arr[i]

void	init_vec(vec *v, u64 size, u64 val);
bool	push_back(vec *v, u64 val);
void	pop_back(vec *v);
void	reverse_vec(vec *v);
void	clear_vec(vec *v);