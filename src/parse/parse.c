#include "parse.h"

#define NormalRoom 0
#define StartRoom 1
#define EndRoom 2

void	parse_room(t_farm *farm, LineIterator *it, int type) {
	P_EXPECT(farm->graph == NULL, it, "Cannot define room after links definition");

	room_pair	room;

	room.first = room.second.name = next_word(it);
	skip_whitespace(it);
	room.second.x = next_u64(it);
	skip_whitespace(it);
	room.second.y = next_u64(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");

	++farm->nb_rooms;

	if (type == StartRoom)
		room.second.id = 0;
	else
		room.second.id = farm->_indexing++;

	room_node *node = insert_room(&farm->rooms_map, room);

	if (type == StartRoom)
	{
		P_EXPECT(farm->_start_room == NULL, it, "Duplicate start room");
		farm->_start_room = &node->value.second;
	}
	if (type == EndRoom)
	{
		P_EXPECT(farm->_end_room == NULL, it, "Duplicate end room");
		farm->_end_room = &node->value.second;
	}
}

void	map_to_room(t_farm *farm, room_node *node) {
	if (!node) {
		return ;
	}
	map_to_room(farm, node->left);
	farm->rooms[node->value.second.id] = &node->value.second;
	map_to_room(farm, node->right);
}

void	parse_link(t_farm *farm, LineIterator *it) {
	if (farm->graph == NULL) {
		P_EXPECT(farm->_start_room != NULL, it, "Missing start room");
		P_EXPECT(farm->_end_room != NULL, it, "Missing end room");

		// Init graph
		farm->graph = safe_malloc(sizeof(vec), farm->nb_rooms);
		for (u64 i = 0; i < farm->nb_rooms; ++i) {
			init_vec(farm->graph + i, 0, 0);
		}

		// Generate (id -> room) table
		farm->rooms = safe_malloc(sizeof(t_room *), farm->nb_rooms);
		map_to_room(farm, farm->rooms_map.root);

		// Mode end room to n - 1
		t_room *tmp = farm->_end_room;
		u64	id_end = farm->_end_room->id;
		u64	id_swap = farm->nb_rooms - 1;
		farm->rooms[id_swap]->id = id_end;
		farm->rooms[id_end]->id = id_swap;
		farm->rooms[id_end] = farm->rooms[id_swap];
		farm->rooms[id_swap] = tmp;
	}

	char	*a = next_word(it);
	P_EXPECT(get(it) == '-', it, "The name of a room can only contains those characters: [a-zA-Z0-9_]");
	room_node	*node_a = get_room(&farm->rooms_map, a);
	P_EXPECT(node_a, it, "No such variable");
	next_void(it);
	char	*b = next_word(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");
	room_node	*node_b = get_room(&farm->rooms_map, b);
	P_EXPECT(node_b, it, "No such variable");

	u64	id_a = node_a->value.second.id;
	u64	id_b = node_b->value.second.id;

	P_EXPECT(id_a != id_b, it, "Cannot create circular link");
	for (u64 i = 0; i < farm->graph[id_a].size; ++i) {
		P_EXPECT(id_b != farm->graph[id_a].at(i), it, "Link redefinition");
	}

	push_back(farm->graph + id_a, id_b);
	push_back(farm->graph + id_b, id_a);

	free(a);
	free(b);
}

t_farm	*parse_farm(char *filename) {
	// Read the file and create a iterator
	FileIterator	lines = create_file_iterator(filename);

	t_farm			*farm = safe_malloc(sizeof(t_farm), 1);
	init_room_map(&farm->rooms_map, greater_str);
	farm->graph = NULL;
	farm->nb_rooms = 0;
	farm->_indexing = 1;
	farm->_start_room = NULL;
	farm->_end_room = NULL;

	// Parse the number of ants
	LineIterator	line = next_line(&lines);
	farm->nb_ants = next_u64(&line);
	P_EXPECT(get(&line) == '\0', &line, "Expected eol, should only contains the number of ants");

	// Parse rooms_map and links
	while (!is_it_end(&lines)) {
		line = next_line(&lines);

		P_EXPECT(!is_whitespace(get(&line)), &line, "Unexpected whitespace");
		if (get(&line) == '\0') {
			continue ;
		}
		
		if (get(&line) == '#') {
			// Comment
			next_void(&line);
			if (next(&line) == '#') {
				LineIterator	next = next_line(&lines);
				// Room type
				if (equal_str(get_ptr(&line), "start")) {
					parse_room(farm, &next, StartRoom);
				} else if (equal_str(get_ptr(&line), "end")) {
					parse_room(farm, &next, EndRoom);
				} else {
					parsing_error(&line, "Can only be \"start\" or \"end\"");
				}
			}
			continue ;
		}
		if (contains(line.ptr, '-')) {
			// Link
			parse_link(farm, &line);
		} else {
			// Room
			parse_room(farm, &line, NormalRoom);
		}
	}

	P_EXPECT(farm->graph != NULL, &line, "Missing links");

	free_file_iterator(&lines);

	return (farm);
}

void	free_farm(t_farm *farm) {
	clear_room_map(&farm->rooms_map);
	for (u64 i = 0; i < farm->nb_rooms; ++i) {
		clear_vec(farm->graph + i);
	}
	free(farm->graph);
	free(farm->rooms);
	free(farm);
}