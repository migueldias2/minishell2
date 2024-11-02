
#include "minishell.h"

extern char **environ;

void	ft_free(char *list[])
{
	int	i;

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
		if (strncmp(environ[i], "PATH=", 5) == 0)
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

	// Fetch the PATH environment variable
	env_path = get_env();
	if (!env_path)
		return (cmd);

	// Split the PATH by ':' to get each directory
	allpath = ft_split(env_path, ':');
	if (!allpath)
		return (cmd);

	// Loop through each path in PATH to find the executable
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
