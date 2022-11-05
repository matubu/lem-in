#include "solve.h"

#define INF u64_MAX
#define ACTION_WAIT INF

void	djikstra(vec *graph, vec *costs, u64_map *available, u64_map *from, u64 n) {
	// store visited nodes
	vec	vis;
	init_vec(&vis, n, 0);

	// stock known cost to go to each node with cheaper cost first
	u64_map	pq;
	init_u64_map(&pq, less);

	// djikstra
	insert_u64(&pq, make_u64_pair(0, 0));
	while (pq.size) {
		// get top element and pop it
		u64_pair	cur = top_u64(&pq)->value;
		pop_u64(&pq);

		// u -> current node idx
		int	u = cur.second;

		vis.at(u) = 1;

		// get cost from u to each neighbor (v) and update costs if needed
		for (u64 i = 0; i < graph[cur.second].size; i++) {
			u64	v = graph[cur.second].at(i),
					cost;

			// if v is visited or cheaper cost to v is known then skip
			if (vis.at(v) || (cost = upper_bound_u64(available + v, cur.first)->value.first) > costs->at(v)) {
				continue;
			}

			// update cost
			costs->at(v) = cost;
			insert_u64(&pq, make_u64_pair(cost, v));
			erase_u64(from, v);
			insert_u64(from, make_u64_pair(v, u));
		}
	}

	clear_u64_map(&pq);
	clear_vec(&vis);

	// check if end is reachable
	if (costs->at(n - 1) == INF) {
		die("No path found");
	}
}

void	generate_path(vec *path, vec *costs, u64_map *available, u64_map *from, u64 n) {
	u64	cur = n - 1;
	u64	turn = costs->at(cur);

	while (1) {
		// add waiting turn
		while (turn > costs->at(cur)) {
			push_back(path, ACTION_WAIT);
			turn--;
		}

		// reserve room at given turn
		if (cur != n - 1)
			erase_u64(available + cur, costs->at(cur));

		// add move to path
		push_back(path, cur);

		if (cur == 0)
			break;

		u64_node	*prev = get_u64(from, cur);
		cur = prev->value.second;
		turn--;
	}

	reverse_vec(path);
}

void find_path(vec *graph, u64 n, u64_map *available, vec *path) {
	u64_map	from;
	init_u64_map(&from, less);

	vec	costs;
	init_vec(&costs, n, INF);
	costs.at(0) = 0;

	djikstra(graph, &costs, available, &from, n);

	generate_path(path, &costs, available, &from, n);

	clear_vec(&costs);
	clear_u64_map(&from);
}

void print_connection(vec *graph, vec *vis, t_room **rooms, u64 u) {
	if (vis->at(u)) return;
	vis->at(u) = 1;
	for (u64 i = 0; i < graph[u].size; i++) {
		if (!vis->at(graph[u].at(i))) {
			fd_put(1, rooms[u]->name);
			FD_PUT(1, "-");
			fd_put(1, rooms[graph[u].at(i)]->name);
			FD_PUT(1, "\n");
		}
	}
	for (u64 i = 0; i < graph[u].size; i++)
		print_connection(graph, vis, rooms, graph[u].at(i));
}

void	print_solution(vec *graph, vec *paths, u64 n, u64 m, t_room **rooms) {
	fd_put_u64(1, m);
	FD_PUT(1, "\n");
	for (u64 i = 0; i < n; i++) {
		if (i == 0) {
			FD_PUT(1, "##START\n");
		}
		else if (i == n - 1) {
			FD_PUT(1, "##END\n");
		}
		fd_put(1, rooms[i]->name);
		FD_PUT(1, " ");
		fd_put_u64(1, rooms[i]->x);
		FD_PUT(1, " ");
		fd_put_u64(1, rooms[i]->y);
		FD_PUT(1, "\n");
	}

	vec	vis;
	init_vec(&vis, n, 0);

	for (u64 i = 0; i < n; i++)
		if (!vis.at(i))
			print_connection(graph, &vis, rooms, i);

	FD_PUT(1, "\n");
	int	f = 1;

	for (u64 i = 1; f; i++) {
		f = 0;
		for (u64 j = 0; j < m; j++) {
			if (paths[j].size <= i || paths[j].at(i) == ACTION_WAIT) {
				continue;
			}
			f = 1;
			FD_PUT(1, "L");
			fd_put_u64(1, j + 1);
			FD_PUT(1, "-");
			fd_put(1, rooms[paths[j].at(i)]->name);
			FD_PUT(1, " ");
		}
		if (f) {
			FD_PUT(1, "\n");
		}
	}

	clear_vec(&vis);
}

void	solve(t_farm *farm) {
	vec		*graph = farm->graph;
	u64	n = farm->nb_rooms;
	u64	m = farm->nb_ants;

	// init and fill available
	u64_map	available[n];
	for (u64 i = 0; i < n; i++) {
		init_u64_map(available + i, less);
		for (u64 j = 0; j < n + m - 1; j++) {
			insert_u64(available + i, make_u64_pair(j, 0));
		}
	}

	// init paths
	vec paths[m];
	// generate path for each ant
	for (u64 i = 0; i < m; i++) {
		init_vec(paths + i, 0, 0);
		find_path(graph, n, available, paths + i);
	}


	print_solution(graph, paths, n, m, farm->rooms);

	// free stuff
	for (u64 i = 0; i < n; i++) {
		clear_u64_map(available + i);
	}
	for (u64 i = 0; i < m; i++) {
		clear_vec(paths + i);
	}
}
