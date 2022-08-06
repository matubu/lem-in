#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char	*readfile(char *filename);
char	**readfile_lines(char *filename);
void	free_lines(char **lines);