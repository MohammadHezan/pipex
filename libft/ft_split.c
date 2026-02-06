/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:56:56 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** FT_SPLIT - String Tokenizer
** =============================================================================
**
** PURPOSE:
** --------
** Splits a string into an array of substrings using a delimiter character.
** This is one of the most critical functions in pipex because:
**
** 1. Splitting command strings: "grep hello" -> {"grep", "hello", NULL}
** 2. Splitting PATH variable: "/bin:/usr/bin" -> {"/bin", "/usr/bin", NULL}
**
** EXAMPLE:
** --------
** char **result = ft_split("hello world foo", ' ');
**
** result[0] = "hello"
** result[1] = "world"
** result[2] = "foo"
** result[3] = NULL
**
** MEMORY LAYOUT:
** --------------
**                    ┌─────────┐
** result ──────────► │ ptr[0] ─┼──► "hello\0"
**                    ├─────────┤
**                    │ ptr[1] ─┼──► "world\0"
**                    ├─────────┤
**                    │ ptr[2] ─┼──► "foo\0"
**                    ├─────────┤
**                    │  NULL   │
**                    └─────────┘
**
** IMPORTANT:
** ----------
** - The caller is responsible for freeing all memory
** - Use ft_free_split() or similar to properly free
** - Returns NULL on allocation failure
**
** =============================================================================
*/

#include "libft.h"

/*
** word_count - Counts the number of words separated by delimiter
** --------------------------------------------------------------
** @str: The input string to analyze
** @c: The delimiter character
** @return: Number of words in the string
**
** Algorithm: Uses a state machine approach with 'in_word' flag
** - Transitions from "not in word" to "in word" = found new word
** - Transitions from "in word" to "not in word" = word ended
**
** Example: word_count("  hello  world  ", ' ')
**   Position:  0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15
**   Character: ' '' ''h''e''l''l''o'' '' ''w''o''r''l''d'' '' '
**   in_word:   0  0  1  1  1  1  1  0  0  1  1  1  1  1  0  0
**   count:     0  0  1  1  1  1  1  1  1  2  2  2  2  2  2  2
**   Result: 2 words
*/
static int	word_count(const char *str, char c)
{
	int	count;    /* Number of words found */
	int	in_word;  /* Flag: currently inside a word? */

	count = 0;    /* No words counted yet */
	in_word = 0;  /* Start outside any word */
	/* Iterate through every character in the string */
	while (*str)
	{
		/* If current char is NOT delimiter AND we're not in a word */
		/* This means we just entered a new word */
		if (*str != c && in_word == 0)
		{
			in_word = 1;  /* Now we're in a word */
			count++;      /* Count this new word */
		}
		/* If current char IS the delimiter */
		/* This means we exited the word (or between delimiters) */
		else if (*str == c)
			in_word = 0;  /* No longer in a word */
		str++;  /* Move to next character */
	}
	return (count);  /* Return total word count */
}

/*
** ft_free - Frees partially allocated array on error
** ---------------------------------------------------
** @strs: Array of strings to free
** @count: Number of strings successfully allocated
** @return: Always returns NULL (for convenient error return)
**
** This is called when allocation fails midway through ft_split.
** It frees everything that was allocated before the failure.
*/
static void	*ft_free(char **strs, int count)
{
	int	i;  /* Loop counter */

	i = 0;
	/* Free each string that was allocated */
	while (i < count)
	{
		free(strs[i]);  /* Free individual string */
		i++;
	}
	/* Free the array of pointers itself */
	free(strs);
	/* Return NULL to indicate failure */
	return (NULL);
}

/*
** ft_init_vars - Initializes variables and allocates result array
** ----------------------------------------------------------------
** @i: Pointer to string index (will be set to 0)
** @j: Pointer to result array index (will be set to 0)
** @s: Input string to check for NULL
** @c: Delimiter character (used to count words)
** @return: Allocated array of pointers, or NULL on failure
**
** This helper function:
** 1. Validates input (s != NULL)
** 2. Initializes loop indices to 0
** 3. Allocates the result array with size = word_count + 1 (for NULL)
*/
static char	**ft_init_vars(int *i, int *j, const char *s, char c)
{
	/* Check for NULL input string */
	if (!s)
		return (NULL);  /* Cannot split NULL */
	/* Initialize indices to start of string/array */
	*i = 0;  /* Index into input string s */
	*j = 0;  /* Index into result array */
	/* Allocate array of pointers */
	/* word_count(s, c) + 1: extra slot for NULL terminator */
	/* ft_calloc zeros memory, ensuring NULL terminator */
	return (ft_calloc(word_count(s, c) + 1, sizeof(char *)));
}

/*
** ft_split - Splits string by delimiter into array of strings
** ------------------------------------------------------------
** @s: The string to split
** @c: The delimiter character
** @return: NULL-terminated array of strings, or NULL on error
**
** Algorithm:
** 1. Count words to allocate correct array size
** 2. For each character:
**    - If not delimiter: mark start, find end, extract substring
**    - If delimiter: skip it
** 3. Return the filled array
**
** Memory ownership: Caller must free the returned array and all strings.
*/
char	**ft_split(const char *s, char c)
{
	char	**res;   /* Result array of strings */
	int		start;  /* Start index of current word */
	int		i;      /* Current position in input string */
	int		j;      /* Current position in result array */

	/* Initialize variables and allocate result array */
	res = ft_init_vars(&i, &j, s, c);
	if (!res)
		return (NULL);  /* Allocation failed */
	/* Process each character in the input string */
	while (s[i])
	{
		/* If current char is NOT the delimiter (start of a word) */
		if (s[i] != c)
		{
			start = i;  /* Mark beginning of word */
			/* Find end of word (advance until delimiter or end of string) */
			while (s[i] && s[i] != c)
				i++;
			/* Extract the word as a substring */
			res[j] = ft_substr(s, start, i - start);
			/* Check if substring allocation failed */
			if (!res[j])
				return (ft_free(res, j));  /* Clean up and return NULL */
			j++;  /* Move to next slot in result array */
		}
		else
			i++;  /* Skip the delimiter character */
	}
	res[j] = NULL;  /* NULL-terminate the array */
	return (res);   /* Return the completed array */
}
