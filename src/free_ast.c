#include "../inc/minishell.h"

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
    if (!node)
        return;

    /* printf("Freeing left subtree of node: %p\n", (void *)node); */
    if (node->left)
        free_ast(node->left);

    /* printf("Freeing right subtree of node: %p\n", (void *)node); */
    if (node->right)
        free_ast(node->right);

    if (node->args)
    {
        /* printf("Freeing args of node: %p\n", (void *)node); */
        int i = 0;
        while (node->args[i] != NULL)
        {
            free(node->args[i]);
            i++;
        }
        free(node->args);
    }

    /* printf("Freeing redirections of node: %p\n", (void *)node); */
    free_redirections(node->redirs);

    /* printf("Freeing node itself: %p\n", (void *)node); */
    free(node);
}
