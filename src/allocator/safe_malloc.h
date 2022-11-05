#pragma once

#undef malloc

#include <unistd.h>

#define PUTS_ERROR(s) {if(write(2, s "\n", sizeof(s))){}}

static inline void	*safe_malloc(size_t type_size, size_t n) {
	void	*ptr = malloc(type_size * n);
	if (ptr == NULL) {
		PUTS_ERROR("=========================================");
		PUTS_ERROR("\x1b[91mError\x1b[91m: Could not allocate");
		exit(1);
	}
	return (ptr);
}

#include "warn_unsafe_malloc.h"