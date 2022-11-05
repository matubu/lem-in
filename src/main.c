#include "io.h"
#include "parse/parse.h"
#include "solve.h"

int	main() {
	t_farm	*farm = parse_farm_from_stdin();

	solve(farm);

	free_farm(farm);
}