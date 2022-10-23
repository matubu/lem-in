#include "lem_in.h"

typedef struct s_uint {
	uint	id;
	uint	x;
	uint	y;
}	t_uint;

typedef struct s_uint_pair {
	uint	first;
	uint	second;
}	uint_pair;


typedef struct s_uint_node {
	uint_pair			value;
	uint					col;
	struct s_uint_node	*par;
	struct s_uint_node	*left;
	struct s_uint_node	*right;

}	uint_node;

typedef struct s_uint_map {
	uint_node	*root;
	uint			size;
	bool			(*cmp)(uint, uint);
}	uint_map;

uint_pair	make_uint_pair(uint first, uint second);
uint_node	*new_uint_node(uint_pair value, uint col);
void		init_uint_map(uint_map *mp, bool (*cmp)(uint, uint));
void		clear_uint_node(uint_node *n);
void		clear_uint_map(uint_map *mp);
uint_node	*top_uint(uint_map *mp);
void		pop_uint(uint_map *pq);
void		insert_uint(uint_map *mp, uint_pair elem);
void		erase_uint(uint_map *mp, uint val);
uint_node	*upper_bound_uint(uint_map *mp, uint value);
uint_node	*get_uint(uint_map *mp, uint value);