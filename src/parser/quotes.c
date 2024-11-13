
#include "../../inc/minishell.h"

bool check_for_unclosed_quotes(char *input)
{
	int i;
	int single_quote_open;
	int double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote_open) // var = 0; !var = 1;
			single_quote_open = !single_quote_open;
		else if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (false);
	return (true);
}

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
			return (false);
		else if (str[i] == '\\' && !s_quote && !d_quote)
			return (false);
		i++;
	}
	return (true);
}

bool checker_quotes(char *input)
{
	if (!check_for_unclosed_quotes(input))
	{
		write(2, "Error unclosed quotes\n", 22);
		free(input);
		return (false);
	}
	if (!semicolon_checker(input))
	{
		write(2, "Error semicolon not supported\n", 30);
		free(input);
		return (false);
	}
	return (true);
}
