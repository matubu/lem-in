#pragma once

#include <string.h>

static inline size_t	len(char *s) {
	size_t	len = 0;

	while (s[len])
		++len;
	return (len);
}

static inline bool	contains(char *s, char c) {
	while (*s) {
		if (*s++ == c)
			return (true);
	}
	return (false);
}

static inline bool	eq(char *a, char *b) {
	while (*a && *a == *b) {
		++a;
		++b;
	}
	return (*a == *b);
}

static inline int	int_string_size(int n) {
	int len = 0;

	do {
		++len;
		n /= 10;
	} while (n);

	return (len);
}

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