#include <time.h>
#include <stdio.h>
#include "lem_in.h"
#include "parse.h"
#include "string.h"
#include "int_map.h"

void solve(t_farm *farm);

void	lem_in(char *filename) {
	t_farm	*farm = parse_farm(filename);
	LOG("Farm parsed");

	solve(farm);
	LOG("Solved");

	free_farm(farm);
	LOG("Farm cleaned up");
}

int main(int ac, char **av) {
	if (ac == 2)
		lem_in(av[1]);
	else
		lem_in("map/simple");
}