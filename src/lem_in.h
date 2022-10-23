#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define BUFFER_SIZE 100

#define at(i) arr[i]

#define INF UINT_MAX

#define BLACK 0
#define RED 1

typedef struct s_vec {
	uint	*arr;
	uint	size;
	uint	capacity;
}	vec;

/* utils */
void	ft_memcpy(void *dst, void *src, uint n);

/* vec */
void	init_vec(vec *v, uint size, int val);
bool	push_back(vec *v, uint val);
void	pop_back(vec *v);
void	reverse_vec(vec *v);
void	clear_vec(vec *v);

/* comp */
bool	greater(uint a, uint b);
bool	less(uint a, uint b);