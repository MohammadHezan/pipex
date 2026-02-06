/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:57:52 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** FT_STRJOIN - String Concatenation with Allocation
** =============================================================================
**
** PURPOSE:
** --------
** Allocates and returns a new string which is the concatenation of s1 and s2.
**
** PIPEX USAGE:
** ------------
** Building command paths by joining directory + "/" + command name:
**   ft_strjoin("/usr/bin", "/")  -> "/usr/bin/"
**   ft_strjoin("/usr/bin/", "ls") -> "/usr/bin/ls"
**
** PARAMETERS:
** -----------
** @s1: The prefix string
** @s2: The suffix string
** @return: New allocated string (s1 + s2), or NULL on failure
**
** MEMORY:
** -------
** Allocates: strlen(s1) + strlen(s2) + 1 bytes
** Caller must free the returned string
**
** =============================================================================
*/

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*result;   /* The new concatenated string */
	size_t	s1_len;    /* Length of first string */
	size_t	s2_len;    /* Length of second string */

	/* Validate inputs - can't concatenate NULL strings */
	if (!s1 || !s2)
		return (NULL);
	/* Calculate lengths once (avoid multiple strlen calls) */
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	/* Allocate memory for combined string + null terminator */
	result = (char *)malloc(s1_len + s2_len + 1);
	/* Check allocation success */
	if (!result)
		return (NULL);
	/* Copy first string to beginning of result */
	ft_memcpy(result, s1, s1_len);
	/* Copy second string after the first */
	ft_memcpy(result + s1_len, s2, s2_len);
	/* Null-terminate the result */
	result[s1_len + s2_len] = '\0';
	/* Return the new string */
	return (result);
}
