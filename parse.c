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

	room.first = next_word(it);
	skip_whitespace(it);
	room.second.x = next_sizet(it);
	skip_whitespace(it);
	room.second.y = next_sizet(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");

	++farm->nb_rooms;
	room.second.id = ++farm->_nb_normal_rooms;

	room_node *node = insert_room(&farm->rooms, room);
	if (type == StartRoom)
	{
		--farm->_nb_normal_rooms;
		room.second.id = 0;
		P_EXPECT(farm->_start == NULL, it, "Duplicate start room");
		farm->_start = &node->value.second;
	}
	else if (type == EndRoom)
	{
		P_EXPECT(farm->_end == NULL, it, "Duplicate end room");
		farm->_end = &node->value.second;
	}
}

void parse_link(t_farm *farm, LineIterator *it) {
	if (farm->graph == NULL) {
		farm->graph = tmalloc(vec, farm->rooms.size);
		for (int i = 0; i < farm->rooms.size; ++i) {
			init_vec(farm->graph + i, 0, 0);
		}
	}

	char	*a = next_word(it);
	P_EXPECT(get(it) == '-', it, "The name of a room can only contains those characters: [a-zA-Z0-9_]");
	next(it);
	char	*b = next_word(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");

	size_t	id_a = get_room(&farm->rooms, a)->value.second.id;
	size_t	id_b = get_room(&farm->rooms, b)->value.second.id;
	push_back(farm->graph + id_a, id_b);
	push_back(farm->graph + id_b, id_a);

	free(a);
	free(b);
}

t_farm *parse_farm(char *filename) {
	// Read the file and create a iterator
	FileIterator	lines = create_file_iterator(filename);

	t_farm	*farm = smalloc(t_farm);
	init_room_map(&farm->rooms, greater_str);
	farm->graph = NULL;
	farm->_start = NULL;
	farm->_end = NULL;

	// Parse the number of ants
	LineIterator	line = next_line(&lines);
	P_EXPECT(line.ptr != NULL, &line, "Should contains the number of ants");
	farm->nb_ants = next_sizet(&line);
	P_EXPECT(get(&line) == '\0', &line, "Expected eol, should only contains the number of ants");

	// Parse rooms and links
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

	// Swap to get the StartRoom at 0
	//         and the EndRoom   at n-1

	free_file_iterator(&lines);

	return (farm);
}

void	free_farm(t_farm *farm) {
	free(farm);
}