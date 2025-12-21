/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:56:56 by mhaizan           #+#    #+#             */
/*   Updated: 2025/08/26 14:56:57 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static void	*ft_free(char **strs, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (NULL);
}

static char	**ft_init_vars(int *i, int *j, const char *s, char c)
{
	if (!s)
		return (NULL);
	*i = 0;
	*j = 0;
	return (ft_calloc(word_count(s, c) + 1, sizeof(char *)));
}

char	**ft_split(const char *s, char c)
{
	char	**res;
	int		start;
	int		i;
	int		j;

	res = ft_init_vars(&i, &j, s, c);
	if (!res)
		return (NULL);
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			res[j] = ft_substr(s, start, i - start);
			if (!res[j])
				return (ft_free(res, j));
			j++;
		}
		else
			i++;
	}
	res[j] = NULL;
	return (res);
}
