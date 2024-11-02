#include "minishell.h"

int check_for_unclosed_quotes(char *input)
{
	int i = 0;
	int single_quote_open = 0;
	int double_quote_open = 0;

	while (input[i])
	{
		if (input[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		else if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (-1);
	return (1);
}
