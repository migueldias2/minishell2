#include "minishell.h"

int semicolon_checker(char *str)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == ';' && !s_quote && !d_quote)
			return (-1);
		else if (str[i] == '\\' && !s_quote && !d_quote)
			return (-1);
		i++;
	}
	return (0);
}
