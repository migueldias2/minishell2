#include "minishell.h"

void free_redirections(t_redirection *redir)
{
	t_redirection *temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		free(temp);
	}
}

void free_ast(t_ast_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return;
	free_ast(node->left);
	free_ast(node->right);

	if (node->args)
	{
		while(node->args[i] != NULL)
		{
			free(node->args[i]);
			i++;
        }
        free(node->args);
    }
    free_redirections(node->redirs);
    free(node);
}
