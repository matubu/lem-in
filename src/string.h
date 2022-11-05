#pragma once

#include </usr/include/string.h>

#include "types.h"
#include "allocator/safe_malloc.h"
#include "utils.h"

static inline u64	len(char *s) {
	u64	len = 0;

	if (s) {
		while (s[len])
			++len;
	}
	return (len);
}

static inline bool	contains(char *s, char c) {
	while (*s) {
		if (*s++ == c)
			return (true);
	}
	return (false);
}

static inline int	u64_string_size(int n) {
	int len = 0;

	do {
		++len;
		n /= 10;
	} while (n);

	return (len);
}

void	ft_strjoin(char **s1, char *s2);

static inline bool	is_number(char c) {
	return (c >= '0' && c <= '9');
}
static inline bool	is_alpha(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
static inline bool	is_whitespace(char c) {
	return (c == ' ' || c == '\t');
}
static inline bool	is_alphanum(char c) {
	return (is_number(c) || is_alpha(c));
}
static inline bool	is_var(char c) {
	return (is_alphanum(c) || c == '_');
}

bool	greater_str(char *s1, char *s2);
bool	less_str(char *s1, char *s2);
bool	equal_str(char *s1, char *s2);