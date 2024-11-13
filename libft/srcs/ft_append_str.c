/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 14:36:31 by pesilva-          #+#    #+#             */
/*   Updated: 2024/08/28 18:06:52 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_append_str(const char *fix, char *s1, char *s2)
{
	char	*inter;
	char	*final_boss;

	if (!fix || !s1 || !s2)
		return (NULL);
	inter = ft_strjoin(fix, s1);
	if (!inter)
		return (NULL);
	final_boss = ft_strjoin(inter, s2);
	free(inter);
	if (!final_boss)
		return (NULL);
	return (final_boss);
}
