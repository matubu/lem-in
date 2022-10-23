#include <stdlib.h>

extern const int malloc_deprecated __attribute__((deprecated("use safe_malloc instead")));

#define malloc(n) ((void)malloc_deprecated, malloc)
