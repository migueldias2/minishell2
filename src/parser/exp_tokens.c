
#include "../../inc/minishell.h"

static char	*replace(char *var_name, char **env_copy)
{
	int		i;
	char	*var_string;
	char	*value;

	i = 0;
	while (env_copy[i])
	{
		var_string = env_copy[i];
		if ((ft_strncmp(var_name, var_string, ft_strlen(var_name)) == 0) \
			&& var_string[ft_strlen(var_name)] == '=')
		{
			value = ft_strdup(var_string + ft_strlen(var_name) + 1);
			free(var_name);
			return (value);
		}
		i++;
	}
	free(var_name);
	return (ft_strdup(""));
}

static void	token_expand(expand_data *d, char *str, char **env_copy)
{
	d->start = d->i + 1;
	while (str[d->i + 1] && str[d->i + 1] != ' ' && \
		str[d->i + 1] != '"' && str[d->i + 1] != '\'' && \
		str[d->i + 1] != '$')
	{
		d->i++;
		d->var_start = ft_strndup(&str[d->start], d->i - d->start + 1);
		d->temp = replace(d->var_start, env_copy);
		d->exp = (char*)ft_realloc(d->exp, ft_strlen(d->exp) + ft_strlen(d->temp) + 1, ft_strlen(d->exp)+1);
		strcat(d->exp, d->temp);
		free(d->temp);
	}
}

static char	*expand_var(char *str, char **env_copy)
{
	expand_data	d;

	d.exp = ft_strdup("");
	d.i = 0;
	d.d_quote = 0;
	d.s_quote = 0;
	while (str[d.i])
	{
		if (str[d.i] == '"' && !d.s_quote)
			d.d_quote = !d.d_quote;
		else if (str[d.i] == '\'' && !d.d_quote)
			d.s_quote = !d.s_quote;
		else if (str[d.i] == '$' && str[d.i + 1] && (d.d_quote || !d.s_quote))
			token_expand(&d, str, env_copy);
		else
		{
			d.len = ft_strlen(d.exp);
			d.exp = (char *)ft_realloc(d.exp, d.len + 2, ft_strlen(d.exp)+1);
			d.exp[d.len] = str[d.i];
			d.exp[d.len + 1] = '\0';
		}
		d.i++;
	}
	free(str);
	return (d.exp);
}

char	**expand_vars(char **new_tokens, char **env_copy)
{
	int		i;
	char	*var;
	char	**exp_tokens;
	int		len;

	i = 0;
	if (new_tokens[i] && new_tokens[i][0] == '|')
	{
		perror("pipe error");
		return (NULL);
	}
	while (new_tokens[i])
	{
		if (new_tokens[i][0] == '|')
		{
			// Check if next token exists and is valid
			if (new_tokens[i+1] == NULL || new_tokens[i+1][0] == '|')
			{
				perror("pipe error");
				return (NULL);
			}
		}
		i++;
	}
	exp_tokens = malloc((i + 1) * sizeof(char *));
	if (!exp_tokens)
		return (NULL);
	i = 0;
	while (new_tokens[i])
	{
		len = ft_strlen(new_tokens[i]);
		var = expand_var(ft_strndup(new_tokens[i], len), env_copy);
		free(new_tokens[i]);
		exp_tokens[i] = var;
		i++;
	}
	free(new_tokens);
	exp_tokens[i] = NULL;
	return (exp_tokens);
}
