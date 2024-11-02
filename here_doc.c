
#include "minishell.h"

void	here_doc(char *str)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	bytes_read;

	fd = open("temp.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		perror("error failed to open");
	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
		if (bytes_read < 0)
			perror("error read");
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (strncmp(buffer, str, strlen(str)) == 0 && \
	(buffer[strlen(str)] == '\n' || buffer[strlen(str)] == '\0'))
			break ;
		write(fd, buffer, bytes_read);
	}
	close(fd);
	fd = open("temp.txt", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	unlink("temp.txt");
	close(fd);
}
