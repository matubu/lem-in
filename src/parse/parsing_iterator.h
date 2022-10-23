#pragma once

#include <stdio.h>

#include "allocator/safe_malloc.h"
#include "readfile/readfile.h"
#include "string.h"

typedef struct {
	char	*filename;
	u64		idx;
	char	**ptr;
}	FileIterator;

/**
 * @note allocate memory (needs free)
 */
static inline FileIterator	create_file_iterator(char *filename) {
	char	**lines = readfile_lines(filename);

	return ((FileIterator){
		.filename = filename,
		.idx = 0,
		.ptr = lines
	});
}

static inline void	free_file_iterator(FileIterator *it) {
	free_lines(it->ptr);
	it->ptr = 0;
	it->idx = 0;
}


typedef struct {
	char		*filename;
	u64			line;
	u64			idx;
	char		*ptr;
}	LineIterator;


static inline void	parsing_error(LineIterator *it, char *s) {
	dprintf(2, "\033[1;91mError\033[0;90m(parsing)\033[0m");
	dprintf(2, "\033[1;94m ➔  \033[0m%s:%lu:%lu\n", it->filename, it->line + 1, it->idx);

	int	offset = u64_string_size(it->line + 1);

	dprintf(2, "\033[1;94m%lu |  \033[0m%s\033[90m⮰\033[0m\n", it->line + 1, it->ptr);
	dprintf(2, "%*s   %*s\033[1;91m↑ %s\033[0m\n", offset, "", (int)it->idx, "", s);
	exit(1);
}

#define P_EXPECT(condition, it, s) if (!(condition)) parsing_error(it, s)

#define is_it_end(it) !((it)->ptr[(it)->idx])

#define get(it) ((it)->ptr[(it)->idx])
#define get_next(it) ((it)->ptr[(it)->idx + 1])
#define get_ptr(it) ((it)->ptr + (it)->idx)

#define next(it) ((it)->ptr[(it)->idx++])
#define next_void(it) (it)->idx++

static inline LineIterator	next_line(FileIterator *it) {
	LineIterator	line_it = (LineIterator){
		.filename = it->filename,
		.line = it->idx,
		.idx = 0,
		.ptr = next(it)
	};
	P_EXPECT(line_it.ptr != NULL, &line_it, "unexpected end of file");
	return (line_it);
}

/**
 * @note allocate memory (needs free)
 */
static inline char	*next_word(LineIterator *it) {
	char	*s = get_ptr(it);
	int		len = 0;

	while (is_var(s[len])) {
		++len;
	}
	P_EXPECT(len, it, "Expected a variable");
	it->idx += len;
	char	*ptr = safe_malloc(sizeof(char), len + 1);
	ptr[len] = '\0';
	while (len--)
		ptr[len] = s[len];
	return (ptr);
}

static inline u64	next_u64(LineIterator *it) {
	u64	n = 0;

	P_EXPECT(is_number(get(it)), it, "Expected a positive number");
	while (is_number(get(it))) {
		n = n * 10 + (next(it) - '0');
	}

	return (n);
}

static inline void	skip_whitespace(LineIterator *it) {
	P_EXPECT(is_whitespace(next(it)), it, "Expected a whitespace");
	while (is_whitespace(get(it))) {
		next_void(it);
	}
}
