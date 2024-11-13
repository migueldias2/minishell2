
#include "../inc/minishell.h"

char **envp;

char	*path_find(char **envp, char *cmd)
{
	int		i;
	char	**possible_ways;
	char	*path;

	if (!envp || !cmd)
		return (NULL);
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	possible_ways = ft_split(envp[i] + 19, ':');
	i = 0;
	while (possible_ways[i])
	{
		path = ft_append_str(possible_ways[i], "/", cmd);
		if (access(path, F_OK) == 0)
			break ;
		else
		{
			free(path);
			path = NULL;
			i++;
		}
	}
	free_args(possible_ways);
	return (path);
}


void check_line(char *line)
{
	int i = 0;
	while (line[i])
	{
		if (line[i] == ';')
		{
			printf("Error: ';' is not allowed\n");
			exit(0);
		}
		i++;
	}
}

void ft_free_env(char **env)
{
	int i = 0;
	if (!env)
		return;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void ft_exit(t_mini *mini)
{
	clear_history();
	ft_free_env(mini->env);
	free_args(mini->args);
	free(mini->line);
	free(mini);
	exit(0);
}

void ft_pwd()
{
	char *pwd;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		printf("Error: getcwd failed\n");
		return ;
	}
	printf("%s\n", pwd);
	free(pwd);
}

void ft_export(t_mini *mini)
{
	int i = 0;
	while (mini->env[i] != NULL)
		i++;
	mini->env[i] = ft_strjoin("NEW_VAR=", mini->args[1]);
}



void ft_unset(t_mini *mini)
{
	int i;
	int j;
	char **new_env;

	i = 0;
	j = 0;
	while (mini->env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (mini->env[i])
	{
		if (ft_strncmp(mini->env[i], mini->args[1], ft_strlen(mini->args[1])) != 0)
		{
			new_env[j] = mini->env[i];
			j++;
		}
		else
		{
			free(mini->env[i]);
		}
		i++;
	}
	new_env[j] = NULL;
	free(mini->env);
	mini->env = new_env;
}

void ft_echo(t_mini *mini)
{
	int i = 1;
	while (mini->args[i])
	{
		printf("%s", mini->args[i]);
		i++;
		if (mini->args[i])
			printf(" ");
	}
	printf("\n");
}

void ft_exec(t_mini *mini)
{
	char	**cmd;
	char	*path;

	cmd = NULL;
	path = NULL;

	path = path_find(mini->env, mini->args[0]);
	if (!path)
	{
		free_all(cmd, path);
		write(2, "Command not found\n", 19);
		exit(127);
	}
	if (execve(path, cmd, mini->env) == -1)
		free_all(cmd, path);
}

void ft_execute(t_mini *mini)
{
	int pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		printf("pid error!\n");
		ft_exit(mini);
	}
	if (pid == 0)
		ft_exec(mini);
	else
	waitpid(pid, &status, 0);
}

void init_envp(t_mini *mini, char **envp)
{
	int i;

	i = 0;
	while (envp[i])
		i++;
	mini->env = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		mini->env[i] = ft_strdup(envp[i]);
		i++;
	}
	mini->env[i] = NULL;
}

void init_myown_envp(t_mini *mini)
{
	int i;
	char *pwd;

	i = 0;
	mini->env = (char **)malloc(sizeof(char *) * 3);
	mini->shlvl = 1;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		printf("Error: getcwd failed\n");
		return ;
	}
	mini->env[0] = ft_strjoin("PWD=", pwd);
	mini->env[1] = ft_strjoin("SHLVL=", ft_itoa(mini->shlvl));
	mini->env[2] = NULL;
	free(pwd);
}

void handling_signals(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
	}
}

// int main(int ac, char **av, char **envp)
// {
//     t_mini *main_mini;
//     char *line;
//     struct sigaction sa;

//     line = NULL;
//     ft_bzero(&sa, sizeof(sa));
//     sa.sa_flags = SA_RESTART;
//     sa.sa_sigaction = &handling_signals;
//    	sigaction(SIGINT, &sa, NULL);
//     sigaction(SIGTSTP, &sa, NULL);
//     (void)av, (void)ac;

//     signal(SIGQUIT, SIG_IGN);
// 	main_mini = (t_mini *)malloc(sizeof(t_mini));
//     if (!envp)
//     	init_myown_envp(main_mini);
//     else
//     	init_envp(main_mini, envp);
//     while (1)
//     {
//         line = readline("minishell> ");
//         if (!line)
//         {
//            	printf("Exiting of shell!\n");
//            	break ;
//         }
//         if (*line)
//         {
//             main_mini->line = ft_strdup(line);
//             if (main_mini->line && *main_mini->line != '\n')
//             {
//                 add_history(main_mini->line);
//                 choose_args(main_mini);
//                 free(main_mini->line);
//             }
//         }
//         free(line);
//     }
//     free(main_mini);
//     return 0;
// }


