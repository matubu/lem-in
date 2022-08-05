#pragma once

#include <stdio.h>

#include "readfile.h"
#include "string.h"

typedef struct {
	char	*filename;
	size_t	idx;
	char	**ptr;
}	FileIterator;

// Warning: allocate memory (needs free)
FileIterator	create_file_iterator(char *filename) {
	char	**lines = readfile_lines(filename);

	return ((FileIterator){
		.filename = filename,
		.idx = 0,
		.ptr = lines
	});
}

void	free_file_iterator(FileIterator *it) {
	free_lines(it->ptr);
	it->ptr = 0;
	it->idx = 0;
}


typedef struct {
	char		*filename;
	size_t		line;
	size_t		idx;
	char		*ptr;
}	LineIterator;


static inline void	parsing_error(LineIterator *it, char *s) {
	printf("\033[1;91mError[parsing]\033[0m:\n");

	int	offset = int_string_size(it->line + 1);

	printf("\033[1;94m%*s--> \033[0m%s:%lu:%lu\n", offset, "", it->filename, it->line + 1, it->idx + 1);
	printf("\033[1;94m%lu |  \033[0m%s\n", it->line + 1, it->ptr);
	printf("%*s    %*s\033[1;91m^ %s\033[0m\n", offset, "", (int)it->idx, "", s);
	exit(1);
}

#define P_EXPECT(condition, it, s) if (!(condition)) parsing_error(it, s)


// Does not advance the pointer
#define	get(it) ((it)->ptr[(it)->idx])
#define get_next(it) ((it)->ptr[(it)->idx + 1])
#define	get_ptr(it) ((it)->ptr + (it)->idx)

// Get the next element advancing the pointer
#define next(it) ((it)->ptr[(it)->idx++])
LineIterator	next_line(FileIterator *it) {
	return ((LineIterator){
		.filename = it->filename,
		.line = it->idx,
		.idx = 0,
		.ptr = next(it)
	});
}
// Warning: allocate memory (needs free)
static inline char	*next_word(LineIterator *it) {
	char	*s = get_ptr(it);
	int		len = 0;

	while (is_var(s[len])) {
		++len;
	}
	it->idx += len;
	char *ptr = tmalloc(char, len + 1);
	while (len--)
		ptr[len] = s[len];
	return (ptr);
}
size_t	next_sizet(LineIterator *it) {
	size_t	n = 0;

	P_EXPECT(is_number(get(it)), it, "Expected a number");
	while (is_number(get(it))) {
		n = n * 10 + (next(it) - '0');
	}

	return (n);
}
static inline void	skip_whitespace(LineIterator *it) {
	P_EXPECT(is_whitespace(get(it)), it, "Expected a whitespace");
	while (is_whitespace(get(it))) {
		next(it);
	}
}


#define	is_it_end(it) !((it)->ptr[(it)->idx])
