
#include "../libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*new;

	if (!s || !*s)
		return (NULL);
	new = malloc(sizeof(char) * (n + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
