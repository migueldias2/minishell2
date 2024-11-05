
#include "minishell.h"
#define SAFE_FD_LIMIT 256

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


/* void	execute_ast(t_ast_node *node)
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
    if (node->type >= REDIR_IN && node->type <= HEREDOC)
	{
    	int fd;

    	if (fork() == 0)
		{ // Fork and perform redirection in the child
        	if (node->type == REDIR_IN)
			{
            	fd = open(node->file, O_RDONLY);
            	if (fd == -1)
				{
                	perror("failed to open input file");
                	exit(EXIT_FAILURE);
            	}
            	dup2(fd, STDIN_FILENO);
            	close(fd);
        	}
			else if (node->type == REDIR_OUT)
			{
            	fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            	if (fd == -1)
				{
                	perror("failed to open output file");
                	exit(EXIT_FAILURE);
            	}
            	dup2(fd, STDOUT_FILENO);
            	close(fd);
        	}
			else if (node->type == REDIR_APPEND)
			{
            	fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            	if (fd == -1)
				{
                	perror("failed to open output file for append");
                	exit(EXIT_FAILURE);
            	}
            	dup2(fd, STDOUT_FILENO);
            	close(fd);
        	}
			else if (node->type == HEREDOC)
			{
            	here_doc(node->delimiter);  // Implement this function to handle HEREDOC
            	// No need to set up `dup2` for HEREDOC if handled internally
        	}

        	// Now execute the command
        	exec(node->left);  // Assuming `node->left` is a command node
        	perror("exec error");
        	exit(EXIT_FAILURE);
    	} 
		else
		{
        	wait(NULL);
    	}
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
} */

void handle_redirections(t_ast_node *node)
{
    t_redirection *redir = node->redirs;
    int output_fds[10];  // Adjust size based on max expected redirects
    int output_count = 0;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("Failed to create pipe");
        exit(EXIT_FAILURE);
    }

    while (redir)
    {
        if (redir->type == REDIR_IN)
        {
            int fd = open(redir->target, O_RDONLY);
            if (fd == -1)
            {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
        {
            int open_flags;
			if (redir->type == REDIR_OUT)
    			open_flags = O_WRONLY | O_CREAT | O_TRUNC; // Add O_TRUNC if it's a standard output redirection
			else if (redir->type == REDIR_APPEND)
    			open_flags = O_WRONLY | O_CREAT | O_APPEND; // Add O_APPEND if it's an append redirection
            int fd = open(redir->target, open_flags, 0644);
            if (fd == -1)
            {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            output_fds[output_count++] = fd;
        }
        else if (redir->type == HEREDOC)
        {
            int fd = here_doc(redir->target);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        redir = redir->next;
    }

	if (node->redirs != NULL)
	{
    	if (fork() == 0)
    	{
        	// Redirect the command output to the write end of the pipe
        	close(pipe_fd[0]);
        	dup2(pipe_fd[1], STDOUT_FILENO);
        	close(pipe_fd[1]);
        	exec(node);
        	exit(EXIT_FAILURE);
    	}
    	else
    	{
        	close(pipe_fd[1]); // Close write end of the pipe in parent

        	// Read the captured output from the command
        	char buffer[1024];
        	ssize_t bytes_read;
        	while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
        	{
            	// Write the captured output to each output redirection
            	for (int i = 0; i < output_count; i++)
            	{
                	write(output_fds[i], buffer, bytes_read);
            	}
        	}
        	close(pipe_fd[0]);

        	// Close all output file descriptors
        	for (int i = 0; i < output_count; i++)
        	{
            	close(output_fds[i]);
        	}

        	wait(NULL); // Wait for child process to finish
    	}
	}
}

/* void	handle_redirections(t_ast_node *node)
{
	t_redirection *redir = node->redirs;
	int fd;

	while (redir)
	{
		if (redir->type == REDIR_IN)
		{
			fd = open(redir->target, O_RDONLY);
			if (fd == -1)
			{
				perror("failed to open input file");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT)
		{
			fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("failed to open output file");
				exit(EXIT_FAILURE);
			}
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
			{
				perror("failed to open output file for append");
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
			if (fork() == 0)
				exec(node);
			else
				wait(NULL);
		}
		else if (redir->type == HEREDOC)
		{
			fd = here_doc(redir->target);  // You need to implement `here_doc` to handle heredoc behavior.
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
} */

int	noredirs_orheredoc(t_ast_node *node)
{
	t_redirection *temp;

	temp = node->redirs;
	if (!temp)
		return (0);
	while (temp)
	{
		if (temp->type == HEREDOC)
			return (0);
		temp = temp->next;
	}
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
		 //create while loop here to execute all redirections? 
		if (fork() == 0)
		{
			
			handle_redirections(node);
			if (!noredirs_orheredoc(node))
			{
				for (int fd = 3; fd < SAFE_FD_LIMIT; fd++)
        		{
            		close(fd);
        		}
			}
			if (!noredirs_orheredoc(node))
			{
				exec(node);
			}
			else
			{
				/* perror("exec error"); */
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			wait(NULL);
		}
	}
}
