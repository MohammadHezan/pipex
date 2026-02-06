/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:58:43 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** FT_STRNCMP - String Comparison (Limited)
** =============================================================================
**
** PURPOSE:
** --------
** Compares at most n characters of two strings lexicographically.
** Returns the difference between the first differing characters.
**
** PIPEX USAGE:
** ------------
** Finding the PATH environment variable:
**   while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
**       i++;  // Skip until we find "PATH=..."
**
** PARAMETERS:
** -----------
** @s1: First string to compare
** @s2: Second string to compare
** @n: Maximum number of characters to compare
** @return: 0 if equal, negative if s1 < s2, positive if s1 > s2
**
** RETURN VALUES:
** --------------
** - Returns 0 if first n characters are identical
** - Returns negative if s1 comes before s2 lexicographically
** - Returns positive if s1 comes after s2 lexicographically
** - Returns 0 if n is 0 (no comparison made)
**
** =============================================================================
*/

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;  /* Index for comparing characters */

	/* If n is 0, no comparison needed - strings are "equal" */
	if (n == 0)
		return (0);
	i = 0;
	/* Compare characters while they're equal and within limit */
	/* Stop at n-1 so we can return the comparison at position i */
	while (i < n - 1 && s1[i] == s2[i])
		i++;  /* Characters match, continue */
	/* Return difference between characters at final position */
	/* Cast to unsigned char to handle negative char values correctly */
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
