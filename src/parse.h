#pragma once

#include "lem_in.h"
#include "room_map.h"

// 0   == StartRoom
// n-1 == EndRoom

typedef struct {
	/* (id -> (vec<connection>)) */
	vec			*graph;
	/* (id -> room) */
	t_room		**rooms;
	/* (name -> room) */
	room_map	rooms_map;
	uint		nb_ants;
	uint		nb_rooms;

	/* Private */
	uint		_indexing;
	t_room		*_start_room;
	t_room		*_end_room;
}	t_farm;

t_farm	*parse_farm(char *filename);
void	free_farm(t_farm *farm);