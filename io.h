#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>
#include "string.h"

#define FD_PUT(fd, s) write(fd, s, sizeof(s) - 1)

static inline void	fd_put(int fd, char *s) {
	write(fd, s, len(s));	
}

static inline void	die_errno(char *s) {
	FD_PUT(2, "\033[1;91mError\033[0m: ");
	fd_put(2, s);
	FD_PUT(2, ": ");
	fd_put(2, strerror(errno));
	FD_PUT(2, "\n");
	exit(1);
}

static inline void	die(char *s) {
	FD_PUT(2, "\033[1;91mError\033[0m: ");
	fd_put(2, s);
	FD_PUT(2, "\n");
	exit(1);
}

#define EXPECT(condition, s) if (!(condition)) { die(s); }
#define EXPECT_ERRNO(condition, s) if (!(condition)) { die_errno(s); }