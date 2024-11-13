
#include "../../inc/minishell.h"


extern char **environ;

void	ft_free(char *list[])
{
	int	i;

	if (!*list)
		return;
	i = 0;
	while (list[i] != NULL)
	{
		free(list[i]);
		i++;
	}
	free(list);
}

char	*get_env()
{
	int	i = 0;

	while (environ[i])
	{
		// Find the PATH environment variable
		if (ft_strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);  // Return everything after "PATH="
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd)
{
	int		i = 0;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	*env_path;

	if (!cmd)
        return NULL;
	env_path = get_env();
	if (!env_path)
		return (cmd);
	allpath = ft_split(env_path, ':');
	if (!allpath)
		return (cmd);
	while (allpath[i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)  // Check if the executable exists and is executable
		{
			ft_free(allpath);
			return (exec);  // Found the executable
		}
		free(exec);
		i++;
	}
	ft_free(allpath);
	return (cmd);  // Return cmd if not found in PATH
}
