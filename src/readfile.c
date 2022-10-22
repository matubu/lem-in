#include "io.h"
#include "readfile.h"

char	*readfile(char *filename) {
	int	fd = open(filename, O_RDONLY);
	EXPECT_ERRNO(fd != -1, "Could not open file");

	struct stat	stat;
	EXPECT_ERRNO(fstat(fd, &stat) != -1, "Could not stat file");

	char	*s = tmalloc(char, stat.st_size + 1);

	int len = read(fd, s, stat.st_size);
	EXPECT_ERRNO(len != -1, "Could not read");

	s[len] = '\0';

	return (s);
}

char	**readfile_lines(char *filename) {
	char	*s = readfile(filename);

	size_t	lines_count = 1;
	for (size_t i = 0; s[i]; ++i) {
		if (s[i] == '\n')
			++lines_count;
	}

	char	**lines = tmalloc(char *, lines_count + 1);

	size_t	idx = 0;
	size_t	i = 0;
	while (1) {
		// test\ntest\0
		// ^	 ^  start of strings

		size_t	end = i;
		while (s[end] && s[end] != '\n') {
			++end;
		}
		// test\ntest\0
		//	 ^	 ^  end of strings

		lines[idx++] = s + i;

		if (s[end] == '\0')
			break ;

		// test\ntest\0
		//	 ^
		//	 \0  replace the newlines with \0
		s[end] = '\0';
		i = end + 1;
	}
	lines[idx] = NULL;

	return (lines);
}

void	free_lines(char **lines) {
	if (lines == NULL) {
		return ;
	}
	free(*lines);
	free(lines);
}