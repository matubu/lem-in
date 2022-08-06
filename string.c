#include "string.h"

int equal_str(char *s1, char *s2) {
	if (!s1 || !s2)
		return 0;
	int i = 0;
	while (s1[i] && s2[i])
		if (s1[i] != s2[i])
			return 0;
		else
			i++;
	return s1[i] == s2[i];
}

int less_str(char *s1, char *s2) {
	int i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] < s2[i]);
}

int greater_str(char *s1, char *s2) {
	return (!less_str(s1, s2) && !equal_str(s1, s2));
}