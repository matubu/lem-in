#include "lem_in.h"

typedef struct s_int {
	size_t	id;
	size_t	x;
	size_t	y;
}	t_int;

typedef struct s_int_pair {
    int first;
    int second;
} int_pair;


typedef struct s_int_node {
    int_pair value;
    int col;
    struct s_int_node *par;
    struct s_int_node *left;
    struct s_int_node *right;

} int_node;

typedef struct s_int_map {
    int_node *root;
    int size;
    int (*cmp)(int, int);
} int_map;

int_pair make_int_pair(int first, int second);
int_node *new_int_node(int_pair value, int col);
void init_int_map(int_map *mp, int (*cmp)(int, int));
void clear_int_node(int_node *n);
int_node *front_int(int_map *mp);
void insert_int(int_map *mp, int_pair elem);
int_node *get_int(int_map *mp, int value);