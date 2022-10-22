#include "lem_in.h"

void	ft_memcpy(void *dst, void *src, size_t n) {
	for (size_t i = 0; i < n; i++)
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
}
