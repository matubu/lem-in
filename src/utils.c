#include "utils.h"

void	ft_memcpy(void *dst, void *src, u64 n) {
	for (u64 i = 0; i < n; i++)
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
}

bool	less(u64 a, u64 b) {
	return a < b;
}

bool	greater(u64 a, u64 b) {
	return a > b;
}