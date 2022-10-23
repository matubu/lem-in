#include "parse.h"
#include "uint_map.h"

#define ACTION_WAIT INF

void	djikstra(vec *graph, vec *costs, uint_map *available, uint_map *from, uint n) {
	// store visited nodes
	vec	vis;
	init_vec(&vis, n, 0);

	// stock known cost to go to each node with cheaper cost first
	uint_map	pq;
	init_uint_map(&pq, less);

	// djikstra
	insert_uint(&pq, make_uint_pair(0, 0));
	while (pq.size) {
		// get top element and pop it
		uint_pair	cur = top_uint(&pq)->value;
		pop_uint(&pq);

		// u -> current node idx
		int	u = cur.second;

		vis.at(u) = 1;

		// get cost from u to each neighbor (v) and update costs if needed
		for (uint i = 0; i < graph[cur.second].size; i++) {
			uint	v = graph[cur.second].at(i),
					cost;

			// if v is visited or cheaper cost to v is known then skip
			if (vis.at(v) || (cost = upper_bound_uint(available + v, cur.first)->value.first) > costs->at(v)) {
				continue;
			}

			// update cost
			costs->at(v) = cost;
			insert_uint(&pq, make_uint_pair(cost, v));
			erase_uint(from, v);
			insert_uint(from, make_uint_pair(v, u));
		}
	}

	clear_uint_map(&pq);
	clear_vec(&vis);

	printf("%u\n", costs->at(n - 1));
	// check if end is reachable
	if (costs->at(n - 1) == INF) {
		die("No path found");
	}
}

void	generate_path(vec *path, vec *costs, uint_map *available, uint_map *from, uint n) {
	uint	cur = n - 1;
	uint	turn = costs->at(cur);

	while (1) {
		// add waiting turn
		while (turn > costs->at(cur)) {
			push_back(path, ACTION_WAIT);
			turn--;
		}

		// reserve room at given turn
		if (cur != n - 1)
			erase_uint(available + cur, costs->at(cur));

		// add move to path
		push_back(path, cur);

		if (cur == 0)
			break;

		uint_node	*prev = get_uint(from, cur);
		cur = prev->value.second;
		turn--;
	}

	reverse_vec(path);
}

void find_path(vec *graph, uint n, uint_map *available, vec *path) {
	uint_map	from;
	init_uint_map(&from, less);

	vec	costs;
	init_vec(&costs, n, INF);
	costs.at(0) = 0;

	djikstra(graph, &costs, available, &from, n);

	generate_path(path, &costs, available, &from, n);

	clear_vec(&costs);
	clear_uint_map(&from);
}

void print_connection(vec *graph, vec *vis, t_room **rooms, uint u) {
	if (vis->at(u)) return;
	vis->at(u) = 1;
	for (uint i = 0; i < graph[u].size; i++) {
		printf("%s-%s\n", rooms[u]->name, rooms[graph[u].at(i)]->name);
		print_connection(graph, vis, rooms, graph[u].at(i));
	}
}

void	print_solution(vec *graph, vec *paths, uint n, uint m, t_room **rooms) {
	for (uint i = 0; i < n; i++) {
		if (i == 0)
			FD_PUT(1, "##START\n");
		else if (i == n - 1)
			FD_PUT(1, "##END\n");
		printf("%s %u %u\n", rooms[i]->name, rooms[i]->x, rooms[i]->y);
	}

	vec	vis;
	init_vec(&vis, n, 0);

	for (uint i = 0; i < n; i++)
		if (!vis.at(i))
			print_connection(graph, &vis, rooms, i);

	FD_PUT(1, "\n");
	int	f = 1;

	for (uint i = 1; f; i++) {
		f = 0;
		for (uint j = 0; j < m; j++) {
			if (paths[j].size <= i || paths[j].at(i) == ACTION_WAIT) {
				continue;
			}
			f = 1;
			printf("L%u-%s ", j + 1, rooms[paths[j].at(i)]->name);
		}
		if (f) {
			FD_PUT(1, "\n");
		}
	}

	clear_vec(&vis);
}

void	solve(t_farm *farm) {
	vec		*graph = farm->graph;
	uint	n = farm->nb_rooms;
	uint	m = farm->nb_ants;

	// init and fill available
	uint_map	available[n];
	for (uint i = 0; i < n; i++) {
		init_uint_map(available + i, less);
		for (uint j = 0; j < n + m - 1; j++) {
			insert_uint(available + i, make_uint_pair(j, 0));
		}
	}

	// init paths
	vec paths[m];
	// generate path for each ant
	for (uint i = 0; i < m; i++) {
		init_vec(paths + i, 0, 0);
		find_path(graph, n, available, paths + i);
	}

	LOG("Solution found");

	print_solution(graph, paths, n, m, farm->rooms);
	LOG("Solution printed");

	// free stuff
	for (uint i = 0; i < n; i++) {
		clear_uint_map(available + i);
	}
	for (uint i = 0; i < m; i++) {
		clear_vec(paths + i);
	}
	LOG("Solution freed");
}
