#include "lem_in.h"

typedef struct s_room {
	char	*name;
	uint	id;
	uint	x;
	uint	y;
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
	uint size;
	bool (*cmp)(char *, char *);
} room_map;

room_pair	make_room_pair(char *first, t_room second);
room_node	*new_room_node(room_pair value, int col);
void		init_room_map(room_map *mp, bool (*cmp)(char *, char *));
void		clear_room_node(room_node *n);
void		clear_room_map(room_map *mp);
room_node	*front_room(room_map *mp);
room_node	*insert_room(room_map *mp, room_pair elem);
room_node	*get_room(room_map *mp, char *value);