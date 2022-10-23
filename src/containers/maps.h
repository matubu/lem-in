#pragma once

#include "allocator/safe_malloc.h"
#include "string.h"
#include "utils.h"
#include "types.h"

#define BLACK 0
#define RED 1

/**
 * Room Map
 */

typedef struct s_room {
	char	*name;
	u64		id;
	u64		x;
	u64		y;
}	t_room;

typedef struct s_room_pair {
	char	*first;
	t_room	second;
} room_pair;


typedef struct s_room_node {
	room_pair			value;
	int					col;
	struct s_room_node	*par;
	struct s_room_node	*left;
	struct s_room_node	*right;

} room_node;

typedef struct s_room_map {
	room_node *root;
	u64 size;
	bool (*cmp)(char *, char *);
} room_map;

room_node	*new_room_node(room_pair value, int col);
void		init_room_map(room_map *mp, bool (*cmp)(char *, char *));
void		clear_room_node(room_node *n);
void		clear_room_map(room_map *mp);
room_node	*insert_room(room_map *mp, room_pair elem);
room_node	*get_room(room_map *mp, char *value);

/**
 * u64 Map
 */

typedef struct s_u64 {
	u64	id;
	u64	x;
	u64	y;
}	t_u64;

typedef struct s_u64_pair {
	u64	first;
	u64	second;
}	u64_pair;


typedef struct s_u64_node {
	u64_pair			value;
	u64					col;
	struct s_u64_node	*par;
	struct s_u64_node	*left;
	struct s_u64_node	*right;

}	u64_node;

typedef struct s_u64_map {
	u64_node		*root;
	u64				size;
	bool			(*cmp)(u64, u64);
}	u64_map;

u64_pair	make_u64_pair(u64 first, u64 second);
u64_node	*new_u64_node(u64_pair value, u64 col);
void		init_u64_map(u64_map *mp, bool (*cmp)(u64, u64));
void		clear_u64_node(u64_node *n);
void		clear_u64_map(u64_map *mp);
u64_node	*top_u64(u64_map *mp);
void		pop_u64(u64_map *pq);
void		insert_u64(u64_map *mp, u64_pair elem);
void		erase_u64(u64_map *mp, u64 val);
u64_node	*upper_bound_u64(u64_map *mp, u64 value);
u64_node	*get_u64(u64_map *mp, u64 value);