
#include "../../inc/minishell.h"

int	find_pipe(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (strcmp(tokens[i],"|") == 0)
			return (i);
		i++;
	}
	return (-1);
}

/* int	find_redirection(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if ((strcmp(tokens[i], ">") == 0) || \
			(strcmp(tokens[i], ">>") == 0) || \
			(strcmp(tokens[i], "<") == 0) || \
			(strcmp(tokens[i], "<<") == 0))
			return (i);
		i++;
	}
	return (-1);
} */
