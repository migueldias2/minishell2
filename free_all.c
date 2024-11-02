#include "minishell.h"

void free_all(char **tokens, char *input)
{
	int i;

	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	free(input);
}
