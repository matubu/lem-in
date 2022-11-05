#include "readall.h"

#define READALL_SIZE 4096

char	*readall(int fd) {
	char	*buf = malloc(1);
	size_t	len = 0;

	while (1) {
		char *new_buf = safe_malloc(sizeof(char), len + READALL_SIZE + 1);
		ft_memcpy(new_buf, buf, len);
		free(buf);
		buf = new_buf;
		int ret = read(fd, buf + len, READALL_SIZE);
		if (ret <= 0) {
			break ;
		}
		len += ret;
	}

	buf[len] = '\0';
	return (buf);
}

char	**readall_lines(int fd) {
	char	*s = readall(fd);

	u64	lines_count = 1;
	for (u64 i = 0; s[i]; ++i) {
		if (s[i] == '\n')
			++lines_count;
	}

	char	**lines = safe_malloc(sizeof(char *), lines_count + 1);

	u64	idx = 0;
	u64	i = 0;

	while (1) {
		// test\ntest\0
		// ^     ^  start of strings

		u64	end = i;
		while (s[end] && s[end] != '\n') {
			++end;
		}
		// test\ntest\0
		//     ^     ^  end of strings

		lines[idx++] = s + i;

		if (s[end] == '\0') {
			break ;
		}

		// test\ntest\0
		//     ^
		//     \0  replace the newlines with \0
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