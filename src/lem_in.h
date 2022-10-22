#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 100

#define at(i) arr[i]

#define INF 2147483647

#define BLACK 0
#define RED 1

typedef struct s_vec {
	int		*arr;
	int		size;
	int		capacity;
}	vec;

typedef struct s_pair {
	int	first;
	int	second;
}	pair;

typedef struct s_node {
	pair			value;
	int				col;
	struct s_node	*par;
	struct s_node	*left;
	struct s_node	*right;

}	node;

typedef struct s_priority_queue {
	node	*root;
	int		size;
	int		(*cmp)(int, int);
}	priority_queue;


/* utils */
void	ft_memcpy(void *dst, void *src, size_t n);

/* vec */
void	init_vec(vec *v, int size, int val);
int		push_back(vec *v, int val);
void	pop_back(vec *v);
void	reverse_vec(vec *v);
void	clear_vec(vec *v);

/* priority_queue */
void	push(priority_queue *pq, pair elem);
void	pop(priority_queue *pq);
node	*top(priority_queue *pq);
void	clear_priority_queue(priority_queue *pq);
void	init_priority_queue(priority_queue *pq, int (*cmp)(int, int));
node	*new_node(pair value, int col);
pair	make_pair(int first, int second);

/* comp */
int		greater(int a, int b);
int		less(int a, int b);