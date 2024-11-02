#include "minishell.h"

static void	free_and_exit(char **env_list, int i)
{
	while (i > 0)
		free(env_list[--i]);
	free(env_list);
	exit(0);
}

char	**copy_env_vars(char **env)
{
	char	**env_list;
	int		total_vars;
	int		i;
	int		j;

	total_vars = 0;
	i = 0;
	while (env[total_vars])
		total_vars++;
	env_list = malloc((total_vars + 1) * sizeof(char *));
	if (!env_list)
		return (NULL);
	while (env[i])
	{
		j = strlen(env[i]);
		env_list[i] = malloc((j + 1) * sizeof(char));
		if (!env_list)
			free_and_exit(env_list, i);
		strcpy(env_list[i], env[i]);
		i++;
	}
	env_list[i] = NULL;
	return (env_list);
}
