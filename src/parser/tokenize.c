#include "../../inc/minishell.h"


static void	add_token(char **t, char *token, int *ti)
{
	if (token && ft_strlen(token) > 0)
	{
		t[*ti] = ft_strdup(token);
		(*ti)++;
	}
	free(token);
}

static void	separate_tokens(tokenize_data *d, char *str)
{
	if (d->i > d->start)
		add_token(d->t, ft_strndup(str + d->start, d->i - d->start), &d->ti);
	if (str[d->i] == '>' && str[d->i + 1] == '>')
	{
		add_token(d->t, ft_strndup(str + d->i, 2), &d->ti);
		d->i += 2;
		d->start = d->i;
	}
	else if (str[d->i] == '<' && str[d->i + 1] == '<')
	{
		add_token(d->t, ft_strndup(str + d->i, 2), &d->ti);
		d->i += 2;
		d->start = d->i;
	}
	else
	{
		add_token(d->t, ft_strndup(str + d->i, 1), &d->ti);
		d->i++;
		d->start = d->i;
	}
}

static void	finish_loop(char *str, tokenize_data *d)
{
	if (str[d->i] == ' ' && !(d->d_quote) && !(d->s_quote))
	{
		if (d->i > d->start)
			add_token(d->t, ft_strndup(str + d->start, d->i - d->start), &d->ti);
		d->i++;
		d->start = d->i;
	}
	else if ((str[d->i] == '>' || str[d->i] == '<' || str[d->i] == '|') \
		&& !(d->d_quote) && !(d->s_quote))
		separate_tokens(d, str);
	else
		d->i++;
}

static int	fill_struct(tokenize_data *d)
{
	d->i = 0;
	d->start = 0;
	d->ti = 0;
	d->d_quote = 0;
	d->s_quote = 0;
	d->t = malloc(MAX_TOKENS * sizeof(char *));
	if (!(d->t))
		return (0);
	return (1);
}

char	**tokenize(char *str)
{
	tokenize_data	d;

	if (!fill_struct(&d))
		return (NULL);
	while (str[d.i])
	{
		if (str[d.i] == '"' && !d.s_quote)
		{
			d.d_quote = !d.d_quote;
			d.i++;
			continue ;
		}
		if (str[d.i] == '\'' && !d.d_quote)
		{
			d.s_quote = !d.s_quote;
			d.i++;
			continue ;
		}
		finish_loop(str, &d);
	}
	if (d.i > d.start)
		add_token(d.t, ft_strndup(str + d.start, d.i - d.start), &d.ti);
	d.t[d.ti] = NULL;
	return (d.t);
}
