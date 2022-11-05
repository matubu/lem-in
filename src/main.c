#include "io.h"
#include "parse/parse.h"
#include "solve.h"

void	lem_in(char *filename) {
	t_farm	*farm = parse_farm(filename);

	solve(farm);

	free_farm(farm);
}

int	main(int ac, char **av) {
	if (ac == 2)
		lem_in(av[1]);
	else
		lem_in("map/simple");
}