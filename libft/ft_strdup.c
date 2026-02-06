/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:57:15 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** FT_STRDUP - String Duplication
** =============================================================================
**
** PURPOSE:
** --------
** Allocates sufficient memory for a copy of the string s, does the copy,
** and returns a pointer to it.
**
** PIPEX USAGE:
** ------------
** When a command already contains '/' (is already a path),
** we duplicate it to maintain consistent memory management:
**   char *path = ft_strdup("/usr/bin/ls");  // Returns copy
**   // Later: free(path);  // Always safe to free
**
** PARAMETERS:
** -----------
** @s: The string to duplicate
** @return: New allocated copy of s, or NULL on failure
**
** MEMORY:
** -------
** Allocates: strlen(s) + 1 bytes
** Caller must free the returned string
**
** =============================================================================
*/

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;  /* Pointer to the duplicate string */

	/* Allocate memory for copy + null terminator */
	dup = (char *)malloc(ft_strlen(s) + 1);
	/* Check allocation success */
	if (!dup)
		return (NULL);
	/* Copy all characters from source to duplicate */
	ft_memcpy(dup, s, ft_strlen(s));
	/* Null-terminate the duplicate */
	dup[ft_strlen(s)] = '\0';
	/* Return the new copy */
	return (dup);
}
