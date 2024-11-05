
#include "minishell.h"

t_ast_node	*new_ast_node(int type, char *command, char **args)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = command;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
	node->redirs = NULL;
	return (node);
}

t_redirection	*new_redirection(int type ,char *target)
{
	t_redirection *redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = target;
	redir->next = NULL;
	return (redir);
}

void	add_redirection(t_ast_node *node, int type, char *target)
{
	t_redirection	*redir;
	t_redirection	*temp;

	temp = NULL;
	redir = new_redirection(type, target);
	if (!redir)
		return ;
	if (!node->redirs)
		node->redirs = redir;
	else
	{
		temp = node->redirs;
		while (temp->next)
			temp = temp->next;
		temp->next = redir;
	}
}

int	get_redir_type(char *token)
{
	if (!strcmp(token, ">"))
		return (REDIR_OUT);
	else if (!strcmp(token, ">>"))
		return (REDIR_APPEND);
	else if (!strcmp(token, "<"))
		return (REDIR_IN);
	else if (!strcmp(token, "<<"))
		return (HEREDOC);
	return (-1);
}

/* static void	fill_redir_node(int redir_index, t_ast_node	*node, char **tokens)
{
	if (tokens[redir_index + 1] == NULL)
	{
		perror("Syntax error: missing file or delimiter for redirection");
	}
	if (get_redir_type(tokens[redir_index]) == HEREDOC)
		node->delimiter = tokens[redir_index + 1];
	else
		node->file = tokens[redir_index + 1];
	node->right = NULL;
} */

char **filter(char **tokens)
{
	int	i;
	int	num;
	int	count;
	char **final_args;

	i = 0;
	count = 0;
	while (tokens[i])
	{
		num = get_redir_type(tokens[i]);
		if (num != -1)
			i +=2;
		else
		{
			count++;
			i++;
		}
	}
	final_args = malloc((count + 1) * sizeof(char *));
	if (!final_args)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	count = 0;
	while (tokens[i])
	{
		num = get_redir_type(tokens[i]);
		if (num != -1)
			i +=2;
		else
		{
			final_args[count] = tokens[i];
			count++;
			i++;
		}
	}
	final_args[count] = NULL;
	return (final_args);
}

t_ast_node	*parse_tokens(char **tokens)
{
	int			pipe_index;
	t_ast_node	*node;
	int			cmd_index;
	int			i;
	int			redir_type;

	i = 0;
	node = NULL;
	cmd_index = -1;
	pipe_index = find_pipe(tokens);
	if (pipe_index != -1)
	{
		node = new_ast_node(PIPE, NULL, NULL);
		node->left = parse_tokens(slice_tokens(tokens, 0, pipe_index));
		node->right = parse_tokens(slice_tokens(tokens, pipe_index + 1, -1));
		return (node);
	}

	while (tokens[i] != NULL)
	{
    	redir_type = get_redir_type(tokens[i]);
    	if (redir_type == HEREDOC)
		{  
        	if (tokens[i + 1] == NULL)
            	perror("Syntax error: missing file for redirection");
        	if (!node)
				node = new_ast_node(CMD, NULL, NULL);
            add_redirection(node, HEREDOC, tokens[i + 1]);
            i += 2;
        }
		else
            i++;
	}
	i = 0;
	while (tokens[i] != NULL)
	{
    	redir_type = get_redir_type(tokens[i]);
    	if (redir_type != -1)
		{  
        	if (tokens[i + 1] == NULL)
            	perror("Syntax error: missing file for redirection");
			if (!node)
				node = new_ast_node(CMD, NULL, NULL);
            add_redirection(node, redir_type, tokens[i + 1]);
            i += 2;  
    	}
		else
			i++;
	}
	i = 0;
	while (tokens[i] != NULL)
	{
		redir_type = get_redir_type(tokens[i]);
		if (redir_type != -1)
			i += 2;
		else
		{
			cmd_index = i;
			break;
		}
	}

	if (cmd_index != -1)
	{
    	if (!node)
			node = new_ast_node(CMD, tokens[cmd_index], slice_tokens(tokens, cmd_index, -1));
		else
		{
			node->cmd = tokens[cmd_index];
			node->args = filter(slice_tokens(tokens, cmd_index, -1));
		} 
	}
	return node;
}


