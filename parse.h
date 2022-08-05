#pragma once

#include "lem_in.h"

typedef struct s_room {
	char	*name;
	size_t	id;
	size_t	x;
	size_t	y;
}	t_room;

// 0   == StartRoom
// n-1 == EndRoom
typedef struct {
	t_room	*rooms;
	vec		*graph;
	size_t	nb_ants;

	t_room	*_start;
	t_room	*_end;
}	t_farm;

t_farm *parse_farm(char *filename);
void	free_farm(t_farm *farm);