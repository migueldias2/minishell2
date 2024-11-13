
# ifndef MINISHELL_H
#  define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <stdbool.h>

# define MAX_TOKENS 300
# define CMD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define REDIR_APPEND 5
# define HEREDOC 6
# define FILE 7
# define CD 8
# define PWD 9
# define EXPORT 10
# define UNSET 11
# define ENV 12
# define BUFFER_SIZE 1024

typedef struct s_mini
{
	int			ac;
	char		**new_tokens;
	char		**exp_tokens;
	char		**env;
	char		*line;
	char		**args;
	int			shlvl;
	int 		exit_status;
	int			pipe_fd[2];
}	t_mini;

typedef struct s_token_node
{
    char **tokens;
    struct s_token_node *next;
}	t_token_node;

typedef struct s_redirection
{
	int					type;
	char				*target;
	struct s_redirection *next;
}	t_redirection;

typedef struct s_ast_node
{
	int					type;//	Type of node
	char				*cmd; //used for command nodes
	char				**args; //arguments for command nodes
	t_redirection 		*redirs;
	struct s_ast_node 	*left;
	struct s_ast_node 	*right;
}	t_ast_node;


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

typedef struct
{
	char **t;
	int	i;
	int	start;
	int	ti;
	int	d_quote;
	int	s_quote;
}	tokenize_data;

void			ft_free_env(char **env);
void free_token_list(t_token_node *head);
void			ft_exit(t_mini *mini);
void			ft_pwd();
void			ft_env(t_mini *mini);
void			ft_export(t_mini *mini);
void			ft_unset(t_mini *mini);
void			choose_args(t_mini *mini);
void			init_args(t_mini *mini, int ac, char **envp);
void			init_envp(t_mini *mini, char **envp);
void 			replace_var_env(char **envp, char *to_found, char *to_replace);
void 			update_env(char *new_pwd, char *pwd,t_mini *mini);
void 			update_env_abs(char *pwd, char *home, t_mini *mini);
char 			*encontra_barra(char *s);
void 			update_env_back_cd(char *new_pwd, char*pwd, t_mini *mini);
void 			ft_cd(t_ast_node *node,t_mini *mini);
char 			*get_var_env(char **env, char *to_find);
char			*path_find(char **envp, char *cmd);
char			**tokenize(char *str);
void 			free_all(char **tokens, char *input);
void			free_args(char **args);
void 			free_2_all(char **args1, char **args2);
char			**expand_vars(char **new_tokens, char **env_copy);
void			shell_looping(t_mini *mini, t_ast_node *ast_root);
char			**slice_tokens(char **tokens, int i, int end);
void			free_sliced(char **sliced, int count);
t_ast_node		*new_ast_node(int type, char *command, char **args);
t_redirection	*new_redirection(int type ,char *target);
void			exec(t_ast_node *node, t_mini *mini);
char			*get_path(char *cmd);
t_ast_node	*parse_tokens(char **tokens, t_token_node **sliced_tokens_list);
int				find_pipe(char **tokens);
int				here_doc(char *str);
void			execute_ast(t_ast_node *node, t_mini *mini);
void			free_ast(t_ast_node *node);
int				noredirs_orheredoc_singlestdin(t_ast_node *node);
void			handle_redirections(t_ast_node *node);
void			ft_close_all_fds(t_mini *mini);
void			ft_close(int fd);
bool 			check_for_unclosed_quotes(char *input);
int 			semicolon_checker(char *str);
bool 			checker_quotes(char *input);
void 			print_ast(t_ast_node *root);

#endif
