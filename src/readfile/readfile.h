#pragma once

#include <fcntl.h>
#include <sys/stat.h>

#include "allocator/safe_malloc.h"
#include "types.h"
#include "io.h"

char	*readfile(char *filename);
char	**readfile_lines(char *filename);
void	free_lines(char **lines);