#pragma once

#include <fcntl.h>
#include <sys/stat.h>

#include "allocator/safe_malloc.h"
#include "types.h"
#include "io.h"
#include "utils.h"

char	*readall(int fd);
char	**readall_lines(int fd);
void	free_lines(char **lines);