bool	if_stats_input(t_mini *mini)
{	//false para break //true para continue
	if (!mini->line)
	{
		printf("exit\n");
		ft_exit(mini);
		return (false);
	}
	if (*mini->line)
	{
		add_history(mini->line);
		if (ft_strcmp(mini->line, "exit") == 0)
		{
			free(mini->line);
			printf("exit\n");
			return (false);
		}
	}
	return (true);
}

void	init_mini(t_mini *mini)
{
	mini->ac = 0;
	mini->new_tokens = NULL;
	mini->exp_tokens = NULL;
	mini->env = NULL;
	mini->line = NULL;
	mini->args = NULL;
	mini->shlvl = 0;
}

/* void print_redirections(t_redirection *redir)
{
    while (redir != NULL)
    {
        // Print the redirection type and target file
        if (redir->type == REDIR_IN)
            printf("Redirection: INPUT from %s\n", redir->target);
        else if (redir->type == REDIR_OUT)
            printf("Redirection: OUTPUT to %s\n", redir->target);
        else if (redir->type == REDIR_APPEND)
            printf("Redirection: APPEND to %s\n", redir->target);
        else if (redir->type == HEREDOC)
            printf("Redirection: HEREDOC to %s\n", redir->target);

        // Move to the next redirection
        redir = redir->next;
    }
}

// Function to print an individual AST node
void print_ast_node(t_ast_node *node)
{
    if (node == NULL)
        return;

    // Print the current node type
    if (node->type == PIPE)
    {
        printf("PIPE\n");
    }
    else if (node->type == CMD)
    {
        printf("CMD: %s\n", node->cmd);
        if (node->args != NULL)
        {
            printf("Arguments: ");
            for (int i = 0; node->args[i] != NULL; i++)
            {
                printf("%s ", node->args[i]);
            }
            printf("\n");
        }
    }

    // Print redirection information if present
    if (node->redirs != NULL)
    {
        print_redirections(node->redirs);
    }

    // Recursively print left and right children if they exist
    if (node->left != NULL)
    {
        printf("Left child of %s:\n", node->cmd);
        print_ast_node(node->left);
    }
    if (node->right != NULL)
    {
        printf("Right child of %s:\n", node->cmd);
        print_ast_node(node->right);
    }
}

// Wrapper function to start the printing from the root of the AST
void print_ast(t_ast_node *root)
{
    print_ast_node(root);
} */
void free_token_list(t_token_node *head) {
    t_token_node *current = head;
    t_token_node *next;

    while (current != NULL) {
        next = current->next;  // Save the next node

        // Free the tokens array
        for (int i = 0; current->tokens[i] != NULL; i++) {
            free(current->tokens[i]);
			current->tokens[i] = NULL;  // Free each string in the tokens array
        }
        free(current->tokens);
		current->tokens = NULL;  // Finally, free the array of tokens itself

        free(current);
		current = NULL;  // Free the current node
        current = next;  // Move to the next node
    }
}

void	shell_looping(t_mini *mini, t_ast_node *ast_root)
{
	t_token_node *sliced_tokens_list;
	(void)ast_root;
	while (1)
	{
		mini->line = readline("minishell> ");
		if (!if_stats_input(mini))
			break;
		if (!checker_quotes(mini->line))
			continue;
		mini->new_tokens = tokenize(mini->line);
		if (!mini->new_tokens)
		{
    		free(mini->line);
    		write(2, "Error in tokenize", 18);
    		continue;
		}
		mini->exp_tokens = expand_vars(mini->new_tokens, envp);
		if (!mini->exp_tokens)
		{
			free_all(mini->new_tokens, mini->line);
			continue;
		}
		sliced_tokens_list = NULL;
		ast_root = parse_tokens(mini->exp_tokens, &sliced_tokens_list);
		/* print_ast(ast_root); */

		execute_ast(ast_root, mini);
		free_token_list(sliced_tokens_list);
		free_ast(ast_root);
		free_all(mini->exp_tokens, mini->line);
	}
}


int main(int ac, char **av, char **env)
{
	t_ast_node *ast_root;
	t_mini		*mini;

	envp = env;
	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (0);
	(void)ac;
	(void)av;
//ft_bzero(mini, (size_t)sizeof(mini));
	init_mini(mini);
	ast_root = NULL;
	if (!env)
   		init_myown_envp(mini);
    else
   		init_envp(mini, envp);
	shell_looping(mini, ast_root);
	ft_close_all_fds(mini);
	free_ast(ast_root);
	free(mini);
	return (0);
}
