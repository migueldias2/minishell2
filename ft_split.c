/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:05:44 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/02 14:12:44 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	i;
	size_t	word_count;

	i = 0;
	word_count = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] == c)
		{
			while (s[i] == c)
				i++;
		}
		if (s[i] != c && s[i])
		{
			word_count++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (word_count);
}

static void	ft_strncpy(char *dest, char *src, size_t n)
{
	while (*src && n > 0)
	{
		*dest = *src;
		dest++;
		src++;
		n--;
	}
	*dest = '\0';
}

static int	ft_fill(char **list, char const *s, char c)
{
	char	*start;
	size_t	len;
	size_t	index;

	index = 0;
	while (*s)
	{
		if (*s != c && *s)
		{
			start = (char *)s;
			while (*s != c && *s)
				s++;
			len = s - start;
			list[index] = (char *)malloc((len + 1) * sizeof(char));
			if (!list[index])
				return (0);
			ft_strncpy(list[index], start, len);
			index++;
		}
		else
			s++;
	}
	list[index] = NULL;
	return (1);
}

static void	ft_freelist(char **list, size_t total_words)
{
	size_t	i;

	i = 0;
	while (i < total_words)
	{
		free(list[i]);
		i++;
	}
	free(list);
}

char	**ft_split(char const *s, char c)
{
	size_t	total_words;
	char	**list;

	if (!s)
		return (NULL);
	total_words = ft_wordcount(s, c);
	list = (char **)malloc((total_words + 1) * sizeof(char *));
	if (!list)
		return (NULL);
	if (!ft_fill(list, s, c))
	{
		ft_freelist(list, total_words);
		return (NULL);
	}
	return (list);
}
/*int	main()
{
	size_t	i;
	char	**ptr = ft_split("      ola     mundo    tudo   bem    ", 'o');

	i = 0;
	while (ptr[i])
	{
		printf("%s\n", ptr[i]);
		i++;
	}
}*/
