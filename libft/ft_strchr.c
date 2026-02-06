/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:57:06 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** FT_STRCHR - Character Search in String
** =============================================================================
**
** PURPOSE:
** --------
** Locates the first occurrence of character c in string s.
** The null terminator is considered part of the string.
**
** PIPEX USAGE:
** ------------
** Checking if a command is already a path (contains '/'):
**   if (ft_strchr(cmd, '/'))  // cmd is a path like "/bin/ls" or "./script"
**       return (ft_strdup(cmd));  // Use as-is, don't search PATH
**
** PARAMETERS:
** -----------
** @s: The string to search
** @c: The character to find (passed as int, converted to char)
** @return: Pointer to first occurrence of c, or NULL if not found
**
** NOTES:
** ------
** - Returns pointer into original string (not a copy)
** - Can find '\0' if c is '\0'
** - Character c is cast to char for comparison
**
** =============================================================================
*/

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;   /* Index for iterating through string */
	char	ch;  /* Character to search for (cast from int) */

	/* Cast int to char (handles values > 255) */
	ch = (char)c;
	i = 0;
	/* Search through the string */
	while (s[i])
	{
		/* If current character matches, return pointer to it */
		if (s[i] == ch)
			return ((char *)&s[i]);
		i++;  /* Move to next character */
	}
	/* Check if searching for null terminator */
	if (ch == s[i])
		return ((char *)&s[i]);
	/* Character not found */
	return (NULL);
}
