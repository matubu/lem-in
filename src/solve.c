#include "parse.h"
#include "int_map.h"


void djikstra(vec *graph, vec *costs, int_map *available, int_map *from, int n) {
	// store visited nodes
	vec vis;
	init_vec(&vis, n, 0);

	// stock known cost to go to each node with cheaper cost first
	int_map pq;
	init_int_map(&pq, less);

	// djikstra
	insert_int(&pq, make_int_pair(0, 0));
	while (pq.size) {
		// get top element and pop it
		int_pair cur = top_int(&pq)->value; pop_int(&pq);

		// u -> current node idx
		int u = cur.second;

		vis.at(u) = 1;

		// get cost from u to each neighbor (v) and update costs if needed
		for (int i = graph[cur.second].size - 1; i >= 0; i--) {
			int v = graph[cur.second].at(i), cost;

			// if v is visisted or cheaper cost to v is known then skip
			if (vis.at(v) || (cost = upper_bound_int(available + v, cur.first)->value.first) > costs->at(v))
				continue;

			// update cost
			costs->at(v) = cost;
			insert_int(&pq, make_int_pair(cost, v));
			erase_int(from, v);
			insert_int(from, make_int_pair(v, u));
		}
	}

	clear_int_map(&pq);

	// check if end is reachable
	if (costs->at(n - 1) == INF) {
		printf("\033[1;91mInexistant path to end\n\033[0m");
		exit(1);
	}
}

void generate_path(vec *path, vec *costs, int_map *available, int_map *from, int n) {
	int cur = n - 1;
	int turn = costs->at(cur);
	while (1) {
		// add waiting turn
		while (turn > costs->at(cur))
			push_back(path, -1), turn--;

		// reserve room at given turn
		if (cur != n - 1)
			erase_int(available + cur, costs->at(cur));

		// add move to path
		push_back(path, cur);

		if (cur == 0)
			break;
		int_node *prev = get_int(from, cur);
		cur = prev->value.second;
		turn--;
	}
	reverse_vec(path);
}

void find_path(vec *graph, int n, int_map *available, vec *path) {
	int_map from;
	init_int_map(&from, less);

	vec costs;
	init_vec(&costs, n, 2147483647);
	costs.at(0) = 0;

	djikstra(graph, &costs, available, &from, n);

	generate_path(path, &costs, available, &from, n);
}

void print_connection(vec *graph, vec *vis, t_room **rooms, int u) {
	if (vis->at(u)) return;
	vis->at(u) = 1;
	for (int i = 0; i < graph[u].size; i++) {
		printf("%s-%s\n", rooms[u]->name, rooms[graph[u].at(i)]->name);
		print_connection(graph, vis, rooms, graph[u].at(i));
	}
}

void print_solution(vec *graph, vec *paths, int n, int m, t_room **rooms) {
	printf("%d\n", m);
	for (int i = 0; i < n; i++) {
		printf((i == 0 ? "##START\n" : (i == n - 1 ? "##END\n" : "")));
		printf("%s %zu %zu\n", rooms[i]->name, rooms[i]->x, rooms[i]->y);
	}

	vec vis; init_vec(&vis, n, 0);

	for (int i = 0; i < n; i++)
		if (!vis.at(i))
			print_connection(graph, &vis, rooms, i);

	printf("\n");
	int f = 1;

	for (int i = 1; f; i++) {
		f = 0;
		for (int j = 0; j < m; j++) {
			if (paths[j].size <= i || paths[j].at(i) == -1) continue;
			f = 1;
			printf("L%d-%s ", j + 1, rooms[paths[j].at(i)]->name);
		}
		if (f) printf("\n");
	}
}

void solve(t_farm *farm) {
	vec *graph = farm->graph;
	int n = farm->nb_rooms;
	int m = farm->nb_ants;

	// init and fill available
	int_map available[n];
	for (int i = 0; i < n; i++) {
		init_int_map(available + i, less);
		for (int j = 0; j < n + m - 1; j++)
			insert_int(available + i, make_int_pair(j, 0));
	}

	// init paths
	vec paths[m];
	for (int i = 0; i < m; i++)
		init_vec(paths + i, 0, 0);

	// generate path for each ant
	for (int i = 0; i < m; i++)
		find_path(graph, n, available, paths + i);

	print_solution(graph, paths, n, m, farm->rooms);
}
