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

void solve(t_farm *farm);

void	lem_in(char *filename) {
	t_farm	*farm = time(parse_farm(filename));

	time(({solve(farm); 42;}));

	free_farm(farm);
}

int main(int ac, char **av) {
	if (ac == 2)
		lem_in(av[1]);
	else
		lem_in("map/simple");
}