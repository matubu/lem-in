#include <time.h>
#include <stdio.h>
#include "lem_in.h"
#include "parse.h"
#include "string.h"
#include "int_map.h"

#define time(code) ({ \
	clock_t	before = clock(); \
	typeof(code) tmp = code; \
	printf("\033[1;94m" #code "\033[0m: took %lfs\n", (double)(clock() - before) / CLOCKS_PER_SEC); \
	tmp; \
})

void find_path(vec *graph, int n, int_map *taken, vec *path, int idx) {
	//printf("__ANTS %d__\n", idx);
	int_map pq;
	init_int_map(&pq, less);

	vec vis;
	init_vec(&vis, n, 0);

	int_map from;
	init_int_map(&from, less);

	vec costs;
	init_vec(&costs, n, 2147483647);
	costs.at(0) = 0;

	insert_int(&pq, make_int_pair(0, 0));
	while (pq.size) {
		int_pair cur = top_int(&pq)->value;
		pop_int(&pq);
		vis.at(cur.second) = 1;
		for (int i = graph[cur.second].size - 1; i >= 0; i--) {
			int v = graph[cur.second].at(i);
			if (vis.at(v))
				continue;
			int cost = upper_bound_int(taken + v, cur.first)->value.first;
			if (cost > costs.at(v))
				continue;
			costs.at(v) = cost;
			insert_int(&pq, make_int_pair(cost, v));
			erase_int(&from, v);
			insert_int(&from, make_int_pair(v, cur.second));
		}
	}

	int cur = n - 1;
	int turn = costs.at(cur);
	while (1) {
		while (turn > costs.at(cur)) {
			push_back(path, -1);
			turn--;

		}
		if (cur != n - 1) {
			erase_int(taken + cur, costs.at(cur));
		}

		push_back(path, cur);

		int_node *prev = get_int(&from, cur);
		if (cur == 0)
			break;
		cur = prev->value.second;
		turn--;
	}

	// reverse path
	for (int i = 0; i < path->size / 2; i++) {
		int tmp = path->at(i);
		path->at(i) = path->at(path->size - i - 1);
		path->at(path->size - i - 1) = tmp;
	}

	printf("ants %d : ", idx);
	for (int i = 0; i < path->size; i++) {
		if (path->at(i) == -1)
			printf("WAIT");
		else
			printf("%d", path->at(i));
		if (i < path->size - 1)
			printf(" -> ");
	}
	printf("\n");


	clear_int_map(&pq);
}

void print_graph(vec *graph, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d : ", i);
		for (int j = 0; j < graph[i].size; j++) {
			printf("%d ", graph[i].at(j));
		}
		printf("\n");
	}
}

void solve(t_farm *farm) {
	//room_map *rooms = &farm->rooms;

	vec *graph = farm->graph;
	//print_graph(graph, farm->nb_rooms);

	int n = farm->nb_rooms;
	int m = farm->nb_ants;

	int_map taken[n];


	printf("number of rooms : %d\n", n);

	for (int i = 0; i < n; i++) {
		init_int_map(taken + i, less);
		//printf("i : %d\n", i);
		for (int j = 0; j < n + m + 100; j++)
			insert_int(taken + i, make_int_pair(j, 0));
	}
	printf("TEST\n");

	vec paths[m];
	for (int i = 0; i < m; i++)
		init_vec(paths + i, 0, 0);

	for (int i = 0; i < m; i++) {
		find_path(graph, n, taken, paths + i, i);
	}
	//find_path(graph, n, taken, paths, 0);
	int number_of_moves = 0;
	for (int i = 0; i < m; i++) {
		if (paths[i].size - 1 > number_of_moves)
			number_of_moves = paths[i].size - 1;
	}
	printf("number of moves : %d\n", number_of_moves);
}


void	lem_in(char *filename) {
	t_farm	*farm = time(parse_farm(filename));

	solve(farm);	

	free_farm(farm);
}

int main(int ac, char **av) {
	if (ac == 2)
		lem_in(av[1]);
	else
		lem_in("map/simple");
}