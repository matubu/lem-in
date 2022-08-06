#include "parse.h"
#include "iterator.h"
#include "string.h"

// TODO warnings

#define NormalRoom 0
#define StartRoom 1
#define EndRoom 2

void parse_room(t_farm *farm, LineIterator *it, int type) {
	P_EXPECT(farm->graph == NULL, it, "Cannot define room after links definition");

	room_pair	room;

	room.first = room.second.name = next_word(it);
	skip_whitespace(it);
	room.second.x = next_sizet(it);
	skip_whitespace(it);
	room.second.y = next_sizet(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");

	++farm->nb_rooms;

	if (type == StartRoom)
		room.second.id = 0;
	else
		room.second.id = farm->_indexing++;

	room_node *node = insert_room(&farm->rooms_map, room);

	if (type == StartRoom)
	{
		P_EXPECT(farm->_start == NULL, it, "Duplicate start room");
		farm->_start = &node->value.second;
	}
	if (type == EndRoom)
	{
		P_EXPECT(farm->_end == NULL, it, "Duplicate end room");
		farm->_end = &node->value.second;
	}
}

void	map_to_room(t_farm *farm, room_node *node) {
	if (!node) {
		return ;
	}
	map_to_room(farm, node->left);
	map_to_room(farm, node->right);
	farm->rooms[node->value.second.id] = &node->value.second;
}

void parse_link(t_farm *farm, LineIterator *it) {
	if (farm->graph == NULL) {
		// Init graph
		farm->graph = tmalloc(vec, farm->nb_rooms);
		for (int i = 0; i < farm->nb_rooms; ++i) {
			init_vec(farm->graph + i, 0, 0);
		}

		// Generate (id -> room) table
		farm->rooms = tmalloc(t_room *, farm->nb_rooms);
		map_to_room(farm, farm->rooms_map.root);

		// Mode end room to n - 1
		farm->rooms[farm->nb_rooms - 1]->id = farm->_end->id;
		farm->_end->id = farm->nb_rooms - 1;
	}

	char	*a = next_word(it);
	P_EXPECT(get(it) == '-', it, "The name of a room can only contains those characters: [a-zA-Z0-9_]");
	next(it);
	char	*b = next_word(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");

	size_t	id_a = get_room(&farm->rooms_map, a)->value.second.id;
	size_t	id_b = get_room(&farm->rooms_map, b)->value.second.id;
	push_back(farm->graph + id_a, id_b);
	push_back(farm->graph + id_b, id_a);

	free(a);
	free(b);
}

t_farm *parse_farm(char *filename) {
	// Read the file and create a iterator
	FileIterator	lines = create_file_iterator(filename);

	t_farm	*farm = smalloc(t_farm);
	init_room_map(&farm->rooms_map, greater_str);
	farm->graph = NULL;
	farm->nb_rooms = 0;
	farm->_indexing = 1;
	farm->_start = NULL;
	farm->_end = NULL;

	// Parse the number of ants
	LineIterator	line = next_line(&lines);
	P_EXPECT(line.ptr != NULL, &line, "Should contains the number of ants");
	farm->nb_ants = next_sizet(&line);
	P_EXPECT(get(&line) == '\0', &line, "Expected eol, should only contains the number of ants");

	// Parse rooms_map and links
	while (!is_it_end(&lines)) {
		line = next_line(&lines);

		P_EXPECT(!is_whitespace(get(&line)), &line, "Unexpected whitespace");
		P_EXPECT(get(&line) != '\0', &line, "Unexpected empty line");
		
		if (get(&line) == '#') {
			// Comment
			next(&line);
			if (get(&line) == '#') {
				next(&line);
				LineIterator	next = next_line(&lines);
				// Room type
				if (equal_str(get_ptr(&line), "start")) {
					P_EXPECT(next.ptr != NULL, &next, "Expected a newline with a room just after \"##start\"");
					parse_room(farm, &next, StartRoom);
				} else if (equal_str(get_ptr(&line), "end")) {
					P_EXPECT(next.ptr != NULL, &next, "Expected a newline with a room just after \"##end\"");
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

	P_EXPECT(farm->_start != NULL, &line, "Missing start room");
	P_EXPECT(farm->_end != NULL, &line, "Missing end room");

	free_file_iterator(&lines);

	return (farm);
}

void	free_farm(t_farm *farm) {
	free(farm);
}