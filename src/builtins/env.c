
#include "../../inc/minishell.h"

void ft_env(t_mini *mini)
{
	int i = 0;
	while (mini->env[i])
		printf("%s\n", mini->env[i++]);
}

void replace_var_env(char **envp, char *to_found, char *to_replace)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], to_found, ft_strlen(to_found)) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(to_found, to_replace);
			break;
		}
		i++;
	}
}

void update_env(char *new_pwd, char *pwd,t_mini *mini)
{
	int i;

	i = 0;
	if (!pwd)
	{
		free(pwd);
		pwd = getcwd(NULL, 0);
	}
	if (!new_pwd)
		new_pwd = getcwd(NULL, 0);
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", new_pwd);
		i++;
		break;
	}
}

void update_env_abs(char *pwd, char *home, t_mini *mini)
{
	int i;

	i = 0;
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", home);
		i++;
		break;
	}
}

char *encontra_barra(char *s)
{
	char *new_pwd;
	int j;
	int i;

	new_pwd = NULL;
	i = 0;
	while (s[i])
		i++;
	while (i > 0)
	{
		if (s[i] == '/')
			break ;
		i--;
	}
	new_pwd = malloc(sizeof(char) * (i + 1));
	if (!new_pwd)
		return (NULL);
	j = 0;
	while (j != i)
	{
		new_pwd[j] = s[j];
		j++;
	}
	new_pwd[j] = '\0';
	return (new_pwd);
}

void update_env_back_cd(char *new_pwd, char*pwd, t_mini *mini)
{
	int i;

	i = 0;
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", new_pwd);
		i++;
		break;
	}
}

char *get_var_env(char **env, char *to_find)
{
	int i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], to_find, ft_strlen(to_find)) == 0)
			return (ft_strdup(ft_strchr(env[i], '/') ));
		i++;
	}
	return NULL;
}
