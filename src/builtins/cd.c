
#include "../../inc/minishell.h"

void ft_cd(t_ast_node *node ,t_mini *mini)
{
	char *home;
	char *pwd;
	char *new_pwd;

	new_pwd = NULL;
	home = get_var_env(mini->env, "HOME=");
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		free(home);
		free(pwd);
		printf("Error: getcwd failed\n");
		return ;
	}
	if (!node->cmd[1])
	{
		if (chdir(home) == -1)
		{
			free(home);
			free(pwd);
			printf("Error: chdir failed\n");
			exit(0);
		}
		update_env_abs(pwd, home, mini);
	//	free(mini->args[0]);
	}
	else if (ft_strncmp(node->args[1], "..", 2) == 0)
	{
		new_pwd = encontra_barra(pwd);
		if (chdir(new_pwd) == -1)
		{
			write(2, "Error: chdir failed1\n", 21);
			//if (mini->args)
			//	free_args(mini->args);
			free(pwd);
			free(home);
			return;
		}
		update_env_back_cd(new_pwd, pwd, mini);
		free(new_pwd);

	}
	else if (node->args[1])
	{
		if (node->args[1][0] == '/')
			new_pwd = ft_strjoin(pwd, node->args[1]);
		else
			new_pwd = ft_append_str(pwd, "/",node->args[1]);
		printf("%s\n", new_pwd);
		if (chdir(new_pwd) == -1)
		{
			write(2, "Error: chdir failed2\n", 21);
			//if (mini->args)
			//	free_args(mini->args);
			free(new_pwd);
			free(pwd);
			free(home);
			return;
		}
		update_env(new_pwd, pwd, mini);
		free(new_pwd);
	}
	free(pwd);
	free(home);
}
