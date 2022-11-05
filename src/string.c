#include "string.h"

bool	equal_str(char *s1, char *s2) {
	if (!s1 || !s2)
		return 0;
	int	i = 0;
	while (s1[i] && s2[i])
		if (s1[i] != s2[i])
			return 0;
		else
			i++;
	return s1[i] == s2[i];
}

bool	less_str(char *s1, char *s2) {
	int	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] < s2[i]);
}

bool	greater_str(char *s1, char *s2) {
	return (!less_str(s1, s2) && !equal_str(s1, s2));
}

void	ft_strjoin(char **s1, char *s2) {
	u64	len1 = len(*s1);
	u64	len2 = len(s2);
	char	*new = safe_malloc(sizeof(char), len1 + len2 + 1);
	ft_memcpy(new, *s1, len1);
	ft_memcpy(new + len1, s2, len2);
	new[len1 + len2] = '\0';
	free(*s1);
	*s1 = new;
}