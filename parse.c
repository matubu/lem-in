#include "parse.h"
#include "iterator.h"

// TODO "warnings"

#define DefaultRoom 0
#define StartRoom 1
#define EndRoom 2

void parse_room(t_farm *farm, LineIterator *it, int type) {
	t_room	*room = smalloc(t_room);

	/*room->name = */ next_word(it);
	skip_whitespace(it);
	room->x = next_sizet(it);
	skip_whitespace(it);
	room->y = next_sizet(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");
	if (type == StartRoom)
	{
		P_EXPECT(farm->_start == NULL, it, "Duplicate start room");
		farm->_start = room;
	}
	if (type == EndRoom)
	{
		P_EXPECT(farm->_end == NULL, it, "Duplicate end room");
		farm->_end = room;
	}
	printf("[%s] id= x=%zu y=%zu\n", ((char *[]){"Default", "Start", "End"})[type], /*room->name,*/ room->x, room->y);
}

void parse_link(t_farm *farm, LineIterator *it) {
	if (farm->graph == NULL) {
		// farm->graph = tmalloc(vec, farm->rooms->size)
		// for (int i = 0; i < farm->rooms->size; ++i) {
		// 	init_vec(farm->rooms->arr[i], 0)
		// }
	}

	char	*a = next_word(it);
	P_EXPECT(get(it) == '-', it, "The name of a room can only contains those characters: [a-zA-Z0-9_]");
	next(it);
	char	*b = next_word(it);
	P_EXPECT(get(it) == '\0', it, "Expected eol");
	printf("[Link] %s %s\n", a, b);
	free(a);
	free(b);
}

t_farm *parse_farm(char *filename) {
	// Read the file and create a iterator
	FileIterator	lines = create_file_iterator(filename);

	t_farm	*farm = tmalloc(t_farm, 1);
	farm->rooms = NULL; // new_array()
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
			P_EXPECT(farm->graph == NULL, &line, "Cannot define room after links definition");
			// Room
			parse_room(farm, &line, DefaultRoom);
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