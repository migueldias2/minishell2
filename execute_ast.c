
#include "minishell.h"

extern char **environ;

/* void	execute_pipe(t_ast_node *node)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("Error pipe"));
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
} */

/* void	execute_redir(t_ast_node *node)
{
	int	fd;

	if (node->type == REDIR_IN)
	{
		fd = open(node->file, O_RDONLY);
		if (fd == -1)
			return (perror("failed to open"));
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (node->type == REDIR_OUT)
	{
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
			return (perror("failed to open"));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (node->type == REDIR_APPEND)
	{
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fd == -1)
			perror("failed to open");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (node->type == HEREDOC)
		here_doc(node->delimiter);
	execute_ast(node->left);
} */

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


void	execute_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
	{
		int	pipe_fd[2];

		if (pipe(pipe_fd) == -1)
			return (perror("Error pipe"));
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
		return ;
	}

	int saved_stdin, saved_stdout, saved_stderr;
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    saved_stderr = dup(STDERR_FILENO);
    if (node->type >= REDIR_IN && node->type <= REDIR_OUT)
    {
        // Save original file descriptors

        int fd;

        if (node->type == REDIR_IN)
        {
            fd = open(node->file, O_RDONLY);
            if (fd == -1)
                return (perror("failed to open"));
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (node->type == REDIR_OUT)
        {
            fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
                return (perror("failed to open"));
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (node->type == REDIR_APPEND)
        {
            fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                return (perror("failed to open"));
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (node->type == HEREDOC)
        {
			here_doc(node->delimiter);
        }

        execute_ast(node->left);

        // Restore original file descriptors
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stderr, STDERR_FILENO);
        return;
    }
	if (node->type == CMD)
	{
		if (fork() == 0)
			exec(node);
		else
		{
			wait(NULL);
		}
	}
	close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);
}
