
#include "minishell.h"

static int	fill_sliced(char **sliced, int *count, char **tokens, int *i)
{
	int	j;

	sliced[*count] = strdup(tokens[*i]);
	if (!sliced[*count])
	{
		j = 0;
		while (j < *count)
		{
			free(sliced[j]);
			j++;
		}
		free(sliced);
		return (0);
	}
	(*count)++;
	(*i)++;
	return (1);
}

char	**slice_tokens(char **tokens, int i, int end)
{
	int count;
	char **sliced;

	count = i;
	if (end == -1)
	{
		while (tokens[count] != NULL)
			count++;
		end = count;
	}
	sliced = malloc((end - i + 1) * sizeof(char *));
	if (!sliced)
		return NULL;
	count = 0;
	while (i < end)
	{
		if (!fill_sliced(sliced, &count, tokens, &i))
			return (NULL);
	}
	sliced[count] = NULL;
	return sliced;
}
