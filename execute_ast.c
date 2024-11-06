
#include "minishell.h"

extern char **environ;

void	exec(t_ast_node *node)
{
	char	*path;

	path = get_path(node->cmd); // fazer o get path com environment global variable
	if (!path)
	{
		perror("Command not found");
		exit(EXIT_FAILURE);
	}
	execve(path, node->args, environ);
	perror("execve error");
	/* free(path); */
	/* exit(EXIT_FAILURE); */
}

void	handle_redirections(t_ast_node *node)
{
	t_redirection *redir;
	int fd;

	redir = node->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			fd = open(redir->target, O_RDONLY);
			if (fd == -1)
				perror("failed to open input file");
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				perror("failed to open output file");
			dup2(fd, STDOUT_FILENO);
			close(fd);
			if (fork() == 0)
				exec(node);
			else
				wait(NULL);
		}
		else if (redir->type == REDIR_APPEND)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				perror("failed to open output file for append");
			dup2(fd, STDOUT_FILENO);
			close(fd);
			if (fork() == 0)
				exec(node);
			else
				wait(NULL);
		}
		else if (redir->type == HEREDOC)
		{
			fd = here_doc(redir->target);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
}

int	noredirs_orheredoc_singlestdin(t_ast_node *node)
{
	t_redirection *temp;
	int	count_redirs;
	int	count_inredir;

	temp = node->redirs;
	count_redirs = 0;
	count_inredir = 0;
	if (!temp)
		return (0);
	while (temp)
	{
		if (temp->type == HEREDOC)
			return (0);
		else if (temp->type == REDIR_IN)
			count_inredir++;
		else
			count_redirs++;
		temp = temp->next;
	}
	if (count_inredir && !count_redirs)
		return (0);
	return (1);
}

void	execute_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
	{
		int	pipe_fd[2];

		if (pipe(pipe_fd) == -1)
		{
			perror("Error pipe");
			return;
		}

		if (fork() == 0)
		{
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			execute_ast(node->left);
			exit(EXIT_FAILURE);
		}
		else
		{
			if (fork() == 0)
			{
				dup2(pipe_fd[0], STDIN_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
				execute_ast(node->right);
				exit(EXIT_FAILURE);
			}
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		wait(NULL);
		wait(NULL);
		return;
	}
	if (node->type == CMD)
	{
		if (fork() == 0)
		{
			handle_redirections(node);
			if (!noredirs_orheredoc_singlestdin(node))
				exec(node);
			else
				exit(EXIT_FAILURE);
		}
		else
			wait(NULL);
	}
}
