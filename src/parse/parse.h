#pragma once

#include "../allocator/safe_malloc.h"
#include "parsing_iterator.h"
#include "readfile/readfile.h"
#include "containers/maps.h"
#include "containers/vec.h"

// 0   == StartRoom
// n-1 == EndRoom

typedef struct {
	/* (id -> (vec<connection>)) */
	vec			*graph;
	/* (id -> room) */
	t_room		**rooms;
	/* (name -> room) */
	room_map	rooms_map;
	u64			nb_ants;
	u64			nb_rooms;

	/* Private */
	u64			_indexing;
	t_room		*_start_room;
	t_room		*_end_room;
}	t_farm;

t_farm	*parse_farm_from_stdin();
void	free_farm(t_farm *farm);