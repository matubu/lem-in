#pragma once

#include "lem_in.h"
#include "room_map.h"

// 0   == StartRoom
// n-1 == EndRoom
typedef struct {
	room_map	rooms;
	vec			*graph;
	size_t		nb_ants;
	size_t		nb_rooms;

	size_t		_nb_normal_rooms;
	t_room		*_start;
	t_room		*_end;
}	t_farm;

t_farm *parse_farm(char *filename);
void	free_farm(t_farm *farm);