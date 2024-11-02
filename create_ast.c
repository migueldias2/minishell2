
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
	node->file = NULL;
	node->delimiter = NULL;
	return (node);
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

static void	fill_redir_node(int redir_index, t_ast_node	*node, char **tokens)
{
	if (tokens[redir_index + 1] == NULL)
	{
		perror("Syntax error: missing file or delimiter for redirection");
		exit(EXIT_FAILURE);
	}
	if (get_redir_type(tokens[redir_index]) == HEREDOC)
		node->delimiter = tokens[redir_index + 1];
	else
		node->file = tokens[redir_index + 1];
	node->right = NULL;
}

t_ast_node	*parse_tokens(char **tokens)
{
	int			pipe_index;
	int			redir_index;
	t_ast_node	*node;

	pipe_index = find_pipe(tokens);
	if (pipe_index != -1)
	{
		node = new_ast_node(PIPE, NULL, NULL);
		node->left = parse_tokens(slice_tokens(tokens, 0, pipe_index));
		node->right = parse_tokens(slice_tokens(tokens, pipe_index + 1, -1));
		return (node);
	}
	redir_index = find_redirection(tokens);
	if (redir_index != -1)
	{

		/* se for < e comando a esquerda, ver se tem comando a esquerda, executar comando ignorando o <infile
		exemplo echo ola <outfile adeus printa ola adeus
		se for < e sem comando a esquerda, mas com comando a direita ex. <outfile cat redi
		reciona o input de stdin para outfile e faz cat */


		node = new_ast_node(get_redir_type(tokens[redir_index]), NULL, NULL);
		fill_redir_node(redir_index, node, tokens);
		node->left = parse_tokens(slice_tokens(tokens, 0, redir_index));
		return (node);
	}
	node = new_ast_node(CMD, tokens[0], slice_tokens(tokens, 0, -1));
	return (node);
}


