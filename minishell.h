#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>

#define MAX_TOKENS 300


#include <fcntl.h>

#define CMD 1
#define PIPE 2
#define REDIR_IN 3
#define REDIR_OUT 4
#define REDIR_APPEND 5
#define HEREDOC 6
#define FILE 7

# define BUFFER_SIZE 1024


//define node structure of the AST
typedef struct s_ast_node
{
	int		type;//	Type of node
	char	*cmd; //used for command nodes
	char	**args; //arguments for command nodes
	char	*file; //in case of redirections
	char	*delimiter; // for here documents
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;

t_ast_node	*parse_tokens(char **tokens);
char		**slice_tokens(char **tokens, int i, int end);
int			find_redirection(char **tokens);
int			find_pipe(char **tokens);


// estrutura para expand_vars.c
typedef struct
{
	char	*exp;
	char	*var_start;
	char	*temp;
	int		i;
	int		d_quote;
	int		s_quote;
	int		len;
	int		start;
}	expand_data;

//estrutura para tokenize.c
typedef struct
{
	char **t;
	int	i;
	int	start;
	int	ti;
	int	d_quote;
	int	s_quote;
}	tokenize_data;

void	free_all(char **tokens, char *input);
int		check_for_unclosed_quotes(char *input);
char	**copy_env_vars(char **env);
char	**tokenize(char *str);
int		semicolon_checker(char *str);
char	**expand_vars(char **new_tokens, char **env_copy);
void	execute_ast(t_ast_node *node);
t_ast_node	*parse_tokens(char **tokens);
void	here_doc(char *str);
char	*get_path(char *cmd);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);


#endif
