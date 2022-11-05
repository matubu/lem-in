#pragma once

#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdlib.h>

#include "string.h"

#define FD_PUT(fd, s) {if(write(fd, s, sizeof(s) - 1)){}}

static inline void	fd_put(int fd, char *s) {
	if(write(fd, s, len(s))){}	
}

static inline void fd_put_u64(int fd, u64 n) {
	char	buf[64];
	int		i = 64;

	do {
		buf[--i] = n % 10 + '0';
		n /= 10;
	} while (n);
	if(write(fd, buf + i, 64 - i)){}
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