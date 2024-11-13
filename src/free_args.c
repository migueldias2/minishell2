#include "../inc/minishell.h"

void free_all(char **tokens, char *input)
{
	int i;

	i = 0;
	if (tokens)
	{
		while (tokens[i])
		{
			free(tokens[i]);
			i++;
		}
		free(tokens);
	}
	if (input)
		free(input);
}

void free_args(char **args)
{
	int i = 0;
	if (!args)
		return;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

void free_2_all(char **args1, char **args2)
{
	free_args(args1);
	free_args(args2);
}
