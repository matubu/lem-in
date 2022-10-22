#pragma once

#include <stddef.h>
#include <stdlib.h>

#define bool int
#define true 1
#define false 0

#include "io.h"

static inline void	*_safe_malloc(size_t n) {
	void	*ptr = malloc(n); \
	EXPECT(ptr != NULL, "Could not allocate"); \
	return (ptr); \
}

#define malloc(n) _safe_malloc(n)
#define smalloc(type) _safe_malloc(sizeof(type))
#define tmalloc(type, n) _safe_malloc((n) * sizeof(type))

#define LOG(stamp) \
	dprintf(2, "\033[90m%10.2lfs \033[1;94m" stamp "\033[0m\n", (double)(clock()) / CLOCKS_PER_SEC)
