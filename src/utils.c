#include "lem_in.h"

void	ft_memcpy(void *dst, void *src, uint n) {
	for (uint i = 0; i < n; i++)
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
}

bool	less(uint a, uint b) {
	return a < b;
}

bool	greater(uint a, uint b) {
	return a > b;
}