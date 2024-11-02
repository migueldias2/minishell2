#include "minishell.h"

/* volatile sig_atomic_t interrupted = 0; */
extern char **environ;

/* void	handle_sigint(int signal)
{
	(void)signal;
	interrupted = 1;
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
} */

int main(int argc, char *argv[], char *env[])
{
	environ = env;

	(void)argc, (void)argv;
	/* char	**env_copy; */
	char	*input = NULL;
	/* char	**tokens; */
	char	**new_tokens = NULL;
	char	**exp_tokens = NULL;
	t_ast_node *ast_root = NULL;
	int		i = 0;

	/* env_copy = copy_env_vars(env); */
	/* signal(SIGINT, handle_sigint); */
	while (1)
	{
		/* interrupted = 0;
		if (interrupted)
		{
			free(input);
			continue;
		} */
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (!*input)
		{
			free(input);
			continue;
		}
        if (*input)
        {
            add_history(input);
            if (strcmp(input, "exit") == 0) //ir buscar ao libft o ft_strcmp
            {
                free(input);
                printf("exit\n");
                break;
            }
        }
        if (check_for_unclosed_quotes(input) == -1)
        {
            perror("Error unclosed quotes"); // put to stderr, nao esquecer
            free(input);
            continue ;
        }
		if (semicolon_checker(input) == -1)
		{
			perror("Error semicolon not supported");
			free(input);
            continue ;
		}
		i = 0;

		new_tokens = tokenize(input);
		if (!new_tokens)
		{
    		free(input);
    		perror("Error in tokenize");
    		continue;
		}
		/* while (new_tokens[i] != NULL)
        {
			printf("token[%d]: %s \n",i ,new_tokens[i]);
			i++;
		} */
		/* printf("\n"); */
        i = 0;
        exp_tokens = expand_vars(new_tokens, environ);
		if (!exp_tokens)
		{
			free_all(new_tokens, input);
			perror("Error in expand_vars");
			continue;
		}
        /* while (exp_tokens[i] != NULL)
        {
			printf("token[%d]: %s \n",i ,exp_tokens[i]);
			i++;
		} */
		//build the AST
		ast_root = parse_tokens(exp_tokens);
		execute_ast(ast_root);
		//free_ast(ast_root);
        free_all(exp_tokens, input);
    }
    /* i = 0;
    while (env_copy[i])
    {
        free(env_copy[i++]);
    }
    free(env_copy); */
    return (0);
}

